<?php

/**
 * @file
 * Checkout pane callback functions for the cart module.
 */

/**
 * Returns a summary text (as a form) for the checkout panes.
 */
function maritpal_account_balance_checkout_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();
  $pane_form['balance_deposit_summary'] = array(
    '#markup' => _maritpal_account_balance_checkout_summary_text($order),
  );
  return $pane_form;
}

/**
 * Returns a summary text for the checkout review pane.
 */
function maritpal_account_balance_checkout_pane_review($form, $form_state, $checkout_pane, $order) {
  return _maritpal_account_balance_checkout_summary_text($order);
}

/**
 * Callback for summary text on the checkout and review pages.
 */
function _maritpal_account_balance_checkout_summary_text($order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $currency = commerce_currency_load();
  // Get raw numbers.
  $deposit_raw = $order_wrapper->commerce_order_total->amount->value();
  $account = user_load($order_wrapper->uid->raw());
  $balance_current_raw = _maritpal_account_balance_get_balance_raw($account);
  $balance_new_raw = $balance_current_raw + $deposit_raw;
  // Get formatted numbers.
  $balance_current = commerce_currency_format($balance_current_raw, $currency['code']);
  $deposit = commerce_currency_format($deposit_raw, $currency['code']);
  $balance_new = commerce_currency_format($balance_new_raw, $currency['code']);

  return t('Your current balance is @balance_current. After a deposit of
    @deposit your balance will be @balance_new.', array(
      '@balance_current' => $balance_current,
      '@deposit' => $deposit,
      '@balance_new' => $balance_new,
    ));
}