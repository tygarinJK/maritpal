<?php

/**
 * @file
 * Admin Page callbacks.
 */

/**
 * Administration form
 */
function maritpal_account_balance_admin_settings_form() {
  
  return 'TODO: output account balance administration form';
}

/**
 * Transaction form
 */
function maritpal_account_balance_admin_transaction_form($form, &$form_state) {
  $form['account_email'] = array(
    '#type' => 'textfield',
    '#title' => t('User E-mail'),
    '#required' => TRUE,
    '#autocomplete_path' => 'admin/maritpal/settings/balance/autocomplete-user',
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Transaction type'),
    '#options' => _commerce_account_balance_transaction_type_list(),
    '#required' => TRUE,
  );

  $form['decimal_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#description' => t('Positive or negative (decimal) amount'),
    '#required' => TRUE,
    '#size' => 60,
    '#max_length' => 60,
  );
  $form['currency_code'] = array(
    '#type' => 'select',
    '#options' => _get_enabled_currencies_options(),
    '#required' => TRUE,
  );

  $form['action'] = array(
    '#type' => 'textarea',
    '#title' => t('Action'),
    '#description' => t('Transaction description (will be displayed to user)'),
  );
  
  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => '<a href="/admin/maritpal/settings/balance">'. t('Cancel') .'</a>',
    '#prefix' => t('or'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'maritpal_account_balance') .'/css/maritpal_account_balance.css';
  $form['#validate'] = array('maritpal_account_balance_admin_transaction_form_validate');
  $form['#submit'] = array('maritpal_account_balance_admin_transaction_form_submit');

  return $form;
}

function maritpal_account_balance_admin_transaction_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['account_email']))
    form_set_error('account_email', t('Invalid Email format'));

  $account = user_load_by_mail($form_state['values']['account_email']);
  if (!isset($account->uid) || $account->uid <= 0)
    form_set_error('account_email', t('Such User doesn`t exist'));

  if (!is_numeric($form_state['values']['decimal_amount']))
    form_set_error('decimal_amount', t('Invalid Amount'));
}

function maritpal_account_balance_admin_transaction_form_submit($form, &$form_state) {
  $account = user_load_by_mail($form_state['values']['account_email']);
  $type = $form_state['values']['type'];
  $amount_decimal = $form_state['values']['decimal_amount'];
  $currency_code = $form_state['values']['currency_code'];
  $action = !empty($form_state['values']['action']) ? $form_state['values']['action'] : NULL;

  $return = _commerce_account_balance_transaction_new($account, $type, $amount_decimal, $currency_code, $action);
  $form_state['redirect'] = 'admin/maritpal/settings/balance';
  drupal_set_message(t('Transaction was successfully created'), 'status', TRUE);
}