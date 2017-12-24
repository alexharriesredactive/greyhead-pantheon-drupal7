<?php
/**
 * @file
 * encrypted_fields_paragraph.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function encrypted_fields_paragraph_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_paragraphs_info().
 */
function encrypted_fields_paragraph_paragraphs_info() {
  $items = array(
    'encrypted_fields' => array(
      'name' => 'Encrypted Fields',
      'bundle' => 'encrypted_fields',
      'locked' => '1',
    ),
  );
  return $items;
}
