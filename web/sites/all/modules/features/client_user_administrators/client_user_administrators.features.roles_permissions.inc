<?php
/**
 * @file
 * client_user_administrators.features.roles_permissions.inc
 */

/**
 * Implements hook_default_roles_permissions().
 */
function client_user_administrators_default_roles_permissions() {
  $roles = array();

  // Exported role: client user administrator
  $roles['client user administrator'] = array(
    'name' => 'client user administrator',
    'weight' => 5,
    'machine_name' => 'client_user_administrator',
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access site in maintenance mode' => TRUE,
      'access user profiles' => TRUE,
      'administer users' => TRUE,
      'assign client administrator role' => TRUE,
      'assign client user administrator role' => TRUE,
      'assign editor role' => TRUE,
      'change own username' => TRUE,
      'view the administration theme' => TRUE,
    ),
  );

  return $roles;
}
