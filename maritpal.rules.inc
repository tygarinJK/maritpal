<?php
/**
 * @file
 * Defines the actions of adding/deducting money to/from ones account.
 */

/**
 * Implements hook_rules_action_info().
 */
function maritpal_rules_action_info() {
  $currency = commerce_currency_load();

  return array(
    'maritpal_rules_action_purchase_balance' => array(
      'label' => t('Custom | Perchase money from user`s account'),
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
          'description' => 'A decimal amount of money to perchase from the balance',
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
    // 'maritpal_rules_action_deposit_balance' => array(
    //   'label' => t('Custom | Deposit money to user`s account'),
    //   'parameter' => array(
    //     'user' => array(
    //       'label' => t('User'),
    //       'type' => 'user',
    //       'optional' => FALSE,
    //       'allow null' => FALSE,
    //     ),
    //     'amount' => array(
    //       'label' => t('Amount'),
    //       'type' => 'decimal',
    //       'allow null' => TRUE,
    //       'optional' => TRUE,
    //       'description' => 'A decimal amount of money to deposit to user`s balance',
    //     ),
    //     'amount' => array(
    //       'label' => t('Amount with currency'),
    //       'type' => 'price',
    //       'allow null' => TRUE,
    //       'optional' => TRUE,
    //       // 'description' => 'A decimal amount of money to deposit to user`s balance',
    //     ),
    //     'order_id' => array(
    //       'label' => t('Order ID'),
    //       'type' => 'integer',
    //       'allow null' => TRUE,
    //       'optional' => TRUE,
    //       'description' => 'The order ID this transaction belongs to (optional)',
    //     ),
    //   ),
    //   'group' => t('Commerce Account Balance'),
    // ),
  );
}

/**
 * Action callback for perchasing money from ones account.
 */
function maritpal_rules_action_purchase_balance($user, $amount_decimal, $order_id = NULL) {
  $amount = round($amount_decimal * 100);
  $uid = $user->uid;

  $currency = commerce_currency_load();
  $formatted_amount = commerce_currency_format($amount_decimal, $currency['code']);

  $result = db_insert('commerce_account_balance_transaction')
    ->fields(array(
      'type' => 'perchase',
      'action' => t('Perchase @amount from your account', array('@amount' => $formatted_amount)),
      'uid' => $uid,
      'time' => REQUEST_TIME,
      'amount' => $amount_decimal,
    ))
    ->execute();

  return is_numeric($result) ? $result : FALSE;
}

/**
 * Action callback for depositing money to ones account.
 */
// function maritpal_rules_action_deposit_balance($user, $amount_decimal, $order_id = NULL) {
//   $amount = round($amount_decimal * 100);
//   $uid = $user->uid;

//   $currency = commerce_currency_load();
//   $formatted_amount = commerce_currency_format($amount_decimal, $currency['code']);

//   $result = db_insert('commerce_account_balance_transaction')
//     ->fields(array(
//       'type' => 'deposit',
//       'action' => t('Deposit @amount to your account', array('@amount' => $formatted_amount)),
//       'uid' => $uid,
//       'time' => REQUEST_TIME,
//       'amount' => $amount_decimal,
//     ))
//     ->execute();

//   return is_numeric($result) ? $result : FALSE;
// }