<?php

/**
 * @file
 * User menu callbacks for Maritpal.
 */

/**
 * Render display Price Rules table
 */
function maritpal_pricing_rules_review($form, &$form_state) {
	// Path query
	$q_params = drupal_get_query_parameters();
	dpm($q_params);

	$form = maritpal_reports_filters_form($form, $form_state);
	$form['filters']['service_location'] = $form['filters']['mdc']['service_location'];
	$form['filters']['service_category'] = $form['filters']['mdc']['service_category'];

	unset(
		$form['filters']['period'],
		$form['filters']['mdc'],
		$form['filters']['mdc']['service_location'],
		$form['filters']['mdc']['service_category']
	);

	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term')
  ->propertyCondition('vid', 23, '=')
	->propertyOrderBy('name', $direction = 'ASC');

	if (!empty($q_params['service_location'])) {
		$chids = array($q_params['service_location']);

		$tree = taxonomy_get_tree(14, $q_params['service_location']);
		foreach ($tree as $i => $term) {
			$chids[] = $term->tid;
		}
		$query->fieldCondition('field_serv_loaction', 'tid', $chids);
	}
	if (!empty($q_params['service_category'])) {
		$chids = array($q_params['service_category']);

		$tree = taxonomy_get_tree(13, $q_params['service_category']);
		foreach ($tree as $i => $term) {
			$chids[] = $term->tid;
		}
		$query->fieldCondition('field_serv_cat', 'tid', $chids);
	}

	$result = $query->execute();
  
	$rows = array();
  if (isset($result['taxonomy_term'])) {
  	$terms = entity_load('taxonomy_term', array_keys($result['taxonomy_term']));

  	foreach ($terms as $term) {
  		$location = taxonomy_term_load($term->field_serv_loaction['und'][0]['tid']);
  		$location = isset($location->name) ? $location->name : '';

  		$category = taxonomy_term_load($term->field_serv_cat['und'][0]['tid']);
  		$category = isset($category->name) ? $category->name : '';

  		$rate_amount = isset($term->field_serv_rate_amount['und']) ? $term->field_serv_rate_amount['und'][0]['value'] : ''; 
			$rate_currency = isset($term->field_serv_rate_currency['und']) ? $term->field_serv_rate_currency['und'][0]['currency_code'] : '';

  		$rows[] = array(
				'tid' => $term->tid,
  			'name' => $term->name, 
				'location' => $location, 
				'category' => $category,
				'sp_mail' => isset($term->field_serv_sp_mail['und']) ? $term->field_serv_sp_mail['und'][0]['email'] : '',
				'serv_rate' => $rate_amount .' '. $rate_currency,
				'serv_duration' => isset($term->field_serv_rate_dur['und']) ? $term->field_serv_rate_dur['und'][0]['value'] : '',
				'operations' => 
					'<a href="/admin/maritpal/settings/pricing-rules/'. $term->tid .'/edit">'. t('Edit') .'</a> | '.
					'<a href="/admin/maritpal/settings/pricing-rules/'. $term->tid .'/delete">'. t('Delete') .'</a>',
  		);
  	}
  }

	$per_page = 20;
	// Initialize the pager
	$current_page = pager_default_initialize(count($rows), $per_page);
	// Split list into page sized chunks
	$chunks = array_chunk($rows, $per_page, TRUE);

	$output = theme_table(array(
		'header' => array(
			'tid' => '#',
			'name' => t('Name'), 
			'location' => t('Service Location'), 
			'category' => t('Service Category'),
			'sp_mail' => t('Service Provider Mail'),
			'serv_rate' => t('Service Rate'),
			'serv_duration' => t('Service Duration (per)'),
			'operations' => t('Operations'),
		),
		'rows' => $chunks[$current_page],
		'attributes' => array(),
		'caption' => t(''),
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => t('No items found')
	));

	// Show the pager
	$output .= theme('pager', array('quantity',count($rows)));

	$form['result_table'] = array(
		'#markup' => $output,
	);

	$form['#attached']['css'][] = drupal_get_path('module', 'maritpal') .'/css/maritpal.css';
  $form['#validate'][] = 'maritpal_pricing_rules_review_validate';
  $form['#submit'][] = 'maritpal_pricing_rules_review_submit';

	return $form;
}

function maritpal_pricing_rules_review_validate(&$form, &$form_state) {
}

function maritpal_pricing_rules_review_submit(&$form, &$form_state) {
	$q_params = array();
	if (!empty($form_state['values']['service_location'][0])) $q_params['service_location'] = $form_state['values']['service_location'][0];
	if (!empty($form_state['values']['service_category'][0])) $q_params['service_category'] = $form_state['values']['service_category'][0];

	if(count($q_params) > 0)
		$form_state['redirect'] = array(current_path(), array('query' => $q_params));
}

