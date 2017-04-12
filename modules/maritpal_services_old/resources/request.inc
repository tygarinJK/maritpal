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

  $locs = $data['location'];
  $cats = $data['category'];

  $locs_revert = array_reverse($locs);

  // -- fetch price rules terms
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'taxonomy_term')
    ->propertyCondition('vid', 23)
    ->fieldCondition('field_serv_cat', 'tid', $cats, '=')
    ->fieldCondition('field_serv_loaction', 'tid', $locs_revert, 'CONTAINS')
    ->addMetaData('account', user_load(4));

  $result = $query->execute();

  if (isset($result['taxonomy_term'])) {
    $tids = array_keys($result['taxonomy_term']);

    foreach ($locs_revert as $l => $loc) {
      foreach ($tids as $t => $tid) {
        $temp_term = taxonomy_term_load($tid);
        $tloc = $temp_term->field_serv_loaction['und'][0]['tid'];

        if ($loc == $tloc) {
          $term = $temp_term;
          break 2;
        }
      }
    }
  }

  if (!$term) $term = taxonomy_term_load(1988);

  // return $term;

  // -- price parameters
  $params = array(
    'date_val' => strtotime($data['date']['value']),
    'date_val2' => strtotime($data['date']['value2']),
    'rate' => intval($term->field_serv_rate['und'][0]['value']),
    'price_hp' => array(
      'amount' => intval($term->field_serv_price_ph['und'][0]['amount']),
      'currency_code' => $term->field_serv_price_ph['und'][0]['currency_code'],
    )
  );
  $params['duration'] = ($params['date_val2'] - $params['date_val']) / 3600;

  $price = array(
    'amount' => round($params['duration'] * ($params['rate'] * $params['price_hp']['amount']), 0, PHP_ROUND_HALF_UP),
    'currency_code' => $params['price_hp']['currency_code'],
    'taxes' => array(),
    'discount' => 0,
  );

  // -- Admin rate/Transsfer rate/Bank fee
  // admin fixed
  $adm_type = variable_get_value('pricing:admin_rate:type');
  $adm_val = variable_get_value('pricing:admin_rate:value');

  if ($adm_type == 0) $price['taxes']['admin'] = $adm_val;
  else $price['taxes']['admin'] = round(($price['amount'] / 100) * $adm_val, 0);

  // transfer fixed
  $trns_type = variable_get_value('pricing:transfer_rate:type');
  $trns_val = variable_get_value('pricing:transfer_rate:value');

  if ($trns_type == 0) $price['taxes']['transfer'] = $trns_val;
  else $price['taxes']['transfer'] = round(($price['amount'] / 100) * $trns_val, 0);

  // bank fee
  $bank_fee = variable_get_value('pricing:bank_fee:value');

  if ($bank_fee) $price['taxes']['bank_fee'] = round(($price['amount'] / 100) * $bank_fee, 0);

  // -- create responce
  if (!empty($price['amount'])) {
    $responce = (object)array(
      'data' => (object)array(
        'rule' => $term->tid,
        'total' => $price['amount'] + $price['taxes']['admin'] + $price['taxes']['transfer'] + $price['taxes']['bank_fee'] + $price['discount'],
        'currency_code' => $price['currency_code'],
        'components' => (object)array(
          'base_price' => intval($price['amount']),
          'admin_tax' => intval($price['taxes']['admin']),
          'transfer_tax' => intval($price['taxes']['transfer']),
          'bank_fee' => intval($price['taxes']['bank_fee']),
          'discount' => intval($price['discount']),
        ), 
      ),
    );
  }
  else return services_error('Ups! Something goes wrong.', 406);
  
  return $responce;
}