<?php
/**
 * @file
 * client_user_administrators.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function client_user_administrators_user_default_roles() {
  $roles = array();

  // Exported role: client user administrator.
  $roles['client user administrator'] = array(
    'name' => 'client user administrator',
    'weight' => 5,
    'machine_name' => 'client_user_administrator',
  );

  return $roles;
}
