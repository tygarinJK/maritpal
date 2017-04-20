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

/**
 * Page callback for the  'deposit' page.
 */
function maritpal_account_balance_deposit_form_wrapper($account = NULL) {

  if (!$account) {
    global $user;
    $account = $user;
  }

  module_load_include('inc', 'maritpal_account_balance', 'includes/maritpal_account_balance.forms');

  $form = drupal_get_form('maritpal_account_balance_deposit_form', $account);
  return $form;
}

/**
 * Modal callback for confirming purchase and executing rule.
 */
function maritpal_account_balance_purchase_confirmation($js, $field_name, $delta, $entity_type, $bundle_name, $entity_id) {

  if (!$js) {
    return t("This module doesn't work without javascript.");
  }
  else {
    // Check if this is an exisiting instance.
    $field_info = field_info_field($field_name);
    $cardinality = $field_info['cardinality'];
    $instance = field_info_instance($entity_type, $field_name, $bundle_name);
    $entity = entity_load_single($entity_type, $entity_id);
    $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
    if ($cardinality == 1) {
      $field_data = $entity_wrapper->{$field_name}->value();
    }
    else {
      $field_data = $entity_wrapper->{$field_name}[$delta]->value();
    }
    $rules_config = entity_load_single('rules_config', $field_data['rules_config']);
    $amount = $field_data['amount'];
    $label = entity_label($entity_type, $entity);
    global $user;

    if (!$instance || $entity == '' || $rules_config == '') {
      return drupal_not_found();
    }

    ctools_include('modal');
    ctools_include('ajax');
    ctools_add_js('ajax-responder');

    if ($user->uid == 0) {

      $form_state = array(
        'title' => t('Login'),
        'ajax' => TRUE,
      );
      drupal_set_message(t('You need to login before you can purchase with
        your account.'), 'status', FALSE);
      $commands = ctools_modal_form_wrapper('user_login', $form_state);
      if (!empty($form_state['executed'])) {
        // We need to unset the messages because other wise the previous
        // messages will be repeated on the main screen.
        unset($_SESSION['messages']);
        $commands[] = ctools_ajax_command_reload();
      }
    }
    else {
      // Check if the user has enough balance.
      // If not, then give the option to deposit.
      $balance = _maritpal_account_balance_get_balance_raw($user);
      $form_state = array(
        'title' => t('Confirm purchase'),
        'ajax' => TRUE,
        'build_info' => array(
          'args' => array($amount, $balance, $label, $entity, $rules_config),
        ),
      );

      form_load_include($form_state, 'inc', 'maritpal_account_balance', 'includes/maritpal_account_balance.forms');

      if ($balance >= $amount) {
        $commands = ctools_modal_form_wrapper('maritpal_account_balance_purchase_item_form', $form_state);
        if ($form_state['executed'] == TRUE) {
          $commands[] = ctools_ajax_command_reload();
        }
        elseif ($form_state['executed'] == FALSE && isset($form_state['triggering_element'])) {
          if ($form_state['triggering_element']['#name'] == "cancel") {
            $commands[] = ctools_modal_command_dismiss();
          }
        }
      }
      else {
        $commands = ctools_modal_form_wrapper('maritpal_account_balance_insufficient_balance_form', $form_state);
        if ($form_state['executed'] == TRUE) {
          $commands[] = ctools_ajax_command_redirect('balance');
        }
        elseif ($form_state['executed'] == FALSE && isset($form_state['triggering_element'])) {
          if ($form_state['triggering_element']['#name'] == "cancel") {
            $commands[] = ctools_modal_command_dismiss();
          }
        }
      }
    }
    print ajax_render($commands);
  }
}