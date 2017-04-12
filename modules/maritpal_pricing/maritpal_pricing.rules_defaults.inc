<?php

/**
 * Implements hook_menu().
 */
// function maritpal_pricing_menu() {
//   $items = array();

//   $items['add/pricing-rule'] = array(
//     'title' => 'Rule added',
//     'page callback' => 'maritpal_pricing_default_rules_configuration',
//     'access callback' => TRUE
//   );

//   return $items;
// }

/**
 * Implements hook_default_rules_configuration().
 */
// function maritpal_pricing_default_rules_configuration() {

// 	// drupal_set_message(t('message'), 'status');
// 	print('dfg');

// 	// if ( 	isset($_POST['rule_title']) && 
// 	// 			isset($_POST['service_location']) && 
// 	// 			isset($_POST['service_category']) &&
// 	// 			isset($_POST['service_price_per_hour']) &&
// 	// 			isset($_POST['service_rate'])	)
// 	// {
// 	// 	$result = array();
// 	// 	$result['rule_title'] = $_POST['rule_title'];
// 	// 	$result['service_location'] = $_POST['service_location']; 
// 	// 	$result['service_category'] = $_POST['service_category'];
// 	// 	$result['service_price_per_hour'] = $_POST['service_price_per_hour'];
// 	// 	$result['service_rate'] = $_POST['service_rate'];

// 	// 	print_r($result) );

// 	// // 	$configs = array();

// 	// //   $rule = '{ "rules_pricing_rule_indonesia_jakarta_it_consultant_" : {
// 	// //     "LABEL" : "PRICING RULE | Indonesia\/Jakarta\/IT Consultant",
// 	// //     "PLUGIN" : "reaction rule",
// 	// //     "OWNER" : "rules",
// 	// //     "TAGS" : [ "Pricing rule" ],
// 	// //     "REQUIRES" : [ "rules", "commerce_line_item", "commerce_product_reference" ],
// 	// //     "ON" : { "commerce_product_calculate_sell_price" : [] },
// 	// //     "IF" : [
// 	// //       { "data_is" : { "data" : [ "commerce-line-item:type" ], "value" : "product" } },
// 	// //       { "data_is" : {
// 	// //           "data" : [ "commerce-line-item:commerce-product:type" ],
// 	// //           "value" : "order"
// 	// //         }
// 	// //       },
// 	// //       { "entity_has_field" : {
// 	// //           "entity" : [ "commerce-line-item:field-product-display" ],
// 	// //           "field" : "field_serv_loc_city"
// 	// //         }
// 	// //       },
// 	// //       { "data_is" : {
// 	// //           "data" : [ "commerce-line-item:field-product-display:field-serv-loc-city" ],
// 	// //           "value" : "710"
// 	// //         }
// 	// //       },
// 	// //       { "entity_has_field" : {
// 	// //           "entity" : [ "commerce-line-item:field-product-display" ],
// 	// //           "field" : "field_request_serv_cat"
// 	// //         }
// 	// //       },
// 	// //       { "data_is" : {
// 	// //           "data" : [ "commerce-line-item:field-product-display:field-request-serv-cat" ],
// 	// //           "value" : "1445"
// 	// //         }
// 	// //       },
// 	// //       { "entity_has_field" : {
// 	// //           "entity" : [ "commerce-line-item:field-product-display" ],
// 	// //           "field" : "field_date_and_time"
// 	// //         }
// 	// //       }
// 	// //     ],
// 	// //     "DO" : [
// 	// //       { "data_calc" : {
// 	// //           "USING" : {
// 	// //             "input_1" : [
// 	// //               "commerce-line-item:field-product-display:field-date-and-time:duration"
// 	// //             ],
// 	// //             "op" : "\/",
// 	// //             "input_2" : "3600"
// 	// //           },
// 	// //           "PROVIDE" : { "result" : { "hour_amount" : "hour amount" } }
// 	// //         }
// 	// //       },
// 	// //       { "data_calc" : {
// 	// //           "USING" : { "input_1" : [ "hour-amount" ], "op" : "*", "input_2" : "30000" },
// 	// //           "PROVIDE" : { "result" : { "total_time_price" : "total time price" } }
// 	// //         }
// 	// //       },
// 	// //       { "data_calc" : {
// 	// //           "USING" : { "input_1" : [ "total-time-price" ], "op" : "*", "input_2" : "2.1" },
// 	// //           "PROVIDE" : { "result" : { "service_rate_price" : "service rate price" } }
// 	// //         }
// 	// //       },
// 	// //       { "commerce_line_item_unit_price_amount" : {
// 	// //           "commerce_line_item" : [ "commerce_line_item" ],
// 	// //           "amount" : [ "service-rate-price" ],
// 	// //           "component_name" : "base_price",
// 	// //           "round_mode" : "1"
// 	// //         }
// 	// //       },
// 	// //       { "commerce_line_item_unit_price_add" : {
// 	// //           "commerce_line_item" : [ "commerce_line_item" ],
// 	// //           "amount" : "10000",
// 	// //           "component_name" : "admin_rate",
// 	// //           "round_mode" : "0"
// 	// //         }
// 	// //       },
// 	// //       { "commerce_line_item_unit_price_add" : {
// 	// //           "commerce_line_item" : [ "commerce_line_item" ],
// 	// //           "amount" : "9000",
// 	// //           "component_name" : "transfer_rate",
// 	// //           "round_mode" : "0"
// 	// //         }
// 	// //       }
// 	// //     ]
// 	// //   }
// 	// // }';
// 	// //   $configs['rules_pricing_rule_indonesia_jakarta_it_consultant_'] = rules_import($rule);
// 	// //   return $configs;
// 	// }



// }