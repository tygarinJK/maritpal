<?php

/**
 * @file
 *  Link general system functionalities to services module.
 */

 /**
 * Returns the details of currently logged in user.
 *
 * @return
 *   object with session id, session name and a user object.
 */
function __system_resource_connect() {
  global $user;
  services_remove_user_data($user);

  $return = new stdClass();
  $return->sessid = session_id();
  $return->session_name = session_name();
  $return->user = $user;

  if ($user->uid > 0) {
    $account = user_load($user->uid);
    $return->mdc = maritpal_services_old_get_mdc_tree($account);
  }

  return $return;
}
