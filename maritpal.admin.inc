<?php

/**
 * @file
 * Administration menu callbacks for Meritpal.
 */

/**
 * Meritpal admin form
 */
function maritpal_admin_form($form, $form_state) {
	return $form;
}

/**
 * Administrative Pricing Settings form
 */
function maritpal_admin_pricing_settings_form($form, &$form_state) {

	// Service Provider comission
	$form['sp_comissions'] = array(
		'#type' => 'fieldset',
		'#title' => t('Service provider comissions'),
		'#collepsible' => FALSE,
		'#collapsed' => FALSE,
	);
	$form['sp_comissions']['maritpal_admin_sp_request_comission'] = array(
		'#type' => 'textfield',
		'#title' => t('Service provider request comission'),
		'#default_value' => variable_get('maritpal_admin_sp_request_comission', 5),
		'#required' => TRUE,
		'#suffix' => '%'
	);

	// Taxes & Rates
	$form['rates'] = array(
		'#type' => 'fieldset',
		'#title' => t('Rates & Taxes'),
		'#collepsible' => FALSE,
		'#collapsed' => FALSE,
	);
	$form['rates']['maritpal_admin_rate'] = array(
		'#type' => 'textfield',
		'#title' => t('Admin rate'),
		'#default_value' => variable_get('maritpal_admin_rate', 0),
		'#element_validate' => array('element_validate_number'),
		'#size' => 60, 
	  '#maxlength' => 128, 
		'#field_suffix' => '%',
		'#required' => TRUE,
	);
	$form['rates']['maritpal_transfer_rate'] = array(
		'#type' => 'textfield',
		'#title' => t('Transfer rate'),
		'#default_value' => variable_get('maritpal_transfer_rate', 0),
		'#element_validate' => array('element_validate_number'),
		'#size' => 60, 
	  '#maxlength' => 128, 
		'#field_suffix' => '%',
		'#required' => TRUE,
	);
	$form['rates']['maritpal_surcharge_rate'] = array(
		'#type' => 'textfield',
		'#title' => t('Surcharge'),
		'#default_value' => variable_get('maritpal_surcharge_rate', 0),
		'#element_validate' => array('element_validate_number'),
		'#size' => 60, 
	  '#maxlength' => 128, 
		'#field_suffix' => '%',
		'#required' => TRUE,
	);

	$form['rates']['taxes_container'] = array(
		'#type' => 'fieldset',
		'#title' => t('Additional Taxes'),
		'#collepsible' => FALSE,
		'#collapsed' => FALSE,
	);
	$form['rates']['taxes_container']['maritpal_taxes'] = array(
		'#type' => 'container',
	  '#tree' => TRUE,
	  '#prefix' => '<div id="taxes-list">',
	  '#suffix' => '</div>',
	);

	$maritpal_additional_taxes = variable_get('maritpal_additional_taxes', array());

	if (!isset($form_state['storage']['additional_taxes'])) {
		foreach ($maritpal_additional_taxes as $i => $tax) {
			$form['rates']['taxes_container']['maritpal_taxes'][$i] = array(
				'#type' => 'container',
			);
		  $form['rates']['taxes_container']['maritpal_taxes'][$i]['name'] = array(
		    '#type' => 'textfield',
		    '#title' => t('Tax Name'),
		    '#default_value' => $maritpal_additional_taxes[$i]['name'],
		    '#size' => 60, 
	  		'#maxlength' => 128, 
		  );
		  $form['rates']['taxes_container']['maritpal_taxes'][$i]['value'] = array(
		    '#type' => 'textfield',
		    '#title' => t('Tax Value'),
		    '#default_value' => $maritpal_additional_taxes[$i]['value'],
		    '#size' => 60, 
	  		'#maxlength' => 128, 
		    '#suffix' => '%',
		  );
		}

		$form_state['storage']['additional_taxes'] = count($maritpal_additional_taxes);
	}
	else {
		for ($i = 0; $i < $form_state['storage']['additional_taxes']; $i++) {
			$form['rates']['taxes_container']['maritpal_taxes'][$i] = array(
				'#type' => 'container',
			);
		  $form['rates']['taxes_container']['maritpal_taxes'][$i]['name'] = array(
		    '#type' => 'textfield',
		    '#title' => t('Tax Name'),
		    '#default_value' => $maritpal_additional_taxes[$i]['name'],
		    '#size' => 60, 
	  		'#maxlength' => 128, 
		  );
		  $form['rates']['taxes_container']['maritpal_taxes'][$i]['value'] = array(
		    '#type' => 'textfield',
		    '#title' => t('Tax Value'),
		    '#default_value' => $maritpal_additional_taxes[$i]['value'],
		    '#size' => 60, 
	  		'#maxlength' => 128, 
		    '#suffix' => '%',
		  );
		}
	}

	$form['rates']['taxes_container']['add_tax'] = array(
		'#type' => 'button',
	  '#value' => t('Add Tax'),
	  '#href' => '',
	  '#ajax' => array(
	    'callback' => 'maritpal_ajax_add_tax_callback',
	    'wrapper' => 'taxes-list',
	   ),
	);
	$form_state['storage']['additional_taxes']++;
	
	// Discounts
	$form['discounts'] = array(
		'#type' => 'fieldset',
		'#title' => t('Discounts'),
		'#collepsible' => FALSE,
		'#collapsed' => FALSE,
	);

	// Invoice Payment
	$form['invoice'] = array(
		'#type' => 'fieldset',
		'#title' => t('Invoice Payment'),
		'#collepsible' => FALSE,
		'#collapsed' => FALSE,
	);
	$maritpal_invoice_description = variable_get('maritpal_invoice_description', array('value' => '', 'format' => 'full_html'));
	$form['invoice']['maritpal_invoice_description'] = array(
		'#type' => 'text_format',
    '#title' => t('Invoice PAyment Description'),
    '#default_value' => $maritpal_invoice_description['value'],
    '#format' => $maritpal_invoice_description['format'],
	);

	$form['actions'] = array(
		'#type' => 'container',
	);
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save configuration'),
	);

	$form['#attached']['css'][] = drupal_get_path('module', 'maritpal') .'/css/maritpal.css';
  $form['#submit'][] = 'maritpal_admin_pricing_settings_form_submit';
  return system_settings_form($form);
}

