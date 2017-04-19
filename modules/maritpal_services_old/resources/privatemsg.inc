<?php

/**
 * Create ticket.
 *
 * @param obj $data
 *   Privatemsg subject.
 *
 * @return
 *   A valid thread_id.
 */
function _privatemsg_ticket($data) {

	foreach ($data as $key => $value) {
		if (!isset($key) || empty($value)) {
			return services_error(t('Field @field is required.', array('@field' => $key)), 406);
		}
	}

	$role = user_role_load_by_name('moderator');
  $query = 'SELECT ur.uid FROM {users_roles} AS ur WHERE ur.rid = :rid';
  $result = db_query($query, array(':rid' => $role->rid));
  $uids = $result->fetchCol();

	$admins_rec = user_load_multiple($uids);

	$result = privatemsg_new_thread($admins_rec, t('Ticket: @subj', array('@subj' => $data['subject'])), $data['body']);

	if ($result['success']) {
		$thread_id = $result['message']->thread_id;

		$admin = user_load(4);
		privatemsg_filter_add_tags($thread_id, 5, $admin);

		$return = (object)array(
			'thread_id' => $thread_id,
		);

		return $return;
	}
	elseif (!$result['success']) {
		$error = $result['messages']['error'][0];

		services_error(t('Error while creating message: @error.', array('@error' => $error)), 406);
	}
}

/**
 * Send a private message to one or more recipients.
 *
 * @param array $message
 *      A private message in array format, with field "subject", "body",
 *      "recipients" and "thread_id".
 *
 * @return bool
 *   Return TRUE if sending the message was successful.
 */
function __privatemsg_services_send(array $message) {
  $subject    = $message['subject'];
  $body       = $message['body'];
  $recipients = $message['recipients'];
  $thread_id  = $message['thread_id'];
	$image			= $message['field_image'];

  // return var_export($message);

  // Make sure the message author is logged in.
  global $user;
  $account = user_load($user->uid);
  if (!user_is_logged_in()) {
    return services_error(t('Author is not logged in.'), 403);
  }

  // Validate at least 1 recipient has been passed in.
  if (!$recipients && !$thread_id) {
    return services_error(
      t('There are no recipients, please enter a recipient for the message.'),
      400
    );
  }
  elseif (!$thread_id) {
    // No thread ID - we are sending a new message.
    // Convert the recipients string to an array of user objects.
    list($recipients, $invalid) = _privatemsg_parse_userstring($recipients);
    if (!empty($invalid)) {

      // At least one of the recipients could not be found.
      $invalid_usernames = array('@names' => implode(', ', $invalid));
      return services_error(
        t('One or more usernames are invalid: @names', $invalid_usernames),
        400
      );
    }

    $result = privatemsg_new_thread($recipients, $subject, $body, array(
      'author' => $account,
    ));
    if ($result['success']) {
      return TRUE;
    }
    else {
      return services_error(implode("\n", $result['messages']['error']), 400);
    }
  }
  else {
    // There is a thread id so we are sending a reply.
    $result = privatemsg_reply($thread_id, $body, array('author' => $account));
    if ($result['success']) {
      if(!empty($image['fid']))
			  return maritpal_services_old_privatemsg_image_attach($result['message']->mid, $image['fid'], $image['width'], $image['height']);
      else return TRUE;
    }
    elseif (!empty($result[0])) {
      // If $result[0] this means the thread could not be loaded.
      return services_error($result[0], 404);
    }
    else {
      // Else there was some other problem.
      return services_error(implode("\n", $result['messages']['error']), 400);
    }
  }
}

/**
 * Mark thread messages as read.
 *
 * @param int $thread_id
 *   Privatemsg thread id.
 * @param obj $data
 *   Privatemsg data.
 *
 * @return boolean
 */
function _privatemsg_mark_as_read($thread_id, $data) {
	foreach ($data as $key => $value) {
		if (!isset($key) || empty($value)) {
			return services_error(t('Field @field is required.', array('@field' => $key)), 406);
		}
	}

  return maritpal_services_old_privatemsg_mark_as_read($thread_id, $data['uid']);
}