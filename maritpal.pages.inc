<?php

/**
 * @file
 * User menu callbacks for Maritpal.
 */

/**
 * Render display Price Rules table
 */
function maritpal_pricing_rules_review() {
	$query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'taxonomy_term')
  ->propertyCondition('vid', 23, '=')
  ->execute();
  
	$rows = array();
  if (count($result['taxonomy_term']) > 0) {
  	$terms = entity_load('taxonomy_term', array_keys($result['taxonomy_term']));

  	foreach ($terms as $term) {
  		$location = taxonomy_term_load($term->field_serv_loaction['und'][0]['tid']);
  		$location = isset($location->name) ? $location->name : '';

  		$category = taxonomy_term_load($term->field_serv_cat['und'][0]['tid']);
  		$category = isset($category->name) ? $category->name : '';

  		$rate_amount = isset($term->field_serv_rate_amount['und']) ? $term->field_serv_rate_amount['und'][0]['value'] : ''; 
			$rate_currency = isset($term->field_serv_rate_currency['und']) ? $term->field_serv_rate_currency['und'][0]['currency_code'] : '';

  		$rows[] = array(
  			'name' => $term->name, 
				'location' => $location, 
				'category' => $category,
				'sp_mail' => isset($term->field_serv_sp_mail['und']) ? $term->field_serv_sp_mail['und'][0]['email'] : '',
				'serv_rate' => $rate_amount .' '. $rate_currency,
				'serv_duration' => isset($term->field_serv_rate_dur['und']) ? $term->field_serv_rate_dur['und'][0]['value'] : '',
				'operations' => 
					'<a href="/admin/maritpal/pricing-rules/'. $term->tid .'/edit">'. t('Edit') .'</a> | '.
					'<a href="/admin/maritpal/pricing-rules/'. $term->tid .'/delete">'. t('Delete') .'</a>',
  		);
  	}
  }

	$output = theme_table(array(
		'header' => array(
			'name' => t('Name'), 
			'location' => t('Service Location'), 
			'category' => t('Service Category'),
			'sp_mail' => t('Service Provider Mail'),
			'serv_rate' => t('Service Rate'),
			'serv_duration' => t('Service Duration (per)'),
			'operations' => t('Operations'),
		),
		'rows' => $rows,
		'attributes' => array(),
		'caption' => t(''),
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => t('No items found')
	));

	return $output;
}

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
	);
	$form['filters']['period']['period_to'] = array(
		'#type' => 'date_popup',
		'#title' => t('To'),
		'#date_timezone' => date_default_timezone(),
		'#date_format' => 'Y-m-d',
		'#date_year_range' => '-10:now',
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
		'#markup' => '<a href="#">'. t('Cancel') .'</a>',
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
		dpm($uids);

		foreach ($users as $uid => $user) {
			if (!in_array($uid, $uids)) unset($users[$uid]);
		}
	}

	// Count users for each MDC
	$count_users = array();
	foreach ($users as $uid => $user) {
		foreach ($user->deapest_service_categories as $tid) {
			$count_users[$tid .'_'. key($user->deapest_service_location)][] = $user->uid;
		}
	}

	// get regular service order of mdc 
	$query = new EntityFieldQuery();
	$res = $query->entityCondition('entity_type', 'node')
		->entityCondition('bundle', 'request')
		->fieldCondition('field_order_type', 'value', 1, '=')
		->addMetaData('account', user_load(4));
	
	$res = $query->execute();
	
	// dpm($res);

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
	
		$reg_orders = $query->execute();

		// get bid service order of mdc 
		$query = new EntityFieldQuery();
		$bid_orders = $query->entityCondition('entity_type', 'node')
			->entityCondition('bundle', 'request')
			->fieldCondition('field_order_type', 'value', 2, '=')
			->fieldCondition('field_serv_loc_city', 'tid', $mdc[1], '=')
			->fieldCondition('field_request_serv_cat', 'tid', $mdc[0], '=')
			->addMetaData('account', user_load(4));
	
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

	$per_page = 50;
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

function maritpal_reports_mdc_figures_by_providers_and_region_form_submit(&$form, &$form_state) {
	$q_params = array();
	if (!empty($form_state['values']['period_from'])) $q_params['period_from'] = $form_state['values']['period_from'];
	if (!empty($form_state['values']['period_to'])) $q_params['period_to'] = $form_state['values']['period_to'];
	if (!empty($form_state['values']['service_location'][0])) $q_params['service_location'] = $form_state['values']['service_location'][0];
	if (!empty($form_state['values']['service_category'][0])) $q_params['service_category'] = $form_state['values']['service_category'][0];

	// dpm($q_params, url('/'. current_path(), array('query' => $q_params)));

	if(count($q_params) > 0)
		$form_state['redirect'] = array(current_path(), array('query' => $q_params));

	dpm($form_state);
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