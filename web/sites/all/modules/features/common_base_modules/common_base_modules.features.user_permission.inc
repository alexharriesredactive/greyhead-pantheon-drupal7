<?php
/**
 * @file
 * common_base_modules.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function common_base_modules_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'use text format full_html'.
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'client administrator' => 'client administrator',
      'editor' => 'editor',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
