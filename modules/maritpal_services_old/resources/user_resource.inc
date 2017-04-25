<?php

/**
 * Login a user using the specified credentials.
 *
 * Note this will transfer a plaintext password.
 *
 * @param $username
 *   Username to be logged in.
 * @param $password
 *   Password, must be plain text and not hashed.
 *
 * @return
 *   A valid session object.
 */
function __user_resource_login($username, $password) {
  global $user;

  if ($user->uid) {
    // user is already logged in
    return services_error(t('Already logged in as @user.', array('@user' => $user->name)), 406);
  }

  // Check if account is active.
  if (user_is_blocked($username)) {
    return services_error(t('The username %name has not been activated or is blocked.', array('%name' => $username)), 403);
  }

  // Emulate drupal native flood control: check for flood condition.
  $flood_state = array();
  if (variable_get('services_flood_control_enabled', TRUE)) {
    $flood_state = _user_resource_flood_control_precheck($username);
  }

  // Only authenticate if a flood condition was not detected.
  if (empty($flood_state['flood_control_triggered'])) {
    $uid = user_authenticate($username, $password);
  }
  else {
    $uid = FALSE;
  }

  // Emulate drupal native flood control: register flood event, and throw error
  // if a flood condition was previously detected
  if (variable_get('services_flood_control_enabled', TRUE)) {
    $flood_state['uid'] = $uid;
    _user_resource_flood_control_postcheck($flood_state);
  }

  if ($uid) {
    $user = user_load($uid);
    if ($user->uid) {
      user_login_finalize();

      $return = new stdClass();
      $return->sessid = session_id();
      $return->session_name = session_name();
      $return->token = drupal_get_token('services');

      $account = clone $user;

      // Empty arrays to empty objects
      foreach ($account as $key => $value) {
        if (is_array($value) && count($value) == 0) $account->$key = (object)array();
      }

      if (module_exists('userpoints'))
        $account->userpoints = userpoints_get_current_points($user->uid);

      if (module_exists('commerce_account_balance')) {
        $balance_raw = _commerce_account_balance_get_balance_raw($user);
        $account->balance = $balance_raw['amount'] .' '. $balance_raw['currency_code'];
      }

      if (module_exists('push_notifications')) {
        $tokens = push_notification_get_user_tokens($user->uid);

        $account->push_notification_tokens = array();
        foreach ($tokens as $k => $v) {
          $account->push_notification_tokens[] = $v;
        }
      }

      services_remove_user_data($account);

      $return->user = $account;

      $mdc_tree = array();

      // -- Categories
      if (isset($user->field_user_serv_categories['und'])) {
        foreach ($user->field_user_serv_categories['und'] as $key => $value) {
          $tid = $value['tid'];
          $parent = taxonomy_get_parents($tid);

          $mdc_tree['field_user_serv_categories'][$key] = (object)array(
            'tid' => $tid
          );

          if ( is_array($parent) && key($parent) != null )
            $mdc_tree['field_user_serv_categories'][$key]->parent = key($parent); 
        }
      }

      // -- Subject
      if (isset($user->field_user_serv_subj['und'])) {
        foreach ($user->field_user_serv_subj['und'] as $key => $value) {
          $tid = $value['tid'];
          $parent = taxonomy_get_parents($tid);

          $mdc_tree['field_user_serv_subj'][$key] = (object)array(
            'tid' => $tid
          );

          if ( is_array($parent) && key($parent) != null )
            $mdc_tree['field_user_serv_subj'][$key]->parent = key($parent); 
        }
      }

      // -- Object
      if ( isset($user->field_user_serv_obj['und']) ) {
        foreach ($user->field_user_serv_obj['und'] as $key => $value) {
          $tid = $value['tid'];
          $parent = taxonomy_get_parents($tid);

          $mdc_tree['field_user_serv_obj'][$key] = (object)array(
            'tid' => $tid
          );

          if ( is_array($parent) && key($parent) != null )
            $mdc_tree['field_user_serv_obj'][$key]->parent = key($parent); 
        }
      }

      // -- Location
      if ( isset($user->field_serv_loc_city['und']) ) {
        foreach ($user->field_serv_loc_city['und'] as $key => $value) {
          $tid = $value['tid'];
          $parent = taxonomy_get_parents($tid);

          $mdc_tree['field_serv_loc_city'][$key] = (object)array(
            'tid' => $tid
          );

          if ( is_array($parent) && key($parent) != null )
            $mdc_tree['field_serv_loc_city'][$key]->parent = key($parent); 
        }
      }

      $return->mdc_tree = $mdc_tree;

      return $return;
    }
  }
  watchdog('user', 'Invalid login attempt for %username.', array('%username' => $username));
  return services_error(t('Wrong username or password.'), 401);
}