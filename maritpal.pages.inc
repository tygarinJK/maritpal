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

function maritpal_reports_mdc_figures_by_providers_and_region() {
	$query = 'SELECT DISTINCT(ur.uid) 
						FROM {users_roles} AS ur
						WHERE ur.rid IN (:rids)';
	// $result = db_query($query, array(':rids' => array(4)));

	// $uids = $result->fetchCol();

	// $users = user_load_multiple($uids);

	// dpm($users);


	// $tmp_tree = array();
	// create help array with arranged terms (mdc)
	// foreach ($raw_tree as $key => $term) {
	// 	if ($term->parents[0] == 0) {
	// 		$tmp_tree['mdc_1'][$term->tid] = array(
	// 			'name' => $term->name,
	// 			'parent' => NULL,
	// 		);
	// 		unset($raw_tree[$key]);
	// 	}
	// }
	// for ($i = 1, $j = 2; $i < 4; $i++, $j++) {
	// 	foreach ($raw_tree as $key => $term) {
	// 		foreach ($tmp_tree["mdc_$i"] as $k => $v) {
	// 			if ($term->parents[0] == $k) {
	// 				$tmp_tree["mdc_$j"][$term->tid] = array(
	// 					'name' => $term->name,
	// 					'parent' => $k,
	// 					'parent_name' => $v['name'],
	// 				);
	// 				unset($raw_tree[$key]);
	// 			}
	// 		}
	// 	}
	// }
	// dpm($tmp_tree);

	// $raw_tree = taxonomy_get_tree(13);
	// $deapest = array();
	// // categories: refill array with tid keys
	// foreach ($raw_tree as $term) {
	// 	$deapest[$term->tid] = $term;
	// }
	// // categories: filter to deapest terms
	// foreach ($raw_tree as $term) {
	// 	if ($term->parents[0] === 0) unset($deapest[$term->tid]);
	// 	else unset($deapest[$term->parents[0]]);
	// }
	// // categories: fill table rows
	// foreach ($deapest as $tid => $term) {
	// 	$tree = taxonomy_get_parents_all($tid);
	// 	$tree_rev = array_reverse($tree);
	// 	foreach ($tree_rev as $k => $term) {
	// 		$rows[$tid]["mdc_$k"] = $term->name;
	// 	}
	// }

	
	


	// dpm($rows);




	$output = theme_table(array(
		'header' => array(
			'mdc_0' => t('MDC level 1'), 
			'mdc_1' => t('MDC level 2'), 
			'mdc_2' => t('MDC level 3'),
			'mdc_3' => t('MDC level 4'),
			'country' => t('Country'),
			'city' => t('City'),
			'no_prov' => t('Nomber of providers'),
			'regular_order' => t('Created order'),
			'bit_order' => t('Created bid order'),
			'ratio_order_prov' => t('Ratio order/provider'),
		),
		'rows' => array(),
		'attributes' => array(),
		'caption' => t(''),
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => t('No items found'),
	));

	return $output;
}