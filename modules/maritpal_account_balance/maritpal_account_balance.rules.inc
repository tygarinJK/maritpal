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

  // return array(
  //   'commerce_account_balance_deposit_to_account' => array(
  //     'label' => t("Deposit money to a user's account"),
  //     'parameter' => array(
  //       'user' => array(
  //         'label' => t('User'),
  //         'type' => 'user',
  //         'optional' => FALSE,
  //         'allow null' => FALSE,
  //       ),
  //       'amount' => array(
  //         'label' => t('Amount in !currency', array('!currency' => $currency['symbol'])),
  //         'type' => 'decimal',
  //         'allow null' => TRUE,
  //         'optional' => TRUE,
  //         'description' => 'A decimal amount of money to add to the balance',
  //       ),
  //       'order_id' => array(
  //         'label' => t('Order ID'),
  //         'type' => 'integer',
  //         'allow null' => TRUE,
  //         'optional' => TRUE,
  //         'description' => 'The order ID this transaction belongs to (optional)',
  //       ),
  //     ),
  //     'group' => t('Commerce Account Balance'),
  //   ),
  // );
}