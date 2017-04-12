<?php

/**
 * Implements hook_variable_info().
 */
function maritpal_variable_info($options) {
	// -- Admin Rate
  $variable['pricing:admin_rate:type'] = array(
    'type' => 'select',
    'title' => t('Admin rate type'),
    'options' => array(
      0 => t('Fixed'),
      1 => t('Persentage'),
    ),
    'default' => 0,
    'description' => t('Can be fixed amount or persentage.'),
    'group' => 'pricing_settings',
  );
  $variable['pricing:admin_rate:value'] = array(
    'type' => 'string',
    'title' => t('Admin rate value'),
    'suffix' => 'IDR or %',
    'default' => '0',
    'required' => TRUE,
    'group' => 'pricing_settings',
  );

  // -- Transfer Rate
  $variable['pricing:transfer_rate:type'] = array(
    'type' => 'select',
    'title' => t('Transfer rate type'),
    'options' => array(
      0 => t('Fixed'),
      1 => t('Persentage'),
    ),
    'default' => 0,
    'description' => t('Can be fixed amount or persentage.'),
    'group' => 'pricing_settings',
  );
  $variable['pricing:transfer_rate:value'] = array(
    'type' => 'string',
    'title' => t('Transfer rate value'),
    'suffix' => 'IDR or %',
    'default' => '0',
    'required' => TRUE,
    'group' => 'pricing_settings',
  );

  // -- Bank Fee
  $variable['pricing:bank_fee:value'] = array(
    'type' => 'string',
    'title' => t('Bank fee'),
    'suffix' => '%',
    'default' => '0',
    'description' => t('Bank fee persentage only'),
    'required' => TRUE,
    'group' => 'pricing_settings',
  );

  $variable['payment:invoice_descr'] = array(
    'title' => t('Invoice Payment description.'),
    'type' => 'text_format',
    'format' => 'filtered_html',
    'description' => t('Invoice Payment description text.'),
    'default' => array(
      'value' => 'Maritpal',
      'format' => 'filtered_html'
    ),
    'token' => TRUE,
  );
  
  // $variable['maritpal:deduct_min_point'] = array(
  //   'type' => 'string',
  //   'title' => t('Minimum point to deduct.', array(), $options),
  //   'default' => '0',
  //   'required' => TRUE,
  //   'token' => TRUE,
  // );
  // $variable['site_settings:service_block'] = array(
  //   'type' => 'boolean',
  //   'title' => t('Block payment procedures.', array(), $options),
  //   'description' => t('If Option checked, refillment procedure will be disabled'),
  //   'default' => FALSE,
  //   'token' => TRUE,
  // );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function maritpal_variable_group_info() {
  $groups['pricing_settings'] = array(
    'title' => t('Pricing settings'),
    'description' => t('Taxes and rates.'),
    'access' => 'administer nodes',
    // 'path' => 'admin/structure/marit', 
  );
  return $groups;
}