function maritpal_admin_pricing_settings_form_submit(&$form, &$form_state) {
	foreach ($form_state['values']['maritpal_taxes'] as $i => $tax) {
		if (empty($tax['name']) || empty($tax['value'])) {
			unset($form_state['values']['maritpal_taxes'][$i]);
		}
	}
	variable_set('maritpal_additional_taxes', $form_state['values']['maritpal_taxes']);

	// dpm($form_state);
  drupal_theme_rebuild();
}

/**
 * Ajax callback
 *
 * @see maritpal_admin_pricing_settings_form()
 */
function maritpal_ajax_add_tax_callback($form, $form_state) {
	return $form['rates']['taxes_container']['maritpal_taxes'];
}

/**
 * Pricing Rules form
 */
function maritpal_admin_pricing_rules_form($form, &$form_state, $term = NULL) {
	if (!is_null($term)) {
		dpm($term);
		$form_state['term'] = $term;
	}

	// $form['admin_title'] = array(
	// 	'#type' => 'textfield',
	// 	'#title' => t('Admin title'),
	// 	'#descdription' => t('Shown only on management screens, not shown to customers.'),
	// 	'#default_value' => !empty($form_state['values']['admin_title']) ? $form_state['values']['admin_title'] : '',
	// 	'#required' => TRUE,
	// 	'#weight' => -50
	// );

	$form['request_serv_loc_hs'] = array(
    '#type' => 'hierarchical_select',
    '#title' => t('Service Location'),
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
          0 => t(' - Select country - '),
          1 => t(' - Select city - '),
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
    '#default_value' => isset($term->field_serv_loaction['und']) ? _service_category_default_value($term->field_serv_loaction['und'][0]['tid']) : '',
    '#required' => TRUE,
    '#weight' => -49,
  );

  $form['request_serv_cat_hs'] = array(
    '#type' => 'hierarchical_select',
    '#title' => t('Service Category'),
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
          0 => t(' - Select MDC 1 - '),
          1 => t(' - Select MDC 2 - '),
          2 => t(' - Select MDC 3 - '),
          3 => t(' - Select MDC 4 - '),
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
    '#default_value' => isset($term->field_serv_cat['und']) ? $term->field_serv_cat['und'][0]['tid'] : '',
    '#required' => TRUE,
    '#weight' => -48,
  );

  $form['provider_mail'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Provider Email'),
  	'#default_value' => !empty($term->field_serv_sp_mail['und']) ? $term->field_serv_sp_mail['und'][0]['email'] : '',
  	'#size' => 60, 
	  '#maxlength' => 128, 
  );


  $form['rate_container'] = array(
  	'#type' => 'container',
  );

  $form['rate_container']['rate'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Rate'),
  	'#default_value' => isset($term->field_serv_rate_amount['und']) ? $term->field_serv_rate_amount['und'][0]['value'] : '',
  	'#element_validate' => array('element_validate_number'),
  	'#size' => 60, 
	  '#maxlength' => 128, 
  	'#required' => TRUE,
  );

  $currs = commerce_currencies(TRUE);
  $currencies = array();
  foreach ($currs as $key => $curr) {
  	$currencies[$key] = $curr['code'];
  }
  $form['rate_container']['currency'] = array(
  	'#type' => 'select',
  	'#title' => t('Currency'),
  	'#options' => $currencies,
  	'#default_value' => isset($term->field_serv_rate_currency['und']) ? $term->field_serv_rate_currency['und'][0]['currency_code'] : '',
  	'#required' => TRUE,
  );

  $form['rate_container']['per'] = array(
  	'#type' => 'select',
  	'#title' => t('Per'),
  	'#options' => array(
  		'hour' => t('Hour'),
  		'visit' => t('Visit'), 
  	),
  	'#default_value' => isset($term->field_serv_rate_dur['und']) ? $term->field_serv_rate_dur['und'][0]['value'] : '',
  	'#field_prefix' => '1',
  	'#required' => TRUE,
  );

	$form['actions'] = array(
		'#type' => 'container',
		'#weight' => 50,
	);
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
	);
	$form['actions']['cancel'] = array(
		'#markup' => '<a href="/admin/maritpal/pricing-rules">'. t('Cancel') .'</a>',
		'#prefix' => t('or'),
	);

	$form['#attached']['css'][] = drupal_get_path('module', 'maritpal') .'/css/maritpal.css';
  $form['#validate'][] = 'maritpal_admin_pricing_rules_form_validate';
  $form['#submit'][] = 'maritpal_admin_pricing_rules_form_submit';
  return $form;
}