/**
 * Reports info Page
 */
function maritpal_reports_intro_page() {
	// $menu = maritpal_submenu_tree_all_data($title, '');
	return 'TODO: Intro page';
}

/**
 * Report | MDC figures by providers and region
 */
function maritpal_reports_mdc_figures_by_providers_and_region_form($form, &$form_state) {
	// Path query
	$q_params = drupal_get_query_parameters();
	dpm($q_params);

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Filters'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$form['filters']['period'] = array(
		'#type' => 'fieldset',
		'#title' => t('Period'),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE,
	);
	$form['filters']['period']['period_from'] = array(
		'#type' => 'date_popup',
		'#title' => t('From'),
		'#date_timezone' => date_default_timezone(),
		'#date_format' => 'Y-m-d',
		'#date_year_range' => '-10:now',
		'#default_value' => !empty($q_params['period_from']) ? $q_params['period_from'] : NULL,
	);
	$form['filters']['period']['period_to'] = array(
		'#type' => 'date_popup',
		'#title' => t('To'),
		'#date_timezone' => date_default_timezone(),
		'#date_format' => 'Y-m-d',
		'#date_year_range' => '-10:now',
		'#default_value' => !empty($q_params['period_to']) ? $q_params['period_to'] : NULL,
	);

	$form['filters']['mdc'] = array(
		'#type' => 'fieldset',
		'#title' => t('MDC'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$form['filters']['mdc']['service_location'] = array(
		'#type' => 'hierarchical_select',
    '#title' => t('Location'),
    '#size' => 1,
    '#config' => array(
      'module' => 'hs_taxonomy',
      'params' => array(
        'vid' => 14,
        'separator' => '|',
      ),
      'save_lineage'    => 0,
      'enforce_deepest' => 0,
      'resizable'       => 0,
      'level_labels' => array(
        'status' => 1,
        'labels' => array(
          0 => t(' - Select Country - '),
          1 => t(' - Select City - '),
        ),
      ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => t('All selections'), 
        'limit'    => 0,
        'reset_hs' => 1,
        'sort'     => 1,
      ),
      'editability' => array(
        'status'           => 0,
        'item_types'       => array(),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
          2 => 1,
        ),
        'allow_new_levels' => 0,
        'max_levels'       => 3,
      ),
      'entity_count'    => array(
        'enabled' => 0,
        'require_entity' => 0,
        'settings' => array(
          'count_children' => 0,
          'entity_types' => array(),
        ),
      ),
      // These settings cannot be configured through the UI: they can only be
      // overridden through code.
      'animation_delay'    => 400,
      'exclusive_lineages' => array(),
      'render_flat_select' => 0,
    ),
    '#description' => t(''),
    '#default_value' => !empty($q_params['service_location']) ? $q_params['service_location'] : '',
    '#weight' => -49,
	);
	$form['filters']['mdc']['service_category'] = array(
		'#type' => 'hierarchical_select',
    '#title' => t('Category'),
    '#size' => 1,
    '#config' => array(
      'module' => 'hs_taxonomy',
      'params' => array(
        'vid' => 13,
        'separator' => '|',
      ),
      'save_lineage'    => 0,
      'enforce_deepest' => 0,
      'resizable'       => 0,
      'level_labels' => array(
        'status' => 1,
        'labels' => array(
          0 => t(' - MDC 1 - '),
          1 => t(' - MDC 2 - '),
          2 => t(' - MDC 3 - '),
          3 => t(' - MDC 4 - '),
        ),
      ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => t('All selections'),
        'limit'    => 0,
        'reset_hs' => 1,
        'sort'     => 1,
      ),
      'editability' => array(
        'status'           => 0,
        'item_types'       => array(),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
          2 => 1,
        ),
        'allow_new_levels' => 0,
        'max_levels'       => 4,
      ),
      'entity_count'    => array(
        'enabled' => 0,
        'require_entity' => 0,
        'settings' => array(
          'count_children' => 0,
          'entity_types' => array(),
        ),
      ),
      // These settings cannot be configured through the UI: they can only be
      // overridden through code.
      'animation_delay'    => 400,
      'exclusive_lineages' => array(),
      'render_flat_select' => 0,
    ),
    '#description' => t(''),
    '#default_value' => !empty($q_params['service_category']) ? $q_params['service_category'] : '',
	);

	$form['filters']['actions'] = array(
		'#type' => 'container',
	);
	$form['filters']['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Filter'),
	);
	$form['filters']['actions']['cancel'] = array(
		'#type' => 'item',
		'#markup' => '<a href="/'. current_path() .'">'. t('Reset') .'</a>',
	);

	$query = 'SELECT DISTINCT(ur.uid) 
	 					FROM {users_roles} AS ur
	 					WHERE ur.rid IN (:rids)';
	$result = db_query($query, array(':rids' => array(4)));
	$uids = $result->fetchCol();

	$users = user_load_multiple($uids);

	// Deapest MDC
	foreach ($users as $uid => $user) {
		// user categories
		$user_categories = array();
		if (isset($user->field_user_serv_categories['und'])) {
			foreach ($user->field_user_serv_categories['und'] as $category) {
				if (!in_array($category['tid'], $user_categories, TRUE)) $user_categories[$category['tid']] = $category['tid'];
			}
			// deapest categories only
			foreach ($user_categories as $i => $tid) {
				$parent = taxonomy_get_parents($tid);
				if (count($parent) == 0) unset($user_categories[$i]);
				else unset($user_categories[key($parent)]);
			}
		}
		if (count($user_categories) == 0) unset($users[$uid]);
		else $users[$uid]->deapest_service_categories = $user_categories;

		// user locations
		if (isset($user->field_serv_loc_city['und'])) {
			$loacation = end($user->field_serv_loc_city['und']);
			$users[$uid]->deapest_service_location = array(
				$loacation['tid'] => $loacation['tid'],
			);
		}
		else unset($users[$uid]);
	}

	// If filters Applyed
	if (!empty($q_params['service_location']) || !empty($q_params['service_category'])) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'user')
			->addMetaData('account', user_load(4));
		
		if (!empty($q_params['service_location']))
			$query->fieldCondition('field_serv_loc_city', 'tid', array(strval($q_params['service_location'])), 'IN');

		if (!empty($q_params['service_category']))
			$query->fieldCondition('field_user_serv_categories', 'tid', array(strval($q_params['service_category'])), 'IN');

		$result = $query->execute();
		$uids = array_keys($result['user']);
		// dpm($uids);

		foreach ($users as $uid => $user) {
			if (!in_array($uid, $uids)) unset($users[$uid]);
		}

		// Deapest filtered category mdc
		if (!empty($q_params['service_category'])) {
			foreach ($users as $uid => $user) {
				foreach ($user->deapest_service_categories as $tid => $category_tid) {
					$tree = taxonomy_get_parents_all($tid);
					$match = FALSE;
					foreach ($tree as $i => $term) {
						if ($term->tid == $q_params['service_category']) $match = TRUE;
					}
					if (!$match) unset($user->deapest_service_categories[$tid]);
				}
			}
		}

	}

	// Count users for each MDC
	$count_users = array();
	foreach ($users as $uid => $user) {
		foreach ($user->deapest_service_categories as $tid) {
			$count_users[$tid .'_'. key($user->deapest_service_location)][] = $user->uid;
		}
	}
	
	// fill table rows
	$rows = array();
	foreach ($count_users as $key => $values) {
		$mdc = explode('_', $key);
		$categories = maritpal_get_mdc_category_tree($mdc[0]);
		$location = maritpal_get_mdc_location_tree($mdc[1]);


		// get regular service order of mdc 
		$query = new EntityFieldQuery();
		$reg_orders = $query->entityCondition('entity_type', 'node')
			->entityCondition('bundle', 'request')
			->fieldCondition('field_order_type', 'value', 1, '=')
			->fieldCondition('field_serv_loc_city', 'tid', $mdc[1], '=')
			->fieldCondition('field_request_serv_cat', 'tid', $mdc[0], '=')
			->addMetaData('account', user_load(4));

		if (!empty($q_params['period_from']))
			$query->propertyCondition('created', strtotime($q_params['period_from']), '>');
		if (!empty($q_params['period_to']))
			$query->propertyCondition('created', strtotime($q_params['period_to']), '<');
	
		$reg_orders = $query->execute();

		// get bid service order of mdc 
		$query = new EntityFieldQuery();
		$bid_orders = $query->entityCondition('entity_type', 'node')
			->entityCondition('bundle', 'request')
			->fieldCondition('field_order_type', 'value', 2, '=')
			->fieldCondition('field_serv_loc_city', 'tid', $mdc[1], '=')
			->fieldCondition('field_request_serv_cat', 'tid', $mdc[0], '=')
			->addMetaData('account', user_load(4));

		if (!empty($q_params['period_from']))
			$query->propertyCondition('created', strtotime($q_params['period_from']), '>');
		if (!empty($q_params['period_to']))
			$query->propertyCondition('created', strtotime($q_params['period_to']), '<');
	
		$bid_orders = $query->execute();

		$rows[] = array(
			'mdc_0' => $categories['mdc_0'], 
			'mdc_1' => $categories['mdc_1'], 
			'mdc_2' => $categories['mdc_2'],
			'mdc_3' => $categories['mdc_3'],
			'country' => $location['country'],
			'city' => $location['city'],
			'no_prov' => count($values),
			'regular_order' => count($reg_orders['node']),
			'bit_order' => count($bid_orders['node']),
			'ratio_order_prov' => round((count($reg_orders['node']) + count($bid_orders['node'])) / count($values), 2),
		);
	}

	$per_page = 20;
	// Initialize the pager
	$current_page = pager_default_initialize(count($rows), $per_page);
	// Split list into page sized chunks
	$chunks = array_chunk($rows, $per_page, TRUE);

	$output = theme_table(array(
		'header' => array(
			'mdc_0' => t('MDC level 1'), 
			'mdc_1' => t('MDC level 2'), 
			'mdc_2' => t('MDC level 3'),
			'mdc_3' => t('MDC level 4'),
			'country' => t('Country'),
			'city' => t('City'),
			'no_prov' => t('Number of providers'),
			'regular_order' => t('Created order'),
			'bit_order' => t('Created bid order'),
			'ratio_order_prov' => t('Ratio order/provider'),
		),
		'rows' => $chunks[$current_page],
		'attributes' => array(),
		'caption' => t(''),
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => t('No items found'),
	));

	// Show the pager
	$output .= theme('pager', array('quantity',count($rows)));

	$form['result_table'] = array(
		'#markup' => $output,
	);

	$form['#attached']['css'][] = drupal_get_path('module', 'maritpal') .'/css/maritpal.css';
  $form['#validate'][] = 'maritpal_reports_mdc_figures_by_providers_and_region_form_validate';
  $form['#submit'][] = 'maritpal_reports_mdc_figures_by_providers_and_region_form_submit';

	return $form;
}

