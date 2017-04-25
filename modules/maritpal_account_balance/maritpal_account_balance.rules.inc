<?php
/**
 * @file
 * Defines the actions of adding and perchasing money to ones account.
 */

/**
 * Implements hook_rules_action_info().
 */
function maritpal_account_balance_rules_action_info() {
  // $currency = commerce_currency_load();

  return array(
    'maritpal_account_balance_load_account_balance' => array(
      'label' => t("Load account balance"),
      'parameter' => array(
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
          'optional' => FALSE,
          'allow null' => FALSE,
        ),
      ),
      'provides' => array(
        'balance' => array(
          'label' => t('Account Balance'),
          'type' => '*',
        ),
      ),
      'group' => t('Commerce Account Balance'),
    ),
    'maritpal_account_balance_purchase_service_request_fee' => array(
      'label' => t('Perchase Service Request fee from sevice provider'),
      'parameter' => array(
        'user' => array(
          'label' => t('Service provider'),
          'type' => 'user',
          'optional' => FALSE,
          'allow null' => FALSE,
        ),
        'node' => array(
          'label' => t('Service request node'),
          'type' => 'node',
          'optional' => FALSE,
          'allow null' => FALSE,
        ),
      ),
      'group' => t('Commerce Account Balance'),
    ),
  );
}

/**
 * implementation of hook_rules_condition_info()
 */
function maritpal_account_balance_rules_condition_info() {
  return array(
    'maritpal_account_balance_request_approve_permission_condition' => array(
      'label' => t('Check balance amount for applying or bidding'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('Service provider to check')),
        'request' => array('type' => 'node', 'label' => t('Service request node to check')),
      ),
      'module' => 'maritpal_account_balance',
    ),
  );
}

/**
 * Return user account balance
 */
function maritpal_account_balance_load_account_balance($account) {
  return _commerce_account_balance_get_balance_raw($account);
}

/**
 * Purchase service requst fee from service provider
 *
 * @param obj $account
 *  Service provider account
 * @param obj $node
 *  Service request node
 */
function maritpal_account_balance_purchase_service_request_fee($account, $node) {
  $fee_amount = maritpal_account_balance_calculate_service_request_fee($account, $node);
  $acc_balance = _commerce_account_balance_get_balance_raw($account);
  $currency_code = $acc_balance['currency_code'];
  $action = t('Purchase @amount service request fee. Your current balance @current', 
    array(
      '@amount' => $fee_amount .' '. $currency_code,
      '@current' => (floatval($acc_balance['amount']) - floatval($fee_amount)) .' '. $currency_code,
    )
  );

  $result = _commerce_account_balance_transaction_new($account, 'purchase', -$fee_amount, $currency_code, $action);
  drupal_set_message($action, 'status', TRUE);
}

/**
 * Condition request_approve_permission
 *  Check if Service Provider has enought account balance amount
 *  to bid or apply on specific service request
 *
 * @param obj $account
 *  User account to check
 * @param obj $node
 *  Service request node 
 */
function maritpal_account_balance_request_approve_permission_condition($account, $node) {
  return maritpal_order_request_acc_balance_access($account->uid, $node) ? TRUE : FALSE;
}