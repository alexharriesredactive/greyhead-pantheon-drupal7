<?php
/**
 * @file
 * common_user_roles.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function common_user_roles_user_default_roles() {
  $roles = array();

  // Exported role: client administrator.
  $roles['client administrator'] = array(
    'name' => 'client administrator',
    'weight' => 3,
    'machine_name' => 'client_administrator',
  );

  // Exported role: editor.
  $roles['editor'] = array(
    'name' => 'editor',
    'weight' => 4,
    'machine_name' => 'editor',
  );

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 2,
    'machine_name' => 'administrator',
  );

  return $roles;
}