function maritpal_reports_mdc_figures_by_providers_and_region_form_validate(&$form, &$form_state) {
}

// function maritpal_reports_mdc_figures_by_providers_and_region_form_submit(&$form, &$form_state) {
// 	$q_params = array();
// 	if (!empty($form_state['values']['period_from'])) $q_params['period_from'] = $form_state['values']['period_from'];
// 	if (!empty($form_state['values']['period_to'])) $q_params['period_to'] = $form_state['values']['period_to'];
// 	if (!empty($form_state['values']['service_location'][0])) $q_params['service_location'] = $form_state['values']['service_location'][0];
// 	if (!empty($form_state['values']['service_category'][0])) $q_params['service_category'] = $form_state['values']['service_category'][0];

// 	if(count($q_params) > 0)
// 		$form_state['redirect'] = array(current_path(), array('query' => $q_params));
// }

/**
 * MDC report for orders
 */
function maritpal_reports_mdc_figures_by_orders_form($form, $form_state) {
	// Path query
	$q_params = drupal_get_query_parameters();
	// dpm($q_params);

	$form = maritpal_reports_filters_form();

	$query = new EntityFieldQuery();
	$orders = $query->entityCondition('entity_type', 'node')
		->entityCondition('bundle', 'request')
		->addMetaData('account', user_load(4));

	if (!empty($q_params['service_location']))
		$query->fieldCondition('field_serv_loc_city', 'tid', $q_params['service_location'], 'CONTAINS');
	if (!empty($q_params['service_category'])) {
		$chids = array($q_params['service_category']);

		$tree = taxonomy_get_tree(13, $q_params['service_category']);
		foreach ($tree as $i => $term) {
			$chids[] = $term->tid;
		}
		$query->fieldCondition('field_request_serv_cat', 'tid', $chids, 'IN');
	}

	if (!empty($q_params['period_from']))
		$query->propertyCondition('created', strtotime($q_params['period_from']), '>');
	if (!empty($q_params['period_to']))
		$query->propertyCondition('created', strtotime($q_params['period_to']), '<');

	$orders = $query->execute();

	// fill table rows
	$rows = array();
	if (isset($orders['node'])) {
		$nids = array_keys($orders['node']);
		$nodes = node_load_multiple($nids);

		// dpm($nodes);

		$statistics = array();
		foreach ($nodes as $node) {
			// sort by mdc
			$mdc_key = end($node->field_request_serv_cat['und'])['tid'] .'_'. end($node->field_serv_loc_city['und'])['tid'];

			// sort orders by status
			if (isset($node->field_request_status['und'])) {
				switch ($node->field_request_status['und'][0]['tid']) {
					case '10':
					case '11':
						$statistics[$mdc_key]['opened'][] = $node->nid;
						break;
					case '14':
						$statistics[$mdc_key]['confirmed'][] = $node->nid;
						break;
					case '13':
					case '15':
						$statistics[$mdc_key]['canceled'][] = $node->nid;
						break;
					case '16':
						$statistics[$mdc_key]['closed'][] = $node->nid;
						break;
				}
			}

			// direct orders
			if (isset($node->field_order_type['und']) && $node->field_order_type['und'][0]['value'] == 1) {
				$statistics[$mdc_key]['direct_orders']['orders'][] = $node->nid;

			}
			// bid orders
			elseif (isset($node->field_order_type['und']) && $node->field_order_type['und'][0]['value'] == 2) {
				$statistics[$mdc_key]['bid_orders']['orders'][] = $node->nid;

				// bid order with no proposal
				if ($node->comment_count == 0) $statistics[$mdc_key]['bid_orders']['no_proposal'][] = $node->nid;

				if (isset($node->field_request_status['und']) && $node->field_request_status['und'][0]['tid'] == 12) {
					// bid order which not confirmed
					$statistics[$mdc_key]['bid_orders']['approved'][] = $node->nid;
				}
			}
		}
	}

	// dpm($statistics);

	foreach ($statistics as $key => $value) {
		$mdc = explode('_', $key);
		if (empty($mdc[0]) || empty($mdc[1])) continue;

		$categories = maritpal_get_mdc_category_tree($mdc[0]);
		$location = maritpal_get_mdc_location_tree($mdc[1]);

		$rows[] = array(
			'mdc_0' => $categories['mdc_0'], 
			'mdc_1' => $categories['mdc_1'], 
			'mdc_2' => $categories['mdc_2'],
			'mdc_3' => $categories['mdc_3'],
			'country' => $location['country'],
			'city' => $location['city'],
			'direct_created' => isset($value['direct_orders']['orders']) ? count($value['direct_orders']['orders']) : 0,
			'confirmed' => isset($value['confirmed']) ? count($value['confirmed']) : 0,
			'opened' => isset($value['opened']) ? count($value['opened']) : 0,
			'canceled' => isset($value['canceled']) ? count($value['canceled']) : 0,
			'closed' => isset($value['closed']) ? count($value['closed']) : 0,
			'bid_created' => isset($value['bid_orders']['orders']) ? count($value['bid_orders']['orders']) : 0,
			'bid_no_pro' => isset($value['bid_orders']['no_proposal']) ? count($value['bid_orders']['no_proposal']) : 0,
			'bid_not_confirmed' => isset($value['bid_orders']['approved']) ? count($value['bid_orders']['approved']) : 0,
		);
	}

	$per_page = 20;
	// Initialize the pager
	$current_page = pager_default_initialize(count($rows), $per_page);
	// Split list into page sized chunks
	$chunks = array_chunk($rows, $per_page, TRUE);

	$output = theme_table(array(
		'header' => array(
			'mdc_0' => t('MDC level 1'), 
			'mdc_1' => t('MDC level 2'), 
			'mdc_2' => t('MDC level 3'),
			'mdc_3' => t('MDC level 4'),
			'country' => t('Country'),
			'city' => t('City'),
			'direct_created' => t('Direct Orders created'),
			'confirmed' => t('Confirmed Orders'),
			'opened' => t('Open Orders'),
			'canceled' => t('Canceled Orders'),
			'closed' => t('Closed Orders'),
			'bid_created' => t('Bid Orders created'),
			'bid_no_pro' => t('Bid Orders with no proposal'),
			'bid_not_confirmed' => t('Bid Orders not confirmed'),
		),
		'rows' => $chunks[$current_page],
		'attributes' => array(),
		'caption' => t(''),
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => t('No items found'),
	));

	// Show the pager
	$output .= theme('pager', array('quantity',count($rows)));

	$form['result_table'] = array(
		'#markup' => $output,
	);

	$form['#attached']['css'][] = drupal_get_path('module', 'maritpal') .'/css/maritpal.css';
	$form['#submit'][] = 'maritpal_reports_mdc_figures_by_providers_and_region_form_submit';
	return $form;	
}

