<?php
/**
 * @file
 * advanced_aggregation_settings.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function advanced_aggregation_settings_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'bypass advanced aggregation'.
  $permissions['bypass advanced aggregation'] = array(
    'name' => 'bypass advanced aggregation',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'advagg',
  );

  return $permissions;
}
