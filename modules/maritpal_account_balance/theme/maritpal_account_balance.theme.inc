<?php
/**
 * @file
 * Defines the themeable functions for this module.
 */

/**
 * Declares theme_maritpal_account_balance_purchase_button.
 *
 * The reason why we put this in a theme function is because we want
 * developpers to be able to theme their own modal. For that you need to add
 * a class to the link.
 */
function theme_maritpal_account_balance_purchase_button($variables) {
  ctools_include('ajax');
  ctools_include('modal');
  ctools_modal_add_js();

  $button_text = $variables['button_text'];
  $path = $variables['path'];
  return ctools_modal_text_button($button_text, $path, '');
}