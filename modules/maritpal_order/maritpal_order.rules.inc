<?php

/**
 * Implement hook_rules_action_info().
 */
function maritpal_order_rules_action_info() {
  return array(
    'maritpal_order_service_request_pricing_calculation' => array(
      'label' => t('CUSTOM PRICING RULE | Service request pricing calculation'),
      'group' => t('Custom'),
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

	$locs = array();
	foreach ($node->field_serv_loc_city['und'] as $key => $value) {
		$locs[] = $value['tid'];
	}

	$cats = $node->field_request_serv_cat['und'][0]['tid'];

	$locs_revert = array_reverse($locs);

	// -- fetch price rules terms
	$query = new EntityFieldQuery();

	$query->entityCondition('entity_type', 'taxonomy_term')
		->propertyCondition('vid', 23)
		->fieldCondition('field_serv_cat', 'tid', $cats, '=')
		->fieldCondition('field_serv_loaction', 'tid', $locs_revert, 'CONTAINS')
		->addMetaData('account', user_load(4));

	$result = $query->execute();

	if (isset($result['taxonomy_term'])) {
		$tids = array_keys($result['taxonomy_term']);

		foreach ($locs_revert as $l => $loc) {
			foreach ($tids as $t => $tid) {
				$temp_term = taxonomy_term_load($tid);
				$tloc = $temp_term->field_serv_loaction['und'][0]['tid'];

				if ($loc == $tloc) {
					$term = $temp_term;
					break 2;
				}
			}
		}
	}

	// If no pricing rule find use default
	if (!$term) $term = taxonomy_term_load(1988);

	// -- price parameters
	$params = array(
		'date_val' => strtotime($node->field_date_and_time['und'][0]['value']),
		'date_val2' => strtotime($node->field_date_and_time['und'][0]['value2']),
		'rate' => intval($term->field_serv_rate['und'][0]['value']),
		'price_hp' => array(
			'amount' => intval($term->field_serv_price_ph['und'][0]['amount']),
			'currency_code' => $term->field_serv_price_ph['und'][0]['currency_code'],
		)
	);
	$params['duration'] = ($params['date_val2'] - $params['date_val']) / 3600;

	$price = array(
		'amount' => round($params['duration'] * ($params['rate'] * $params['price_hp']['amount']), 0, PHP_ROUND_HALF_UP),
		'currency_code' => $params['price_hp']['currency_code'],
		'taxes' => array(),
	);

	// -- Admin rate/Transsfer rate/Bank fee
  // admin fixed
  $adm_type = variable_get_value('pricing:admin_rate:type');
  $adm_val = variable_get_value('pricing:admin_rate:value');

  if ($adm_type == 0) $price['taxes']['admin'] = $adm_val;
  else $price['taxes']['admin'] = round(($price['amount'] / 100) * $adm_val, 0);

  // transfer fixed
  $trns_type = variable_get_value('pricing:transfer_rate:type');
  $trns_val = variable_get_value('pricing:transfer_rate:value');

  if ($trns_type == 0) $price['taxes']['transfer'] = $trns_val;
  else $price['taxes']['transfer'] = round(($price['amount'] / 100) * $trns_val, 0);

  // bank fee
  $bank_fee = variable_get_value('pricing:bank_fee:value');

  if ($bank_fee) $price['taxes']['bank_fee'] = round(($price['amount'] / 100) * $bank_fee, 0);


	$line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $commerce_line_item);

  // -- Line item total price
	$sum = $price['amount'] + $price['taxes']['admin'] + $price['taxes']['transfer'] + $price['taxes']['bank_fee'];
	$line_item_wrapper->commerce_unit_price->amount = $sum;
	
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
	 * Bank fee price component
	 */
	$line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
		$line_item_wrapper->commerce_unit_price->value(),
		'bank_transfer_fee',
		array(
			'amount' => $price['taxes']['bank_fee'],
			'currency_code' => $price['currency_code'],
			'data' => array(),
		),
		FALSE
	);

	// dpm($commerce_line_item);
	// commerce_line_item_rebase_unit_price($line_item_wrapper->value());

	/** Invoke UI action set Add Administrative pricing taxes */
	// $result = rules_invoke_component('rules_add_admin_rates', $commerce_line_item);

	commerce_line_item_save($commerce_line_item);

	/** Reset DC line-items cache */
	entity_get_controller('commerce_line_item')->resetCache(array($commerce_line_item->line_item_id));
}
