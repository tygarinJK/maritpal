<?php

/**
 * Implement hook_rules_action_info().
 */
function maritpal_order_rules_action_info() {
  return array(
    'maritpal_order_service_request_pricing_calculation' => array(
      'label' => t('CUSTOM PRICING RULE | Service request pricing calculation'),
      'group' => t('Maritpal Pricing'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('node'),
        ),
        'commerce_line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('commerce line item'),
        ),
      ),
      'provides' => array(),
    ),
		'maritpal_order_service_request_pricing_rate_calculation' => array(
      'label' => t('Calculate service request price rate depend on pricing rule'),
      'group' => t('Maritpal Pricing'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('node'),
        ),
      ),
      'provides' => array(
				'price_amount' => array(
					'type' => 'decimal',
					'label' => t('Price amount')
				),
				'price_currency_code' => array(
					'type' => 'text',
					'label' => t('Price currency code')
				),
			),
    ),
		'maritpal_order_account_balance_pricing_calculation' => array(
      'label' => t('Calculating Balance sell Price'),
      'group' => t('Maritpal Account Balance'),
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('user'),
        ),
        'commerce_line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('commerce-line-item'),
        ),
      ),
      'provides' => array(),
    ),
  );
}


/**
 * Callback function to maritpal_order_rules_action_info
 * 
 * @see maritpal_order_rules_action_info()
 *
 * @param object $node
 * 	Service Request Node
 * @param object $commerce_line_item
 * 	Commerce Line Item with service order reference
 */
function maritpal_order_service_request_pricing_calculation($node, $commerce_line_item) {

	$location = array();
	if (isset($node->field_serv_loc_city['und'])) {
		foreach ($node->field_serv_loc_city['und'] as $key => $value) {
			$location[] = $value['tid'];
		}
	}
	$category = array(isset($node->field_request_serv_cat['und']) ? $node->field_request_serv_cat['und'][0]['tid'] : 0);
	$date = array(
		'value' => $node->field_date_and_time['und'][0]['value'],
		'value2' => $node->field_date_and_time['und'][0]['value2'],
	);

	if (isset($node->field_resource_ref['und'][0]['uid'])) {
		$provider = user_load($node->field_resource_ref['und'][0]['uid']);
		$mail = $provider->mail;
	} else $mail = NULL;

	$pricing_rule = maritpal_service_order_price_rule_term_load($location, $category, $date, $mail);

	if (isset($pricing_rule->tid)) {
		// -- price parameters
		$params = array(
			'date_val' => strtotime($date['value']),
			'date_val2' => strtotime($date['value2']),
			'rate' => floatval($pricing_rule->field_serv_rate['und'][0]['value']),
			'per' => $pricing_rule->field_serv_rate_dur['und'][0]['value'],
			'price_hp' => array(
				'amount' => intval($pricing_rule->field_serv_rate_amount['und'][0]['value']),
				'currency_code' => $pricing_rule->field_serv_rate_currency['und'][0]['currency_code'],
			)
		);
		$params['duration'] = ($params['date_val2'] - $params['date_val']) / 3600;

		$price = array(
			'currency_code' => $params['price_hp']['currency_code'],
			'taxes' => array(),
			'discount' => 0,
		);

		if ($params['per'] == 'hour')
			$price['amount'] = round($params['duration'] * ($params['rate'] * $params['price_hp']['amount']), 0, PHP_ROUND_HALF_UP);
		else
			$price['amount'] = $params['price_hp']['amount'];

		// -- Admin rate/Transsfer rate/Bank fee
		// admin tax
		$admin_tax = variable_get('maritpal_admin_rate', 0);
		$price['taxes']['admin'] = round(($price['amount'] / 100) * $admin_tax, 0);

		// transfer tax
		$transfer_tax = variable_get('maritpal_transfer_rate', 0);
		$price['taxes']['transfer'] = round(($price['amount'] / 100) * $transfer_tax, 0);

		// surcharge tax
		$surcharge_tax = variable_get('maritpal_surcharge_rate', 0);
		$price['taxes']['surcharge'] = round(($price['amount'] / 100) * $surcharge_tax, 0);

		// additional taxes
		$price['taxes']['additional']['sum'] = 0;
		$addditional_taxes = variable_get('maritpal_additional_taxes', array());
		foreach ($addditional_taxes as $i => $tax) {
			$price['taxes']['additional'][] = array(
				'name' => $tax['name'],
				'amount' => round(($price['amount'] / 100) * $tax['value'], 0),
			);
			$price['taxes']['additional']['sum'] += round(($price['amount'] / 100) * $tax['value'], 0);
		}

		$line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $commerce_line_item);

		// -- Line item total price
		$sum = $price['amount'] + $price['taxes']['admin'] + $price['taxes']['transfer'] + $price['taxes']['surcharge'] + $price['taxes']['additional']['sum'];
		// $sum = 0;
		// foreach ($price['taxes'] as $key => $value) {
		// 	if ($key == 'additional') $sum += $value['sum'];
		// 	else $sum += $value;
		// }
		
		$line_item_wrapper->commerce_unit_price->amount = $sum;
		$line_item_wrapper->commerce_unit_price->currency_code = $price['currency_code'];
		
		/**
		* Base price component
		*/
		$line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
			$line_item_wrapper->commerce_unit_price->value(),
			'base_price',
			array(
				'amount' => $price['amount'],
				'currency_code' => $price['currency_code'],
				'data' => array(),
			),
			TRUE
		);	

		/**
		* Admin rate price component
		*/
		$line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
			$line_item_wrapper->commerce_unit_price->value(),
			'admin_rate',
			array(
				'amount' => $price['taxes']['admin'],
				'currency_code' => $price['currency_code'],
				'data' => array(),
			),
			FALSE
		);

		/**
		* Transfer rate price component
		*/
		$line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
			$line_item_wrapper->commerce_unit_price->value(),
			'transfer_rate',
			array(
				'amount' => $price['taxes']['transfer'],
				'currency_code' => $price['currency_code'],
				'data' => array(),
			),
			FALSE
		);

		/**
		* Surcharge fee price component
		*/
		$line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
			$line_item_wrapper->commerce_unit_price->value(),
			'surcharge_rate',
			array(
				'amount' => $price['taxes']['surcharge'],
				'currency_code' => $price['currency_code'],
				'data' => array(),
			),
			FALSE
		);
		
		/**
		* Additional taxes price component
		*/
		$line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
			$line_item_wrapper->commerce_unit_price->value(),
			'additional_taxes',
			array(
				'amount' => $price['taxes']['additional']['sum'],
				'currency_code' => $price['currency_code'],
				'data' => array(),
			),
			FALSE
		);

		// dpm($commerce_line_item);
		// commerce_line_item_rebase_unit_price($line_item_wrapper->value());

		commerce_line_item_save($commerce_line_item);

		/** Reset DC line-items cache */
		entity_get_controller('commerce_line_item')->resetCache(array($commerce_line_item->line_item_id));
	}
}

