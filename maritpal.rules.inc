<?php
/**
 * @file
 * Defines the actions of adding/deducting money to/from ones account.
 */

/**
 * Implements hook_rules_action_info().
 */
function maritpal_rules_action_info() {
 
}

/**
 * implementation of hook_rules_condition_info()
 */
function maritpal_rules_condition_info() {
  return array(
    'maritpal_privatemsg_thread_is_new_condition' => array(
      'label' => t('Privatemsg thread is new'),
      'arguments' => array(
        'privatemsg_message' => array(
          'type' => 'privatemsg_message', 
          'label' => t('Private message'),
          'optional' => FALSE,
          'allow null' => FALSE,
        ),
        'account' => array(
          'type' => 'user', 
          'label' => t('User'),
          'optional' => FALSE,
          'allow null' => FALSE,
        ),
      ),
      'group' => t('Private messages'),
    ),
  );
}

/**
 * Rules condition callback 
 *  Check if privatemsg thread is new
 *
 * @param obj $privatemsg_message
 *  Privatemsg message object
 * @param obj $account
 *  User who check account
 */
function maritpal_privatemsg_thread_is_new_condition($privatemsg_message, $account) {
  return maritpal_privatemsg_thread_is_new($privatemsg_message, $account);
}