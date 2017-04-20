<?php
/**
 * @file
 * Defines the action set that admins can execute in e.g. VBO.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function maritpal_account_balance_default_rules_configuration() {
  $items['maritpal_account_balance_deposit_to_account'] = entity_import('rules_config', '{ "maritpal_account_balance_deposit_to_account" : {
    "LABEL" : "Deposit money to user account",
    "PLUGIN" : "action set",
    "OWNER" : "rules",
    "REQUIRES" : [ "maritpal_account_balance" ],
    "USES VARIABLES" : {
      "user" : { "label" : "User", "type" : "user" },
      "amount" : { "label" : "Amount", "type" : "decimal" }
    },
    "ACTION SET" : [
      { "maritpal_account_balance_deposit_to_account" : { "user" : [ "user" ], "amount" : [ "amount" ] } }
    ]
  }
}'
  );
  return $items;
}
