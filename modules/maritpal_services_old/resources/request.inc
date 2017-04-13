<?php
/**
 * Callback for Creating Service Requests.
 *
 * @param obj $request
 * 	Service request data
 */
function __service_request_node_create($request) {
	$request = _services_arg_value($request, 'request');

  $node = (object) NULL;
  
  foreach ($request as $key => $value) {
    $node->$key = $value;
  }

  if (isset($node->field_order_type['und']) && $node->field_order_type['und'][0]['value'] == 2) $node->comment = 2;

  node_save($node);

  // -- create responce
  if (isset($node->nid)) {
    $responce = array(
      'nid' => $node->nid,
      'path' => $node->path,
    );
  }
  else return services_error('Invalid data. Request was not created.', 404);
  
  return $responce;
}

/**
 * Callback for Updating Service Requests.
 *
 * @param int $nid
 * 	Service Requewst Node ID
 * @param obj $request
 * 	Service request data
 */
function __service_request_node_update($nid, $request) {
	$request = _services_arg_value($request, 'request');

  $old_node = node_load($nid);
  if (empty($old_node->nid)) {
    return services_error(t('Node @nid not found', array('@nid' => $old_node->nid)), 404);
  }

  $node = node_load($nid);

  unset($request['nid']);

  foreach ($request as $key => $value) {
    // $node->$key = $value;
    if (isset($node->$key)) $node->$key = $request[$key];
  }

  node_save($node);

  // -- create responce
  return array(
    'nid' => $node->nid,
    'path' => $node->path,
  );
}

/**
 * Callback for Requesting Service Price.
 *
 * @param obj $data
 * 	Incomming data
 *
 * @return obj
 *	Price data object
 */
function __service_request_price_calculate($data) {
	$data = _services_arg_value($data, 'data');

	if (!isset($data['location']) || !isset($data['category']) || !isset($data['date'])) return services_error('Invalid format data', 404);

  $location = $data['location'];
  $category = $data['category'];
  $date = $data['date'];
  $mail = $data['mail'];

  $price = maritpal_services_old_calculate_request_service_price($location, $category, $date, $mail);
  return !$price ? (object)array('data' => (object)array()) : $price;
}