function maritpal_admin_pricing_rules_form_validate($form, &$form_state) {
	if (!empty($form_state['values']['provider_mail']) && !valid_email_address($form_state['values']['provider_mail'])) {
		form_set_error('provider_mail', t('The email address appears to be invalid.'));
	}
}

function maritpal_admin_pricing_rules_form_submit($form, &$form_state) {
	global $user;

	$term = new stdClass();
	
	if (isset($form_state['term'])) $term = $form_state['term'];

	// $term->name = !empty($form_state['values']['admin_title']) ? $form_state['values']['admin_title'] : '__';
	$term->uid = $user->uid;
	$term->vid = 23;
	$term->field_serv_loaction['und'][0]['tid'] = end($form_state['values']['request_serv_loc_hs']);
	$term->field_serv_cat['und'][0]['tid'] = end($form_state['values']['request_serv_cat_hs']);
	$term->field_serv_sp_mail['und'][0]['email'] = $form_state['values']['provider_mail'];

	$term->field_serv_price_ph['und'][0]['amount'] = $form_state['values']['rate'];
	$term->field_serv_price_ph['und'][0]['currency_code'] = $form_state['values']['currency'];

	$term->field_serv_rate_amount['und'][0]['value'] = $form_state['values']['rate'];
	$term->field_serv_rate_currency['und'][0]['currency_code'] = $form_state['values']['currency'];
	$term->field_serv_rate_dur['und'][0]['value'] = $form_state['values']['per'];

	taxonomy_term_save($term);

	$form_state['redirect'] = 'admin/maritpal/settings/pricing-rules';
	drupal_set_message(t('Pricing Rule was successfuly created'), 'status', FALSE);

	// dpm($term);
	// dpm($form_state);
}

/**
 * Service category field default value
 *
 * @param int $tid
 *	Deapest level term id
 *
 * @return array
 */
function _service_category_default_value($tid = NULL) {
	if (is_null($tid)) return '';

	$parents = taxonomy_get_parents_all($tid);

	$result = array();
	foreach ($parents as $term) {
		$result[] = $term->tid;
	}
	return array_reverse($result);
}

/**
 * Pricing Rule delete confirm form
 */
function maritpal_admin_pricing_rules_delete_confirm_form($form, &$form_state, $term = NULL) {
	$form_state['term'] = $term;

	return confirm_form(
		$form,
		t('Are you sure you want to delete this item?'),
		'admin/maritpal/pricing-rules',
		t('This action cannot be undone.'),
		t('Delete'),
		t('Cancel')
	);
}

function maritpal_admin_pricing_rules_delete_confirm_form_submit($form, &$form_state) {
	if (!isset($form_state['term']->tid)) return FALSE;

	$form_state['redirect'] = 'admin/maritpal/pricing-rules';
	return taxonomy_term_delete($form_state['term']->tid);
}