function maritpal_reports_mdc_figures_by_providers_and_region_form_submit(&$form, &$form_state) {
	$q_params = array();
	if (!empty($form_state['values']['period_from'])) $q_params['period_from'] = $form_state['values']['period_from'];
	if (!empty($form_state['values']['period_to'])) $q_params['period_to'] = $form_state['values']['period_to'];
	if (!empty($form_state['values']['service_location'][0])) $q_params['service_location'] = $form_state['values']['service_location'][0];
	if (!empty($form_state['values']['service_category'][0])) $q_params['service_category'] = $form_state['values']['service_category'][0];

	if(count($q_params) > 0)
		$form_state['redirect'] = array(current_path(), array('query' => $q_params));
}

/**
 * Report marketers form
 */
function maritpal_reports_mdc_figures_by_marketers_form($form, $form_state) {
	// Path query
	$q_params = drupal_get_query_parameters();
	// dpm($q_params);

	$form = maritpal_reports_filters_form($form, $form_state);

	unset(
		$form['filters']['mdc']['service_category']
	);

	$form['filters']['period']['#title'] = t('Provider created period');

	$form['filters']['marketing_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Marketing code'),
		'#description' => t('Contains value to compare'),
		'#default_value' => isset($q_params['marketing_code']) ? $q_params['marketing_code'] : '',
		'#weight' => 49,
	);

	// select providers role
	$result = db_select('users_roles', 'ur')
		->fields('ur', array('uid'))
		->condition('rid', 4, '=')
		->execute()
		->fetchAll();
	
	$uids = array();
	foreach ($result as $value) {
		$uids[] = $value->uid;
	}

	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'user')
		->propertyCondition('uid', $uids, 'IN')
		->addMetaData('account', user_load(4));

	// Provider location filter
	if (!empty($q_params['service_location']))
		$query->fieldCondition('field_serv_loc_city', 'tid', $q_params['service_location'], 'CONTAINS');

	// Marketing code filter
	if (!empty($q_params['marketing_code']))
		$query->fieldCondition('field_user_marketing_code', 'value', $q_params['marketing_code'], 'CONTAINS');

	// Provider created filter
	if (!empty($q_params['period_from']))
		$query->propertyCondition('created', strtotime($q_params['period_from']), '>');
	if (!empty($q_params['period_to']))
		$query->propertyCondition('created', strtotime($q_params['period_to']), '<');

	$eq_users = $query->execute();

	$rows = array();
	if (isset($eq_users['user'])) {
		$uids = array_keys($eq_users['user']);
		$users = user_load_multiple($uids);
		$uids = array_combine($uids, $uids);

		foreach ($users as $uid => $user) {
			if (!isset($user->field_user_marketing_code['und']) || empty($user->field_user_marketing_code['und'][0]['value'])) {
				unset(
					$users[$uid],
					$uids[$uid]
				);
			}
		}

		// dpm($users);
		// dpm($uids);

		$query = new EntityFieldQuery();
		$nodes = $query->entityCondition('entity_type', 'node')
			->entityCondition('bundle', 'request')
			->fieldCondition('field_resource_ref', 'uid', $uids, 'IN')
			->fieldCondition('field_request_status', 'tid', array(11, 12, 13, 14, 15, 16, 17), 'IN')
			->addMetaData('account', user_load(4))
			->execute();
		
		if (isset($nodes['node'])) {
			$nids = array_keys($nodes['node']);
			$nodes = entity_load('node', $nids);
		} else $nodes = array();

		// dpm($nodes);

		$stat = array();
		foreach ($users as $uid => $user) {
			$mcode = preg_replace('/ /', '_', $user->field_user_marketing_code['und'][0]['value']);

			if (!isset($user->field_serv_loc_city['und'])) continue;
			else {
				$location = isset($user->field_serv_loc_city['und']) ? end($user->field_serv_loc_city['und'])['tid'] : 0;
				$stat[$location][$mcode]['users'][] = $uid;

				foreach ($nodes as $nid => $node) {
					if (!isset($node->field_order_type['und']) || !isset($node->field_resource_ref['und']) || !isset($node->field_request_status['und'])) continue;

					if ($node->field_resource_ref['und'][0]['uid'] == $uid) {
						$stat[$location][$mcode]['orders']['total'][] = $node->nid;

						if ($node->field_order_type['und'][0]['value'] == 1) $stat[$location][$mcode]['orders']['direct'][] = $node->nid;
						elseif ($node->field_order_type['und'][0]['value'] == 2) $stat[$location][$mcode]['orders']['bid'][] = $node->nid;
					}
				}
			}
		}

		dpm($stat);

		foreach ($stat as $tid => $locset) {
			$location = maritpal_get_mdc_location_tree($tid);

			foreach ($locset as $mcode => $value) {
				$rows[] = array(
					'country' => $location['country'],
					'city' => $location['city'],
					'mark_code' => $mcode,
					'prov_num' => count($value['users']),
					'direct_confirmed' => count($value['orders']['direct']),
					'bid_confirmed' => count($value['orders']['bid']),
					'total' => count($value['orders']['total']),
				);
			}
		}
	}

	$per_page = 20;
	// Initialize the pager
	$current_page = pager_default_initialize(count($rows), $per_page);
	// Split list into page sized chunks
	$chunks = array_chunk($rows, $per_page, TRUE);

	$output = theme_table(array(
		'header' => array(
			'country' => t('Country'),
			'city' => t('City'),
			'mark_code' => t('Marketing code'),
			'prov_num' => t('Number of providers'),
			'direct_confirmed' => t('Number of Direct Orders confirmed'),
			'bid_confirmed' => t('Number of Bid Orders confirmed'),
			'total' => t('Total Orders'),
		),
		'rows' => $chunks[$current_page],
		'attributes' => array(),
		'caption' => t(''),
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => t('No items found'),
	));

	// Show the pager
	$output .= theme('pager', array('quantity',count($rows)));

	$form['result_table'] = array(
		'#markup' => $output,
	);

	$form['#attached']['css'][] = drupal_get_path('module', 'maritpal') .'/css/maritpal.css';
	$form['#submit'][] = 'maritpal_reports_mdc_figures_by_marketers_form_submit';
	return $form;	
}

