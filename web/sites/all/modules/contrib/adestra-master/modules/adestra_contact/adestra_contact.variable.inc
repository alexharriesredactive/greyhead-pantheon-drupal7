<?php

/**
 * @file
 * Adestra Contact Variable Implementations.
 */

/**
 * Implements hook_variable_info().
 */
function adestra_contact_variable_info($options) {
  $variables = array();

  $variables['adestra_contact_attribute_uid'] = array(
    'title' => t('Attribute to store: Drupal ID Attribute', array(), $options),
    'type' => 'string',
    'group' => 'adestra',
    'description' => t('If set, on register or user edit a users drupal id will be stored in this attribute.', array(), $options)
  );
  $variables['adestra_contact_attribute_username'] = array(
    'title' => t('Attribute to store: Drupal Username', array(), $options),
    'type' => 'string',
    'group' => 'adestra',
    'default' => '',
    'description' => t('If set, on register or user edit a users username will be stored in this attribute.', array(), $options)
  );
  $variables['adestra_contact_lists_register_optional'] = array(
    'title' => t('Contact Lists: Option to subscribe on registration', array(), $options),
    'type' => 'text',
    'group' => 'adestra',
    'default' => '',
    'description' => t('<strong>Key-value pairs MUST be specified as "safe_key|Some readable option"</strong>. Use of only alphanumeric characters and underscores is recommended in keys. One option per line. Option groups may be specified with &lt;Group Name&gt;. &lt;&gt; can be used to insert items at the root of the menu after specifying a group.', array(), $options),
  );
  $variables['adestra_contact_lists_register_optional_defaults'] = array(
    'title' => t('Contact Lists: Option to subscribe on registration defaults', array(), $options),
    'type' => 'string',
    'group' => 'adestra',
    'default' => '',
    'description' => t('Pipe delimited list ids ticked by default', array(), $options),
  );
  $variables['adestra_contact_lists_register_optional_weight'] = array(
    'title' => t('Contact Lists: Option to subscribe on registration weight', array(), $options),
    'type' => 'select_number',
    'options' => range(-10, 10),
    'default' => 0,
    'group' => 'adestra',
    'description' => t('Controls the weight of the preferences fieldset on registration', array(), $options),
  );
  $variables['adestra_contact_lists_register_hidden'] = array(
    'title' => t('Contact Lists: Subscribe on registration', array(), $options),
    'type' => 'string',
    'group' => 'adestra',
    'default' => '',
    'description' => t('Pipe delimited list ids to subscribe customer to automatically on registration', array(), $options),
  );
  $variables['adestra_contact_lists_editable'] = array(
    'title' => t('Contact Lists: Editable subscriptions', array(), $options),
    'type' => 'text',
    'group' => 'adestra',
    'default' => '',
    'description' => t('<strong>Key-value pairs MUST be specified as "safe_key|Some readable option"</strong>. Use of only alphanumeric characters and underscores is recommended in keys. One option per line. Option groups may be specified with &lt;Group Name&gt;. &lt;&gt; can be used to insert items at the root of the menu after specifying a group.', array(), $options),
  );
  return $variables;

}