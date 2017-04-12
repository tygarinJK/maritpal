<?php

/**
 * Index all Currency Codes objects
 *
 * @return array
 */
function _commerce_currency_index() {
	return (object)array(
		'commerce_currencies' => commerce_currencies(TRUE)
	);
}

/**
 * Retrive Currency Code object
 *
 * @param string $cc
 * 	Currency code
 *
 * @return object
 */
function _commerce_currency_retrieve($cc) {
	return (object)array(
		'commerce_currency' => commerce_currency_load(strtoupper($cc))
	);
}