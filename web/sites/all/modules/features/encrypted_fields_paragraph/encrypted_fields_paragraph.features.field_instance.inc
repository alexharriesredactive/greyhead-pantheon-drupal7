<?php
/**
 * @file
 * encrypted_fields_paragraph.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function encrypted_fields_paragraph_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'paragraphs_item-encrypted_fields-field_encrypted_fields_data'.
  $field_instances['paragraphs_item-encrypted_fields-field_encrypted_fields_data'] = array(
    'bundle' => 'encrypted_fields',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Information you store in encrypted fields is stored encrypted in the database and is only accessible to users with view permission, granted by the ACLs you specify for each field, and the site-wide ACLs.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'greyhead_encrypted_field',
        'settings' => array(),
        'type' => 'greyhead_encrypted_field_formatter',
        'weight' => 1,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_encrypted_fields_data',
    'label' => 'Encrypted data',
    'required' => 0,
    'settings' => array(
      'default_acls' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'greyhead_encrypted_field',
      'settings' => array(),
      'type' => 'greyhead_encrypted_field_widget',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-encrypted_fields-field_encrypted_fields_group'.
  $field_instances['paragraphs_item-encrypted_fields-field_encrypted_fields_group'] = array(
    'bundle' => 'encrypted_fields',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the name of this group of private information, for example "Test account usernames and passwords".',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'fences_wrapper' => 'h3',
    'field_name' => 'field_encrypted_fields_group',
    'label' => 'Group name',
    'required' => 1,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'full_html' => 'full_html',
          'plain_text' => 'plain_text',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'full_html' => array(
              'weight' => -10,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
          ),
        ),
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Encrypted data');
  t('Enter the name of this group of private information, for example "Test account usernames and passwords".');
  t('Group name');
  t('Information you store in encrypted fields is stored encrypted in the database and is only accessible to users with view permission, granted by the ACLs you specify for each field, and the site-wide ACLs.');

  return $field_instances;
}