function maritpal_reports_mdc_figures_by_marketers_form_submit(&$form, &$form_state) {
	$q_params = array();
	if (!empty($form_state['values']['period_from'])) $q_params['period_from'] = $form_state['values']['period_from'];
	if (!empty($form_state['values']['period_to'])) $q_params['period_to'] = $form_state['values']['period_to'];
	if (!empty($form_state['values']['service_location'][0])) $q_params['service_location'] = $form_state['values']['service_location'][0];
	if (!empty($form_state['values']['service_category'][0])) $q_params['service_category'] = $form_state['values']['service_category'][0];
	if (!empty($form_state['values']['marketing_code'])) $q_params['marketing_code'] = $form_state['values']['marketing_code'];

	if(count($q_params) > 0)
		$form_state['redirect'] = array(current_path(), array('query' => $q_params));

	// dpm($form_state);
}

/**
 * MDC Filters form (locations/categories)
 *
 * @see maritpal_reports_mdc_figures_by_orders_form()
 */
function maritpal_reports_filters_form($form, &$form_state) {
	// Path query
	$q_params = drupal_get_query_parameters();

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Filters'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$form['filters']['period'] = array(
		'#type' => 'fieldset',
		'#title' => t('Period'),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE,
	);
	$form['filters']['period']['period_from'] = array(
		'#type' => 'date_popup',
		'#title' => t('From'),
		'#date_timezone' => date_default_timezone(),
		'#date_format' => 'Y-m-d',
		'#date_year_range' => '-10:now',
		'#default_value' => !empty($q_params['period_from']) ? $q_params['period_from'] : NULL,
	);
	$form['filters']['period']['period_to'] = array(
		'#type' => 'date_popup',
		'#title' => t('To'),
		'#date_timezone' => date_default_timezone(),
		'#date_format' => 'Y-m-d',
		'#date_year_range' => '-10:now',
		'#default_value' => !empty($q_params['period_to']) ? $q_params['period_to'] : NULL,
	);

	$form['filters']['mdc'] = array(
		'#type' => 'fieldset',
		'#title' => t('MDC'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$form['filters']['mdc']['service_location'] = array(
		'#type' => 'hierarchical_select',
    '#title' => t('Location'),
    '#size' => 1,
    '#config' => array(
      'module' => 'hs_taxonomy',
      'params' => array(
        'vid' => 14,
        'separator' => '|',
      ),
      'save_lineage'    => 0,
      'enforce_deepest' => 0,
      'resizable'       => 0,
      'level_labels' => array(
        'status' => 1,
        'labels' => array(
          0 => t(' - Select Country - '),
          1 => t(' - Select City - '),
        ),
      ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => t('All selections'), 
        'limit'    => 0,
        'reset_hs' => 1,
        'sort'     => 1,
      ),
      'editability' => array(
        'status'           => 0,
        'item_types'       => array(),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
          2 => 1,
        ),
        'allow_new_levels' => 0,
        'max_levels'       => 3,
      ),
      'entity_count'    => array(
        'enabled' => 0,
        'require_entity' => 0,
        'settings' => array(
          'count_children' => 0,
          'entity_types' => array(),
        ),
      ),
      // These settings cannot be configured through the UI: they can only be
      // overridden through code.
      'animation_delay'    => 400,
      'exclusive_lineages' => array(),
      'render_flat_select' => 0,
    ),
    '#description' => t(''),
    '#default_value' => !empty($q_params['service_location']) ? $q_params['service_location'] : '',
    '#weight' => -49,
	);
	$form['filters']['mdc']['service_category'] = array(
		'#type' => 'hierarchical_select',
    '#title' => t('Category'),
    '#size' => 1,
    '#config' => array(
      'module' => 'hs_taxonomy',
      'params' => array(
        'vid' => 13,
        'separator' => '|',
      ),
      'save_lineage'    => 0,
      'enforce_deepest' => 0,
      'resizable'       => 0,
      'level_labels' => array(
        'status' => 1,
        'labels' => array(
          0 => t(' - MDC 1 - '),
          1 => t(' - MDC 2 - '),
          2 => t(' - MDC 3 - '),
          3 => t(' - MDC 4 - '),
        ),
      ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => t('All selections'),
        'limit'    => 0,
        'reset_hs' => 1,
        'sort'     => 1,
      ),
      'editability' => array(
        'status'           => 0,
        'item_types'       => array(),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
          2 => 1,
        ),
        'allow_new_levels' => 0,
        'max_levels'       => 4,
      ),
      'entity_count'    => array(
        'enabled' => 0,
        'require_entity' => 0,
        'settings' => array(
          'count_children' => 0,
          'entity_types' => array(),
        ),
      ),
      // These settings cannot be configured through the UI: they can only be
      // overridden through code.
      'animation_delay'    => 400,
      'exclusive_lineages' => array(),
      'render_flat_select' => 0,
    ),
    '#description' => t(''),
    '#default_value' => !empty($q_params['service_category']) ? $q_params['service_category'] : '',
	);

	$form['filters']['actions'] = array(
		'#type' => 'container',
		'#weight' => 50,
	);
	$form['filters']['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Filter'),
	);
	$form['filters']['actions']['cancel'] = array(
		'#type' => 'item',
		'#markup' => '<a href="/'. current_path() .'">'. t('Reset') .'</a>',
	);

	return $form;
}

