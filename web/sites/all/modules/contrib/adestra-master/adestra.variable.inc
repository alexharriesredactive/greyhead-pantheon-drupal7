<?php

/**
 * @file
 * Adestra Variable Implementations.
 */

/**
 * Implements hook_variable_info().
 */
function adestra_variable_info($options) {
  $variables = array();

  $variables['adestra_default_authentication_[authentication_part]'] = array(
    'title' => t('Adestra authentication details', array(), $options),
    'type' => 'adestra_authentication',
    'group' => 'adestra',
    'required' => TRUE,
    'default' => array(
      'endpoint' => 'https://new.adestra.com/api/xmlrpc',
      'account' => '',
      'username' => '',
      'password' => '',
    ),
  );
  $variables['adestra_default_table_id'] = array(
    'title' => t('Default Table ID', array(), $options),
    'type' => 'number',
    'group' => 'adestra',
    'required' => TRUE,
    'default' => '1',
  );
  $variables['adestra_default_debug'] = array(
    'title' => t('Debug Mode', array(), $options),
    'type' => 'enable',
    'group' => 'adestra',
    'required' => TRUE,
    'default' => 0,
    'description' => t('When ticked messages will be displayed to users with the %debug_adestra permission.', array('%debug_adestra' => t('Debug Adestra')), $options),
  );

  return $variables;
}


/**
 * Implements hook_variable_type_info().
 */
function adestra_variable_type_info() {

  $types = array();
  // Array of values
  $types['adestra_authentication'] = array(
    'title' => t('Adestra Authentication Details'),
    'multiple' => array(
      'endpoint' => t('XML-RPC End Point'),
      'account' => t('Account'),
      'username' => t('Username'),
      'password' => t('Password'),
    ),
    'build callback' => 'adestra_build_adestra_authentication',
    'type' => 'multiple',
  );

  return $types;
}

/**
 * Implements hook_variable_group_info().
 */
function adestra_variable_group_info() {
  $groups = array();

  $groups['adestra'] = array(
    'title' => t('Adestra Settings'),
    'description' => t('Stores Adestra Information'),
    'access' => 'administer adestra configuration',
    'path' => array('admin/config/services/adestra'),
  );

  return $groups;
}

/**
 * Build multiple authentication variable
 */
function adestra_build_adestra_authentication($variable, $options = array()) {
  $name = str_replace('[authentication_part]', '', $variable['name']);
  // For mail text, children have different types
  $variable['children'][$name . 'endpoint']= array(
    'type' => 'string',
    'group' => 'adestra_authentication',
    'required' => TRUE,
    'default' => 'https://new.adestra.com/api/xmlrpc',
    'description' => t('The location of the MessageFocus API XMLRPC End Point.', array(), $options),
  );
  $variable['children'][$name . 'account'] = array(
    'type' => 'string',
    'group' => 'adestra_authentication',
    'required' => TRUE,
    'default' => '',
    'description' => t('The Adestra account ID.', array(), $options),
  );
  $variable['children'][$name . 'username'] = array(
    'type' => 'string',
    'group' => 'adestra_authentication',
    'required' => TRUE,
    'default' => '',
    'description' => t('The Adestra API account username.', array(), $options),
  );
  $variable['children'][$name . 'password'] = array(
    'type' => 'string',
    'group' => 'adestra_authentication',
    'required' => TRUE,
    'default' => '',
    'description' => t('The Adestra API account password.', array(), $options),
  );
  $variable = variable_build_multiple($variable, $options);
  return $variable;
}