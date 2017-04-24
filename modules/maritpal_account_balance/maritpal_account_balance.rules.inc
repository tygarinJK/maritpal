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
  );
}

/**
 * Return user account balance
 */
function maritpal_account_balance_load_account_balance($account) {
  return _commerce_account_balance_get_balance_raw($account);
}