/**
 * Callback function to maritpal_order_rules_action_info
 * 
 * @see maritpal_order_rules_action_info()
 *
 * @param object $node
 * 	Service Request Node
 */
function maritpal_order_service_request_pricing_rate_calculation($node) {

	$location = array();
	if (isset($node->field_serv_loc_city['und'])) {
		foreach ($node->field_serv_loc_city['und'] as $key => $value) {
			$location[] = $value['tid'];
		}
	}
	$category = array(isset($node->field_request_serv_cat['und']) ? $node->field_request_serv_cat['und'][0]['tid'] : 0);
	$date = array(
		'value' => $node->field_date_and_time['und'][0]['value'],
		'value2' => $node->field_date_and_time['und'][0]['value2'],
	);

	if (isset($node->field_resource_ref['und'][0]['uid'])) {
		$provider = user_load($node->field_resource_ref['und'][0]['uid']);
		$mail = $provider->mail;
	} else $mail = NULL;

	$price = maritpal_services_old_calculate_request_service_price_rate($location, $category, $date, $mail);
	$sum  = $price->data->components->base_price;
	$sum += $price->data->components->admin_tax;
	$sum += $price->data->components->transfer_tax; 
	$sum += $price->data->components->surcharge;
	$sum += $price->data->components->additional_taxes;

	return array(
		'price_amount' => $sum,
		'price_currency_code' => $price->data->currency_code
	);
}

/**
 * Callback function to maritpal_order_rules_action_info
 * 
 * @see maritpal_order_rules_action_info()
 *
 * @param object $user
 * 	User frofile
 * @param object $commerce_line_item
 * 	Commerce Line Item with service order reference
 */
function maritpal_order_account_balance_pricing_calculation($user, $commerce_line_item) {
	dpm($commerce_line_item);

	// $commerce_line_item->commerce_unit_price['und'][0]['amount'] = 300;
	// $commerce_line_item->commerce_unit_price['und'][0]['currency_code'] = 'EUR';
	// $commerce_line_item->commerce_unit_price['und'][0]['data']['components'][0]['price']['amount'] = 300;
	// $commerce_line_item->commerce_unit_price['und'][0]['data']['components'][0]['price']['currency_code'] = 'EUR';
	$line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $commerce_line_item);

  // -- Line item total price
	$sum = 300;
	$line_item_wrapper->commerce_unit_price->amount = $sum;
	$line_item_wrapper->commerce_unit_price->currency_code = 'EUR';
	
	/**
	 * Base price component
	 */
	$line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
		$line_item_wrapper->commerce_unit_price->value(),
		'base_price',
		array(
			'amount' => $sum,
			'currency_code' => 'EUR',
			'data' => array(),
		),
		TRUE
	);	

	// dpm($commerce_line_item);
	commerce_line_item_rebase_unit_price($line_item_wrapper->value());

	/** Invoke UI action set Add Administrative pricing taxes */
	// $result = rules_invoke_component('rules_add_admin_rates', $commerce_line_item);

	commerce_line_item_save($commerce_line_item);

	/** Reset DC line-items cache */
	entity_get_controller('commerce_line_item')->resetCache(array($commerce_line_item->line_item_id));
}
