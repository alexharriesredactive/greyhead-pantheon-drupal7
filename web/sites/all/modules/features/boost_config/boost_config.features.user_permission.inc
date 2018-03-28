<?php
/**
 * @file
 * boost_config.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function boost_config_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'boost flush pages'.
  $permissions['boost flush pages'] = array(
    'name' => 'boost flush pages',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'boost',
  );

  return $permissions;
}
