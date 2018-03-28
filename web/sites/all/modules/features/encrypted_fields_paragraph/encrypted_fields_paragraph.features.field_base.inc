<?php
/**
 * @file
 * encrypted_fields_paragraph.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function encrypted_fields_paragraph_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_encrypted_fields_data'.
  $field_bases['field_encrypted_fields_data'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_encrypted_fields_data',
    'global_block_settings' => 1,
    'indexes' => array(),
    'locked' => 0,
    'module' => 'greyhead_encrypted_field',
    'settings' => array(
      'global_acls' => 'user-type:creator::permissions:full control
role-drupal:administrator::permissions:edit permissions,view title,view value',
    ),
    'translatable' => 0,
    'type' => 'greyhead_encrypted_field',
  );

  // Exported field_base: 'field_encrypted_fields_group'.
  $field_bases['field_encrypted_fields_group'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_encrypted_fields_group',
    'global_block_settings' => 1,
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  return $field_bases;
}