/**
 * Return sorted mdc locations to countries and cities
 */
function _sort_mdc_locations() {
	$locations = taxonomy_get_tree(14);
	$result = array();
	foreach ($locations as $term) {
		if ($term->parents[0] == 0) $result['countries'][] = $term->name;
		else $result['cities'][] = $term->name;
	}

	return $result;
}

/**
 * Return of aranged mdc categories
 *
 * @param int $tid
 *	Taxonomy mdc TID
 *
 * @return array
 */
function maritpal_get_mdc_category_tree($tid) {
	$tree = taxonomy_get_parents_all($tid);
	$tree_rev = array_reverse($tree);

	$result = array();
	if (count($tree_rev) > 0) {
		foreach ($tree_rev as $k => $term) {
			$result["mdc_$k"] = $term->name;
		}
	}

	return $result;
}

/**
 * Return of aranged mdc locations
 *
 * @param int $tid
 *	Taxonomy mdc TID
 *
 * @return array
 */
function maritpal_get_mdc_location_tree($tid) {
	$tree = taxonomy_get_parents_all($tid);

	$result = array();
	if (count($tree) > 0) {
		$city = current($tree);
		$country = end($tree);

		$result = array (
			'country' => $country->name,
			'city' => $city->name,
		);
	}

	return $result;
}

