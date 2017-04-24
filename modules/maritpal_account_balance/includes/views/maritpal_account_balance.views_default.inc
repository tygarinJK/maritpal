<?php

/**
 * @file
 * Generates default views for Maritpal Account Balance.
 */

/**
 * Implements hook_views_default_views().
 */
function maritpal_account_balance_views_default_views() {

  $view = new view();
  $view->name = 'maritpal_balance_transactions';
  $view->description = '';
  $view->tag = 'maritpal';
  $view->base_table = 'commerce_account_balance_transaction';
  $view->human_name = 'Maritpal balance transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My transactions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'maritpal balance admin settings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Commerce Account Balance Transaction: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Commerce Account Balance Transaction: Unique transaction ID */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['label'] = 'Transaction ID';
  $handler->display->display_options['fields']['transaction_id']['separator'] = '';
  /* Field: Commerce Account Balance Transaction: Transaction action */
  $handler->display->display_options['fields']['action']['id'] = 'action';
  $handler->display->display_options['fields']['action']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['action']['field'] = 'action';
  $handler->display->display_options['fields']['action']['label'] = 'Action';
  /* Field: Commerce Account Balance Transaction: Transaction type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  /* Field: Commerce Account Balance Transaction: Time */
  $handler->display->display_options['fields']['time']['id'] = 'time';
  $handler->display->display_options['fields']['time']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['time']['field'] = 'time';
  $handler->display->display_options['fields']['time']['date_format'] = 'short';
  $handler->display->display_options['fields']['time']['second_date_format'] = 'long';
  /* Field: Commerce Account Balance Transaction: Transaction amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['label'] = 'Amount';
  /* Field: Commerce Account Balance Transaction: Transaction currency code */
  $handler->display->display_options['fields']['currency_code']['id'] = 'currency_code';
  $handler->display->display_options['fields']['currency_code']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['currency_code']['field'] = 'currency_code';
  $handler->display->display_options['fields']['currency_code']['label'] = '';
  /* Sort criterion: Commerce Account Balance Transaction: Time */
  $handler->display->display_options['sorts']['time']['id'] = 'time';
  $handler->display->display_options['sorts']['time']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['sorts']['time']['field'] = 'time';
  $handler->display->display_options['sorts']['time']['order'] = 'DESC';
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Commerce Account Balance Transaction: Transaction type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Transaction type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );

  /* Display: Admin balance */
  $handler = $view->new_display('page', 'Admin balance', 'admin_balance_page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Administer balance';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'input_required' => 0,
      'text_input_required' => array(
        'text_input_required' => array(
          'value' => 'Select any filter and click on Apply to see results',
          'format' => 'filtered_html',
        ),
      ),
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
      'secondary_collapse_override' => '0',
    ),
    'type' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'mail' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'uid_raw' => array(
      'bef_format' => 'default',
      'slider_options' => array(
        'bef_slider_min' => '0',
        'bef_slider_max' => '99999',
        'bef_slider_step' => '1',
        'bef_slider_animate' => '',
        'bef_slider_orientation' => 'horizontal',
      ),
      'more_options' => array(
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
      ),
    ),
    'time' => array(
      'bef_format' => 'bef_datepicker',
      'more_options' => array(
        'autosubmit' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
          ),
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
        ),
        'datepicker_options' => '',
      ),
    ),
  );
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'transaction_id' => 'transaction_id',
    'uid' => 'uid',
    'mail' => 'mail',
    'action' => 'action',
    'type' => 'type',
    'time' => 'time',
    'amount' => 'amount',
    'currency_code' => 'amount',
  );
  $handler->display->display_options['style_options']['default'] = 'time';
  $handler->display->display_options['style_options']['info'] = array(
    'transaction_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'action' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'time' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'currency_code' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Commerce Account Balance Transaction: Unique transaction ID */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['label'] = 'Transaction ID';
  $handler->display->display_options['fields']['transaction_id']['separator'] = '';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'UID';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'uid';
  $handler->display->display_options['fields']['mail']['link_to_user'] = '0';
  /* Field: Commerce Account Balance Transaction: Transaction action */
  $handler->display->display_options['fields']['action']['id'] = 'action';
  $handler->display->display_options['fields']['action']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['action']['field'] = 'action';
  $handler->display->display_options['fields']['action']['label'] = 'Action';
  /* Field: Commerce Account Balance Transaction: Transaction type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  /* Field: Commerce Account Balance Transaction: Time */
  $handler->display->display_options['fields']['time']['id'] = 'time';
  $handler->display->display_options['fields']['time']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['time']['field'] = 'time';
  $handler->display->display_options['fields']['time']['date_format'] = 'short';
  $handler->display->display_options['fields']['time']['second_date_format'] = 'long';
  /* Field: Commerce Account Balance Transaction: Transaction amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['label'] = 'Amount';
  /* Field: Commerce Account Balance Transaction: Transaction currency code */
  $handler->display->display_options['fields']['currency_code']['id'] = 'currency_code';
  $handler->display->display_options['fields']['currency_code']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['currency_code']['field'] = 'currency_code';
  $handler->display->display_options['fields']['currency_code']['label'] = '';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Account Balance Transaction: Transaction type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Transaction type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'uid';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'E-mail';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
  );
  /* Filter criterion: User: The user ID */
  $handler->display->display_options['filters']['uid_raw']['id'] = 'uid_raw';
  $handler->display->display_options['filters']['uid_raw']['table'] = 'users';
  $handler->display->display_options['filters']['uid_raw']['field'] = 'uid_raw';
  $handler->display->display_options['filters']['uid_raw']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_raw']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_raw']['expose']['operator_id'] = 'uid_raw_op';
  $handler->display->display_options['filters']['uid_raw']['expose']['label'] = 'UID';
  $handler->display->display_options['filters']['uid_raw']['expose']['operator'] = 'uid_raw_op';
  $handler->display->display_options['filters']['uid_raw']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid_raw']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
  );
  /* Filter criterion: Commerce Account Balance Transaction: Time */
  $handler->display->display_options['filters']['time']['id'] = 'time';
  $handler->display->display_options['filters']['time']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['filters']['time']['field'] = 'time';
  $handler->display->display_options['filters']['time']['exposed'] = TRUE;
  $handler->display->display_options['filters']['time']['expose']['operator_id'] = 'time_op';
  $handler->display->display_options['filters']['time']['expose']['label'] = 'Time';
  $handler->display->display_options['filters']['time']['expose']['operator'] = 'time_op';
  $handler->display->display_options['filters']['time']['expose']['identifier'] = 'time';
  $handler->display->display_options['filters']['time']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
  );
  $handler->display->display_options['path'] = 'admin/maritpal/settings/balance';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Balance transactions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['maritpal_balance_transactions'] = array(
    t('Master'),
    t('My transactions'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('User'),
    t('Transaction ID'),
    t('.'),
    t('Action'),
    t('Type'),
    t('Time'),
    t('Amount'),
    t(','),
    t('All'),
    t('Transaction type'),
    t('Admin balance'),
    t('Administer balance'),
    t('Select any filter and click on Apply to see results'),
    t('Advanced options'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('UID'),
    t('E-mail'),
  );

  $views[$view->name] = $view;

  /** Users Balance view */
  $view = new view();
  $view->name = 'meritpal_users_balance';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Meritpal users balance';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Meritpal users balance';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'maritpal balance admin settings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: User Balance Page */
  $handler = $view->new_display('page', 'User Balance Page', 'admin_user_balance_page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Users balance';
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['pure_distinct'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'UID';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['link_to_user'] = '0';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Balance';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php

  $account = user_load($row->uid);
  $cab = _commerce_account_balance_get_balance_raw($account);
  print $cab[\'amount\'] .\' \'. $cab[\'currency_code\'];

  ?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'E-mail';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
  );
  /* Filter criterion: User: The user ID */
  $handler->display->display_options['filters']['uid_raw']['id'] = 'uid_raw';
  $handler->display->display_options['filters']['uid_raw']['table'] = 'users';
  $handler->display->display_options['filters']['uid_raw']['field'] = 'uid_raw';
  $handler->display->display_options['filters']['uid_raw']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_raw']['expose']['operator_id'] = 'uid_raw_op';
  $handler->display->display_options['filters']['uid_raw']['expose']['label'] = 'UID';
  $handler->display->display_options['filters']['uid_raw']['expose']['operator'] = 'uid_raw_op';
  $handler->display->display_options['filters']['uid_raw']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid_raw']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
  );
  $handler->display->display_options['path'] = 'admin/maritpal/settings/users-balance';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Users balance';
  $handler->display->display_options['menu']['weight'] = '-45';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['meritpal_users_balance'] = array(
    t('Master'),
    t('Meritpal users balance'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('User Balance Page'),
    t('Users balance'),
    t('UID'),
    t('E-mail'),
    t('Balance'),
  );


  $views[$view->name] = $view;

  return $views;
}
