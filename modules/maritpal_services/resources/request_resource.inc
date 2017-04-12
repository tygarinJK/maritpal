<?php

/**
 * Callback for retrieving Order Request resources.
 *
 * @param int $id
 * @return object
 */
function _request_resource_retrieve($nid) {
  $node = node_load($nid);

  // if ($node) {
  //   $uri = entity_uri('node', $node);
  //   $node->path = url($uri['path'], array('absolute' => TRUE));
  //   // Unset uri as it has complete entity and this
  //   // cause never ending recursion in rendering.
  //   unset($node->uri);
  // }
  //Lets check field_permissions
  // $node = services_field_permissions_clean('view', 'node', $node);
  return $node;
}

/**
 * Callback for creating Order Request resources.
 *
 * @param object $data
 * @return object
 */
function _request_resource_create($data) {
  global $user;

  $json = drupal_json_decode($data, TRUE);

  if (isset($json)) {

    $node = (object) NULL;
    
    foreach ($json as $key => $value) {
      $node->$key = $value;
    }

  	$node = node_save($node);

    // if (isset($node)) {
  	 //  return $node;
    // }
  }
  else return services_error('Missing some data', 406);
}

/**
 * Callback for updating Order Request resources.
 *
 * @param int $nid
 * @param object $data
 * @return object
 */
function _request_resource_update($nid, $data) {
  global $user;

  $node = node_load($nid);
  if (empty($node->nid)) {
    return services_error(t('Node @nid not found', array('@nid' => $node->nid)), 404);
  }

  $json = drupal_json_decode($data, TRUE);

  if (isset($json)) {

    foreach ($json as $key => $value) {
      $node->$key = $value;
    }

    $node = node_save($node);
  }
  else return services_error('Missing some data', 406);
  
  return (object)array(
    'nid' => $node->nid,
    'path' => $node->path,
  );
}

/**
 * Callback for deleting Order Request resources.
 *
 * @param int $nid
 * @return object
 */
function _request_resource_delete($nid) {
  node_delete($nid)

  return (object)array(
    'nid' => $nid,
  );
}