<?php
/**
 * @file
 * Page callbacks.
 */

/**
 * Review Account balance and balance transactions
 *
 * @param obj $user
 *  User account object
 */
function maritpal_account_balance_review($user) {
  
  return 'TODO: output account balance and transactions table';
}

/**
 * Refill Account balance form
 *
 * @param obj $account
 *  User account object
 */
function maritpal_account_balance_refill_form($form, &$form_state, $account = NULL) {

  if (is_null($account)) {
    if (arg(0) == 'user' && is_numeric(arg(1))) $account = user_load(arg(1));
    else {
      global $user;
      $account = user_load($user->uid);
    }
  }

  $form_state['storage']['user'] = $account;

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

  return $form;
}

function maritpal_account_balance_refill_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['balance_amount']) || $form_state['values']['balance_amount'] <= 0)
    form_set_error('balance_amount', t('Amount must be positive number'));
}

function maritpal_account_balance_refill_form_submit($form, &$form_state) {
  $amount = isset($form_state['values']['balance_amount']) ? $form_state['values']['balance_amount'] : 0;
  $cc = isset($form_state['values']['balance_currency']) ? $form_state['values']['balance_currency'] : commerce_default_currency();
  $account = $form_state['storage']['user'];

  rules_invoke_component('rules_acc_balance_deposit_user_account_balance', $account, $amount, $cc);
}

/**
 * Account balance deposit page
 *
 * @see maritpal_account_balance_menu_alter()
 */
function maritpal_account_balance_deposit_page() {
  return '';
}

/**
 * Account balance deposit cash form
 */
function maritpal_account_balance_deposit_cash($form, &$form_state, $account) {
	$line_item = commerce_line_item_new('deposit');
  $form_state['line_item'] = $line_item;
  $form_state['order'] = commerce_order_new();
  $form_state['account'] = $account;

  field_attach_form('commerce_line_item', $line_item, $form, $form_state);

	$form['balance_amount'] = array(
		'#type' => 'textfield',
		'#title' => t('Balance'),
		'#requred' => TRUE,
		'#weight' => -50
	);
	
	$form['balance_currency'] = array(
		'#type' => 'select',
		'#description' => t('Please select currency of Your location'),
		'#options' => _get_enabled_currencies_options(),
		'#requred' => TRUE,
		'#weight' => -49
	);

  // // The standard commerce unit price widget will have to be hidden.
  // $form['commerce_unit_price'][LANGUAGE_NONE][0]['amount']['#type'] = 'hidden';
  // $form['commerce_unit_price'][LANGUAGE_NONE][0]['amount']['#value'] = 0;

	$form['actions'] = array(
		'#type' => 'container',
	);
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Deposit cash')
	);

	return $form;
}

function maritpal_account_balance_deposit_cash_submit($form, &$form_state) {
	dpm($form_state);
}