// /**
//  * Account balance deposit cash form
//  */
// function maritpal_commerce_account_balance_deposit_cash($form, &$form_state, $account) {
// 	$line_item = commerce_line_item_new('deposit');
//   $form_state['line_item'] = $line_item;
//   $form_state['order'] = commerce_order_new();
//   $form_state['account'] = $account;

//   field_attach_form('commerce_line_item', $line_item, $form, $form_state);

// 	$form['balance_amount'] = array(
// 		'#type' => 'textfield',
// 		'#title' => t('Balance'),
// 		'#requred' => TRUE,
// 		'#weight' => -50
// 	);
	
// 	$form['balance_currency'] = array(
// 		'#type' => 'select',
// 		'#description' => t('Please select currency of Your location'),
// 		'#options' => _get_enabled_currencies_options(),
// 		'#requred' => TRUE,
// 		'#weight' => -49
// 	);

//   // // The standard commerce unit price widget will have to be hidden.
//   // $form['commerce_unit_price'][LANGUAGE_NONE][0]['amount']['#type'] = 'hidden';
//   // $form['commerce_unit_price'][LANGUAGE_NONE][0]['amount']['#value'] = 0;

// 	$form['actions'] = array(
// 		'#type' => 'container',
// 	);
// 	$form['actions']['submit'] = array(
// 		'#type' => 'submit',
// 		'#value' => t('Deposit cash')
// 	);

// 	return $form;
// }

// function maritpal_commerce_account_balance_deposit_cash_submit($form, &$form_state) {
// 	dpm($form_state);
// }