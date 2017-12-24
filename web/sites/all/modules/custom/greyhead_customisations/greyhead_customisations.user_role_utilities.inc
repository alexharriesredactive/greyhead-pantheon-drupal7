<?php

/**
 * @file
 * greyhead_customisations.user_role_utilities.inc
 *
 * Provides a couple of utility functions to make it easier to check if a user
 * has a particular role.
 *
 * This file might also be included in other Greyhead Projects of Doooom to
 * allow us to avoid a dependency on this module.
 */

if (!function_exists('get_role_by_name')) {
  /**
   * Gets a user role by name.
   *
   * @param string $name The role's name.
   *
   * @return mixed The role ID if found, or FALSE if not.
   */
  function get_role_by_name($name) {
    $roles = user_roles();
    return array_search($name, $roles);
  }
}

if (!function_exists('user_has_role_by_name')) {
  /**
   * Checks if a user has a role by role name.
   *
   * @param string      $role_name
   *   A role name.
   *
   * @param object|null $account
   *   (optional) A user account. Defaults to the current user.
   *
   * @return bool
   *   TRUE if the user has the role, or FALSE if not.
   */
  function user_has_role_by_name($role_name, $account = NULL) {
    if (!$account) {
      $account = $GLOBALS['user'];
    }

    // Get the role ID.
    if (($rid = get_role_by_name($role_name)) && ($rid !== FALSE)) {
      return isset($account->roles[$rid]);
    }
  }
}

