<?php
/**
 * @file
 * Defines the actions of adding and perchasing money to ones account.
 */

/**
 * Implements hook_rules_action_info().
 */
function maritpal_account_balance_rules_action_info() {
  $currency = commerce_currency_load();

  return array(
    'maritpal_account_balance_deposit_to_account' => array(
      'label' => t("Deposit money to a user's account"),
      'parameter' => array(
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
          'optional' => FALSE,
          'allow null' => FALSE,
        ),
        'amount' => array(
          'label' => t('Amount in !currency', array('!currency' => $currency['symbol'])),
          'type' => 'decimal',
          'allow null' => TRUE,
          'optional' => TRUE,
          'description' => 'A decimal amount of money to add to the balance',
        ),
        'order_id' => array(
          'label' => t('Order ID'),
          'type' => 'integer',
          'allow null' => TRUE,
          'optional' => TRUE,
          'description' => 'The order ID this transaction belongs to (optional)',
        ),
      ),
      'group' => t('Commerce Account Balance'),
    ),
    'maritpal_account_balance_calculating_sell_price' => array(
      'label' => t('ACC BALANCE | Calculating sell price'),
      'group' => t('Maritpal Account Balance'),
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('user'),
        ),
        // 'deposit_amount' => array(
        //   'type' => 'decimal',
        //   'label' => t('deposit-amount'),
        // ),
        // 'deposit_currency_code' => array(
        //   'type' => 'text',
        //   'label' => t('deposit-currency-code'),
        // ),
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
 * Action callback for depositing money to ones account.
 */
function maritpal_account_balance_deposit_to_account($user, $amount_decimal, $order_id = NULL) {
  $amount = round($amount_decimal * 100);
  $uid = $user->uid;

  $currency = commerce_currency_load();
  $formatted_amount = commerce_currency_format($amount, $currency['code']);

  $transaction = entity_create('maritpal_account_balance_transaction', array(
      'type' => 'deposit',
      'action' => t('Deposit of @amount to your account', array('@amount' => $formatted_amount)),
      'uid' => $uid,
      'time' => REQUEST_TIME,
      'amount' => $amount,
      'order_id' => $order_id ? $order_id : NULL,
    ));

  $transaction_wrapper = entity_metadata_wrapper('maritpal_account_balance_transaction', $transaction);
  $transaction_wrapper->save();
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
function maritpal_account_balance_calculating_sell_price($account, $commerce_line_item) {
  dpm($commerce_line_item);

	// $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $commerce_line_item);

  // // -- Line item total price
	// $line_item_wrapper->commerce_unit_price->amount = $commerce_line_item->commerce_deposit_amount['und'][0]['value'];
	// $line_item_wrapper->commerce_unit_price->currency_code = $commerce_line_item->commerce_deposit_currency_code['und'][0]['value'];
	
	// $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
	// 	$line_item_wrapper->commerce_unit_price->value(),
	// 	'base_price',
	// 	array(
	// 		'amount' => $commerce_line_item->commerce_deposit_amount['und'][0]['value'],
	// 		'currency_code' => $commerce_line_item->commerce_deposit_currency_code['und'][0]['value'],
	// 		'data' => array(),
	// 	),
	// 	TRUE
	// );	

	// // dpm($commerce_line_item);

	// /** Invoke UI action set Add Administrative pricing taxes */
	// // $result = rules_invoke_component('rules_add_admin_rates', $commerce_line_item);

	// $line_item_wrapper->save();

	// /** Reset DC line-items cache */
	// entity_get_controller('commerce_line_item')->resetCache(array($commerce_line_item->line_item_id));
}
