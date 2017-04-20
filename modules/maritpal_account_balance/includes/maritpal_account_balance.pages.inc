<?php
/**
 * @file
 * Page callbacks.
 */

/**
 * Review Account balance and balance transactions
 *
 * @param obj $account
 *  User account object
 */
function maritpal_account_balance_review($account) {
  
  return 'TODO: output account balance and transactions table';
}

/**
 * Refill Account balance form
 *
 * @param obj $account
 *  User account object
 */
function maritpal_account_balance_refill_form($form, &$form_state, $account) {
  $form_state['user'] = $account;
  
  // field_attach_form('commerce_line_item', $line_item, $form, $form_state);

  $form['markup'] = array(
    '#markup' => 'TODO: output account balance refill form!!',
  );

  // $form['commerce_unit_price'][LANGUAGE_NONE][0] = array(
  //   'amount' => array(
  //     '#type' => 'hidden',
  //     '#value' => 0,
  //   ),
  //   'currency_code' => array(
  //     '#type' => 'hidden',
  //     '#value' => commerce_default_currency(),
  //   ),
  // );

  $form['balance_amount'] = array(
		'#type' => 'textfield',
		'#title' => t('Balance'),
		'#required' => TRUE,
		'#weight' => -50
	);
	
	$form['balance_currency'] = array(
		'#type' => 'select',
		'#description' => t('Please select currency of Your location'),
		'#options' => _get_enabled_currencies_options(),
		'#required' => TRUE,
		'#weight' => -49
	);

	$form['actions'] = array(
		'#type' => 'container',
	);
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Deposit cash')
	);

  // $result = maritpal_account_balance_transaction_get($account);
  // dpm($result);
  dpm($form);

  return $form;
}

function maritpal_account_balance_refill_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['balance_amount']) || $form_state['values']['balance_amount'] <= 0)
    form_set_error('balance_amount', t('Amount must be positive number'));
}

function maritpal_account_balance_refill_form_submit($form, &$form_state) {
  $account = $form_state['user'];
  $type = 'deposit';
  $amount_decimal = $form_state['values']['balance_amount'];
  $currency_code = $form_state['values']['balance_currency'];

  $product = commerce_product_load(6);
  $line_item = commerce_product_line_item_new($product);
  $order = commerce_order_new();

  // field_attach_submit('commerce_line_item', $line_item, $form, $form_state);

  // Use the official hook to recalculate the unit price.

  $line_item->commerce_unit_price[LANGUAGE_NONE][0]['amount'] = $amount_decimal;
  $line_item->commerce_unit_price[LANGUAGE_NONE][0]['currency_code'] = $currency_code;

  $line_item->commerce_deposit_amount[LANGUAGE_NONE][0]['value'] = $amount_decimal;
  $line_item->commerce_deposit_currency_code[LANGUAGE_NONE][0]['value'] = $currency_code;

  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $line_item_wrapper->commerce_unit_price->amount = $amount_decimal;
  $line_item_wrapper->commerce_unit_price->currency_code = $currency_code;

  // commerce_line_item_rebase_unit_price($line_item);
  $line_item_wrapper->save();

  /** Reset DC line-items cache */
	// entity_get_controller('commerce_line_item')->resetCache(array($line_item->line_item_id));

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $order_wrapper->uid = $account->uid;
  $order_wrapper->mail = $account->mail;
  $order_wrapper->status = 'checkout_checkout';
  $order_wrapper->created = REQUEST_TIME;
  $order_wrapper->changed = REQUEST_TIME;
  $order_wrapper->commerce_line_items = array($line_item_wrapper->line_item_id->value());

  commerce_order_calculate_total($order);
  $order_wrapper->save();

  // $line_item_wrapper->order_id = $order_wrapper->order_id->value();
  // $line_item_wrapper->save();

  $form_state['redirect'] = 'checkout/' . $order_wrapper->order_id->value();


  // dpm($line_item);
  // dpm($order);
  // dpm($form_state);
  // $result = maritpal_account_balance_transaction_new($account, $type, $amount_decimal, $currency);

  // dpm($result);
}

/*  
 * implements hook_commerce_cart_line_item_refresh()
 *  
 */
// function maritpal_account_balance_commerce_cart_line_item_refresh($line_item, $order_wrapper){

//   dpm($line_item);

//   $line_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

//   $new_price = 100; //I use a function to calculate the value of $new_price

//   if(!empty($new_price)){
//     $line_wrapper->commerce_unit_price->amount->set($new_price);
//     $line_wrapper->save();
//   }
// }