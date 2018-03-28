<?php

/**
 * @file
 * greyhead_encrypted_field.api.inc
 *
 * Functions provided by the greyhead_encrypted_field module.
 */

/**
 * Get the default ACL entries for an encrypted field.
 *
 * ACL entries are specified as: entity:[entity type]|name:[entity identifier]|permissions:[permission 1],[permission 2] (etc)
 *
 * By default, the following entity types are handled:
 *
 *  - user-type: types of user, including:
 *    - creator: the user who created the field and its parent element, e.g.
 *      "user-type:creator"
 *    - root: the primary administration account (UID 1), e.g. "user-type:root"
 *
 *  - user-uid: a user account's UID, e.g. "user-uid:123"
 *
 *  - user-name: a user account's login name, e.g. "user-name:alexharries"
 *
 *  - role-drupal: one of the drupal-provided roles:
 *      anonymous user
 *      authenticated user
 *      administrator
 *
 *    e.g. "role-drupal:authenticated user"
 *
 *  - role-custom: a user role
 */
function greyhead_encrypted_field_acl_default_acls($context = GEF_ACL_CONTEXT_FIELD, $reload = FALSE, $reset_to_defaults = FALSE) {
  // Get the name for this type of default.
  $cache_id = 'greyhead_encrypted_field_default_acls_' . $context;

  $default_acls = &drupal_static($cache_id);

  if (is_null($default_acls) || $reload || $reset_to_defaults) {
    $default_acls = variable_get($cache_id);

    if (is_null($default_acls) || $reset_to_defaults) {
      $default_acls_array = array();

      // Implement a hook_greyhead_encrypted_field_acl_default_acls_alter.
      drupal_alter($cache_id, $default_acls_array);

      // Convert back into strings.
      $default_acls = greyhead_encrypted_field_acl_acls_to_strings($default_acls_array);

      // Set the default ACLs in the variable table.
      variable_set($cache_id, $default_acls);
    }
  }

  gefdpm(__FUNCTION__);
  gefdpm($cache_id);
  gefdpm($default_acls);

  return $default_acls;
}

/**
 * Implements hook_greyhead_encrypted_field_default_acls_sitewide_alter.
 *
 * @param $default_acls
 */
function greyhead_encrypted_field_greyhead_encrypted_field_default_acls_sitewide_alter(&$default_acls) {
  // Allow the field creator full control over the field.
  $acl = new greyhead_acl(GEF_USER_TYPE, GEF_USER_TYPE_CREATOR, GEF_PERMISSION_FULL_CONTROL);
  $default_acls[$acl->to_string()] = $acl;

  // On fields attached to nodes, allow users with permission to edit the node
  // to edit the encrypted field labels and values.
  $acl = new greyhead_acl(GEF_PARENT_NODE, GEF_NODE_EDIT, array(
    GEF_PERMISSION_EDIT_TITLE,
    GEF_PERMISSION_EDIT_VALUE,
  ));
  $default_acls[$acl->to_string()] = $acl;

  // On fields attached to nodes, allow users with permission to edit the node
  // to edit the encrypted field labels and values.
  $acl = new greyhead_acl(GEF_PARENT_NODE, GEF_NODE_VIEW, array(
    GEF_PERMISSION_VIEW_TITLE,
    GEF_PERMISSION_VIEW_VALUE,
  ));
  $default_acls[$acl->to_string()] = $acl;

  // Allow site administrators to view permissions and take control over the
  // field if they choose.
  $acl = new greyhead_acl(GEF_ROLE_DRUPAL, GEF_ROLE_DRUPAL_ADMINISTRATOR, array(
    GEF_PERMISSION_EDIT_PERMISSIONS,
    GEF_PERMISSION_VIEW_TITLE,
    GEF_PERMISSION_VIEW_VALUE,
  ));
  $default_acls[$acl->to_string()] = $acl;
}

/**
 * Implements hook_greyhead_encrypted_field_default_acls_field_alter.
 *
 * @param $default_acls
 */
function greyhead_encrypted_field_greyhead_encrypted_field_default_acls_field_alter(&$default_acls) {
}

/**
 * Convert an array of ACLs to a textarea-friendly chunk of text.
 *
 * @param array $acls
 *
 * @return string
 */
function greyhead_encrypted_field_acl_acls_to_strings(array $acls) {
  $acl_strings = array();

  if (is_array($acls) && !empty($acls)) {
    foreach ($acls as $acl) {
      $acl_strings[] = $acl->to_string();
    }
  }

  return implode(GEF_PERMISSION_LINE_BREAK, $acl_strings);
}

/**
 * Convert an ACL textfield into an array of ACLs.
 *
 * @param $textfield
 *
 * @return array
 */
function greyhead_encrypted_field_acl_textfield_to_acls($textfield) {
  // Trim the data and normalise the line endings.
  $textfield = trim(greyhead_customisations_normalise_line_endings($textfield));

  // Initialise an array of ACL entities.
  $acls = array();

  // Do we have any ACL strings?
  if (!empty($textfield)) {
    // Break the text into ACLs.
    $acl_strings = explode(GEF_PERMISSION_LINE_BREAK, $textfield);

    // Trim each ACL as well.
    foreach ($acl_strings as $acl_string) {
      $acls[$acl_string] = greyhead_encrypted_field_acl_string_to_acl($acl_string);
    }
  }

  return $acls;
}

/**
 * Break an ACL string into an ACL object.
 *
 * For example, the ACL string:
 *
 * 'user-type:creator::permissions:view value,view title,edit value,edit title'
 *
 * ... will return an array such as:
 *
 *  array(
 *    'entity-type' => array(0 => 'user-type'),
 *    'user-type' => array(0 => 'creator'),
 *    'permissions' => array(
 *      0 => 'view value',
 *      1 => 'view title',
 *      2 => 'edit value',
 *      3 => 'edit title',
 *    ),
 *  );
 *
 * @param $acl_string
 *
 * @return array|bool
 */
function greyhead_encrypted_field_acl_string_to_acl($acl_string) {
  // Split the ACL string by its main parts, which will give us two
  // sections - the entity type and entity identifier, and the permissions
  // granted to that entity.
  $acl_components = explode(GEF_PERMISSIONS_PART_SEPARATOR, $acl_string);
  gefdpm(__FUNCTION__);
  gefdpm($acl_components);
  gefdpm($acl_string);

  // The first part will be the entity type and ID.
  list($entity_type, $entity_id) = explode(GEF_PERMISSIONS_KEY_VALUE_SEPARATOR, $acl_components[0], 2);

  // The second part will be the permission(s).
  list($permission_label, $permissions_string) = explode(GEF_PERMISSIONS_KEY_VALUE_SEPARATOR, $acl_components[1], 2);

  return new greyhead_acl($entity_type, $entity_id, $permissions_string);
}

/**
 * Check if a list of ACLs grant a permission to a user.
 *
 * To check whether a user has a permission granted by a list of ACLs, we first
 * get a list of permissions which also implicitly grant $permission, which
 * allows us to return TRUE for example if we've been asked if the user can view
 * a value, and the user only has 'full access'.
 *
 * We then loop through each the access control list object in $acls
 * to determine if that ACL provides $permission, or any permissions which we
 * have in our list of implicit grants.
 *
 * If a particular ACL provides a matching permission, we then query the access
 * callback for the ACL's object type to see if the user is a member of that
 * entity type, for example a Drupal role.
 *
 * $entity and $entity_type provide context for us for ACLs which are based on
 * a property of the entity to which the ACL's field is attached. For example,
 * we might want to grant full access to the user who owns (i.e. created) the
 * node to which an encrypted field is attached.
 *
 * ---
 *
 * For example, we might ask:
 *
 * greyhead_encrypted_field_acl_user_access('edit value', $acls, node_load(123), 'node', $user);
 *
 * Which we can break down to:
 *
 * 'edit value': the permission to edit an encrypted field's value.
 *
 * $acls: for the sake of this example, this is an array containing two access
 * control lists:
 *
 *  - full control over the encrypted field for the creator of the field's
 *    parent entity (node 123).
 *
 *  - view access to the field's label and value for logged-in ("authorized")
 *    users.
 *
 * A node object, node 123, owned by uid 567.
 *
 * 'node' is the entity type.
 *
 * $user is the user account, with a uid of 567.
 *
 * ---
 *
 * In the example above, the 'edit value' permission is also granted by the
 * 'full control' permission.
 *
 * In the ACLs provided, one of them provides the 'full control' permission
 * for the creator of the $entity, which is node 123.
 *
 * Node 123's owner is uid 567, which we pass to the access callback along with
 * the $user's uid, and because they match, the user gets access.
 *
 * Yay!
 *
 * ---
 *
 * For a list of actions available,
 * @see greyhead_encrypted_field_acl_permissions_available().
 *
 * @param string $permission           The permission being checked, for example
 *                                     'edit label' or 'view value', etc.
 *
 * @param array       $acls            An array of ACLs where the key is the ACL
 *                                     string, and the value is a greyhead_acl
 *                                     object.
 *
 * @param mixed|NULL  $entity          A valid Drupal entity, such as a node,
 *                                     user, etc. This entity's owner uid, if
 *                                     set, is passed in to the access callback
 *                                     for each ACL as the entity owner.
 *
 * @param string|NULL $entity_type     The type of entity to which we are
 *                                     checking $account's access, if available.
 *
 * @param null        $account
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_user_access($permission, $acls, $entity = NULL, $entity_type = NULL, $account = NULL) {
  // Create our access result, defaulting to no access.
  $access = FALSE;

  // Get the user account if not already provided.
  if (is_null($account)) {
    $account = $GLOBALS['user'];
  }

  gefdpm(__FUNCTION__);
  gefdpm($entity);
  gefdpm($entity_type);
  gefdpm($permission);
  gefdpm($account);

  // Check if the user is a super-duper admin.
  $access = $access || greyhead_encrypted_field_acl_user_is_admin($account, $entity_type);

  // Check that the requested permission is a valid permission name.
  if (!$access && greyhead_encrypted_field_acl_valid_permission($permission)) {
    // It's valid. Now, we need to see if any other permissions also grant this
    // permission. For example, "edit value" has the implicit permission "view
    // value".
    $permissions_which_also_grant_this_permission = greyhead_encrypted_field_permission_implied_by_permissions($permission);

    // Create an array with all the permissions which will return a TRUE.
    $all_possible_permissions = $permissions_which_also_grant_this_permission;
    $all_possible_permissions[] = $permission;

    // Loop through each ACL and check whether any of them define any of
    // $all_possible_permissions.
    foreach ($acls as $acl) {
      // Is it a valid ACL?
      if ($acl->validate()) {
        // Yes, its entity type and ID, and permissions, are valid.

        // Does this ACL define one or more of the permissions we're looking
        // for?
        $permissions_defined_by_acl = $acl->defines_permissions($all_possible_permissions);

        if (!empty($permissions_defined_by_acl)) {
          // Yes - this ACL entry provides the permission we're looking for. Check
          // whether the user is a member of the ACL's entity type. If they are,
          // then we can return TRUE.

          // Load the entity type info, which tells us the access callback.
          $acl_entity_type_info = greyhead_encrypted_field_acl_entity_type_info($acl->entity_type);

          // Determine the access function name.
          $access_callback_function = $acl_entity_type_info['access callback'];

          if (function_exists($access_callback_function)) {
            // Assemble the $options array.
            $options = array(
              'property_id' => $acl->entity_id,
              'uid' => $account->uid,
//              'field_instance' => $instance,
              'entity_uid' => isset($entity->uid) ? $entity->uid : NULL,
              'entity' => $entity,
            );

            gefdpm(__FUNCTION__ . ' - access check: callback function: ' . $access_callback_function);
            gefdpm($options, '$options');

            // At last, we know everything we need to - check if the user has
            // access as a result of this ACL.
            $callback_response = call_user_func($access_callback_function, $options);
            gefdpm($callback_response, '$callback_response');

            $access = $access || $callback_response;
            gefdpm($access, '$access');
          }
        }
      }

      // If permission has been granted, stop processing since we don't need to
      // go any further.
      if ($access) {
        break;
      }
    }

    // Implement a hook_greyhead_encrypted_field_acl_user_access_alter.
    drupal_alter('greyhead_encrypted_field_acl_user_access', $access, $permission, $acls, $entity, $entity_type, $account);
  }

  return $access;
}

/**
 * Implements hook_greyhead_encrypted_field_acl_user_access_alter.
 *
 * This function will check whether an anonymous user is being granted a
 * permission which allows them to view (i.e. decrypt) an encrypted title or
 * value.
 *
 * @param $access
 * @param $permission
 * @param $acls
 * @param $entity
 * @param $entity_type
 * @param $account
 */
function greyhead_encrypted_field_greyhead_encrypted_field_acl_user_access_alter(&$access, $permission, $acls, $entity, $entity_type, $account) {
  // We only need to check if permission has already been granted, and the
  // $account user is anonymous.
  if ($access && !$account->uid) {
    // Skip if anonymous decryption is explicitly permitted.
    if (!variable_get('greyhead_encrypted_field_allow_anonymous_decryption')) {
      // Get the list of permissions implied by $permission.
      $implicit_permissions = greyhead_encrypted_field_permission_implies_permissions($permission);

      // Get the list of dangerous permissions.
      $dangerous_permissions = greyhead_encrypted_field_dangerous_permissions();

      // If any of $dangerous_permissions appear in $implicit_permissions, then
      // this access check could potentially grant an anonymous user access to
      // sensitive information, and we will block that attempt and log a
      // warning.
      $matches = array_intersect($implicit_permissions, $dangerous_permissions);

      if (!empty($matches)) {
        // We've found one or more dangerous permissions which are granted by
        // this access check, so we will block this permission and log a
        // warning.
        $access = FALSE;

        $message = 'An unsafe permission granting anonymous access to encrypted data was blocked.<br /> '
          . 'The permission is %permission, which implies the following permissions: %implicit_permissions.<br /> '
          . 'The following permissions are considered dangerous: %dangerous_permissions. <br />'
          . 'If you want to disable this security check (but you shouldn\'t because it\'s there to protect your data), see !settings_link.';

        greyhead_encrypted_field_error($message, array(
          '%permission' => $permission,
          '%implicit_permissions' => implode(', ', $implicit_permissions),
          '%dangerous_permissions' => implode(', ', $dangerous_permissions),
          '!settings_link' => l(t('the Greyhead Encrypted Field settings page'), 'admin/config/system/encrypt/encryptedfield'),
        ), WATCHDOG_WARNING, TRUE);
      }
    }
  }
}

/**
 * Gets a list of permissions which would allow anonymous users access to
 * view (i.e. decrypt) encrypted field labels or values.
 *
 * @return array
 */
function greyhead_encrypted_field_dangerous_permissions() {
  $dangerous_permissions = &drupal_static(__FUNCTION__);

  if (is_null($dangerous_permissions)) {
    $dangerous_permissions = array(
      GEF_PERMISSION_FULL_CONTROL,
      GEF_PERMISSION_EDIT_PERMISSIONS,
      GEF_PERMISSION_VIEW_VALUE,
      GEF_PERMISSION_EDIT_TITLE,
      GEF_PERMISSION_EDIT_VALUE,
    );

    // Implement a hook_greyhead_encrypted_field_dangerous_permissions_alter.
    drupal_alter('greyhead_encrypted_field_dangerous_permissions', $dangerous_permissions);
  }

  return $dangerous_permissions;
}

/**
 * Get a list of entities which could allow anonymous access.
 *
 * This function returns a list of entity types and IDs which include
 * anonymous users, for example "user-type:everyone".
 *
 * If an ACL specifies one of these entity type-and-ID combinations, it should
 * be checked for any permission grants which are included in
 * greyhead_encrypted_field_dangerous_permissions().
 *
 * Alternatively, this information can be used in a
 * hook_greyhead_encrypted_field_acl_user_access_alter, by looking at the
 * permission string's implicit permissions and checking if any are present in
 * greyhead_encrypted_field_dangerous_permissions().
 *
 * @return array
 */
function greyhead_encrypted_field_dangerous_entities() {
  $dangerous_entities = &drupal_static(__FUNCTION__);

  if (is_null($dangerous_entities)) {
    $dangerous_entities = array(
      GEF_ROLE_DRUPAL => array(GEF_ROLE_DRUPAL_ANONYMOUS),
      GEF_USER_TYPE => array(GEF_USER_TYPE_EVERYONE),
    );

    // Implement a hook_greyhead_encrypted_field_dangerous_entities_alter.
    drupal_alter('greyhead_encrypted_field_dangerous_entities', $dangerous_entities);
  }

  return $dangerous_entities;
}

/**
 * Checks whether $permission also grants $implied_permission to a user.
 *
 * For example, we might want to know if 'full control' implies 'edit value'
 * (it does), or whether 'edit permissions' implies 'view value' (it doesn't,
 * but the user can use their 'edit permissions' grant to give themselves
 * permission to edit the field's value).
 *
 * @param string $permission         The permission string we know, e.g. 'edit value'.
 *
 * @param string $implied_permission The permission string which may or may not
 *                                   be granted by $permission.
 *
 * @return bool TRUE if $implied_permission is granted by $permission, or FALSE
 *              if not.
 */
function greyhead_encrypted_field_permission_implies_permission($permission, $implied_permission) {
  $permission_maps = &drupal_static(__FUNCTION__, array());

  if (!array_key_exists($permission, $permission_maps)) {
    $permission_maps[$permission] = greyhead_encrypted_field_permission_implies_permissions($permission, FALSE);
  }

  return (bool) in_array($implied_permission, $permission_maps[$permission], TRUE);
}

/**
 * Get a list of any other permissions which are granted by $permission.
 *
 * For example, the $permission "edit value" implies "view value", because
 * otherwise you wouldn't be able to view the field's value when editing it.
 *
 * @param      $permission
 *
 * @param bool $include_in_results Whether to include $permission in the array
 *                                 of results.
 *
 * @return mixed
 */
function greyhead_encrypted_field_permission_implies_permissions($permission, $include_in_results = TRUE) {
  if (greyhead_encrypted_field_acl_valid_permission($permission)
    && ($permission_data = greyhead_encrypted_field_acl_permissions_available($permission))
  ) {
    $implicit_permissions = array();

    if (array_key_exists('implicit permissions', $permission_data)) {
      $implicit_permissions = $permission_data['implicit permissions'];
    }

    // Have we been asked to include the $permission string in the implicit
    // permissions? This makes it easier to create a comprehensive list without
    // faffing about with merging arrays, etc.
    if ($include_in_results) {
      $implicit_permissions[] = $permission;
    }

    return $implicit_permissions;
  }
}

/**
 * Get a list of any permissions which implicitly grant $permission.
 *
 * For example, if $permission is "view value", then the permissions "edit
 * value", "edit permissions" and "full control" would be returned, because they
 * all imply access to view the value of an encrypted field.
 *
 * @param      $permission
 *
 * @param bool $include_in_results
 *
 * @return bool
 */
function greyhead_encrypted_field_permission_implied_by_permissions($permission, $include_in_results = TRUE) {
  if (greyhead_encrypted_field_acl_valid_permission($permission)
    && ($permission_data = greyhead_encrypted_field_acl_permissions_available($permission))
  ) {
    $implied_by_permissions = array();

    if (array_key_exists('also granted by', $permission_data)) {
      $implied_by_permissions = $permission_data['also granted by'];
    }

    // Have we been asked to include the $permission string in the implied
    // permissions? This makes it easier to create a comprehensive list without
    // faffing about with merging arrays, etc.
    if ($include_in_results) {
      $implied_by_permissions[] = $permission;
    }

    return $implied_by_permissions;
  }
}

/**
 * Check that the requested permission is a valid permission name.
 *
 * @param string     $permission      The permission to check for.
 *
 * @param array|NULL $all_permissions An array of existing permissions. If NULL,
 *                                    the existing permissions are retrieved
 *                                    from
 *                                    greyhead_encrypted_field_acl_permissions_available().
 *
 *                                    This function is also called from
 *                                    greyhead_encrypted_field_acl_permissions_available(),
 *                                    which provides a pre-populated $permissions
 *                                    array to prevent recursion.
 *
 *                                    greyhead_encrypted_field_acl_valid_permission
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_valid_permission($permission, $all_permissions = NULL) {
  if (is_null($all_permissions)) {
    $all_permissions = greyhead_encrypted_field_acl_permissions_available();
  }

  $valid_permission = array_key_exists($permission, $all_permissions);

  if (!$valid_permission) {
    greyhead_encrypted_field_error('Invalid permission %permission passed to %function in %file, line %line.', array(
      '%permission' => $permission,
      '%function' => __FUNCTION__,
      '%file' => __FILE__,
      '%line' => __LINE__,
    ), WATCHDOG_ERROR, TRUE);
  }

  return $valid_permission;
}

/**
 * Gets information on validation and access functions for a type of entity.
 *
 * @param $acl_entity_type
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_entity_type_info($acl_entity_type) {
  // Get the list of access providers.
  $acl_entity_types = greyhead_encrypted_field_acl_entity_types_info();

  return isset($acl_entity_types[$acl_entity_type]) ? $acl_entity_types[$acl_entity_type] : FALSE;
}

/**
 * Checks whether a given entity type - such as "user-uid" - is valid.
 *
 * @param $acl_entity_type
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_entity_type_validate($acl_entity_type) {
  return greyhead_encrypted_field_acl_entity_type_info($acl_entity_type) !== FALSE;
}

/**
 * Gets information on the types of ACL entities which can be assigned access.
 *
 * @TODO: check this documentation isn't out of date.
 *
 * The return value is an array whose keys are the entity type's ACL name, and
 * the value is an array containing:
 *
 *  - module: the name of the module providing the access check
 *
 *  - access callback: the function name which checks whether a user has this
 *    access
 *
 *  - validation callback: the function which checks that an ACL property ID is
 *    valid. This function is called when an encrypted field is submitted and
 *    ACLs are being saved.
 *
 * Callback functions must accept one argument, which is an options array with
 * the following values:
 *
 *  - property_id: the identifier of the property which the user account is to
 *    be checked for, for example a role ID, the user type "authenticated user",
 *    etc.
 *
 *  - uid (for access callbacks): the user ID being checked. Access callbacks
 *    should not assume that a NULL user ID means it should check the current
 *    global user entity.
 *
 *  - field_instance: the current instance of the field. This information is
 *    needed, for example, for checks which need to know the UID which created
 *    the field.
 *
 *  - entity (where applicable): the entity to which the field is attached.
 *
 * (&$item, $delta, &$field, $instance, &$entity)
 *
 * Access callback functions should return TRUE if the user explicitly DOES have
 * the property in property_id; FALSE should be returned if they do not.
 *
 * Validation callbacks should return TRUE if the submitted property_id is valid
 * or FALSE if not.
 */
function greyhead_encrypted_field_acl_entity_types_info() {
  $acl_entity_types = array();

  // Implement a hook_greyhead_encrypted_field_acl_entity_types_info_alter.
  drupal_alter(GEF_MODULE_NAME . '_acl_entity_types_info', $acl_entity_types);

  return $acl_entity_types;
}

/**
 * Implements hook_greyhead_encrypted_field_acl_entity_types_info_alter.
 *
 * @param $acl_entity_types
 */
function greyhead_encrypted_field_greyhead_encrypted_field_acl_entity_types_info_alter(&$acl_entity_types) {
  $acl_entity_types[GEF_USER_TYPE] = array(
    'module' => GEF_MODULE_NAME,
    'access callback' => GEF_MODULE_NAME . '_acl_access_user_type',
    'validation callback' => GEF_MODULE_NAME . '_acl_validate_user_type',
  );

  $acl_entity_types[GEF_USER_UID] = array(
    'module' => GEF_MODULE_NAME,
    'access callback' => GEF_MODULE_NAME . '_acl_access_user_uid',
    'validation callback' => GEF_MODULE_NAME . '_acl_validate_user_uid',
  );

  $acl_entity_types[GEF_USER_NAME] = array(
    'module' => GEF_MODULE_NAME,
    'access callback' => GEF_MODULE_NAME . '_acl_access_user_name',
    'validation callback' => GEF_MODULE_NAME . '_acl_validate_user_name',
  );

  $acl_entity_types[GEF_ROLE_DRUPAL] = array(
    'module' => GEF_MODULE_NAME,
    'access callback' => GEF_MODULE_NAME . '_acl_access_role_drupal',
    'validation callback' => GEF_MODULE_NAME . '_acl_validate_role_drupal',
  );

  $acl_entity_types[GEF_ROLE_CUSTOM] = array(
    'module' => GEF_MODULE_NAME,
    'access callback' => GEF_MODULE_NAME . '_acl_access_role_custom',
    'validation callback' => GEF_MODULE_NAME . '_acl_validate_role_custom',
  );

  /**
   * Access based on this field's parent node permissions (where applicable);
   * for example, all users who can edit the field's parent node can do x, y
   * or z.
   *
   * @examples:
   *
   *          parent-node:edit::permissions:edit title,edit value
   *          parent-node:view::permissions:view title,view value
   */
  $acl_entity_types[GEF_PARENT_NODE] = array(
    'module' => GEF_MODULE_NAME,
    'access callback' => GEF_MODULE_NAME . '_acl_access_parent_node',
    'validation callback' => GEF_MODULE_NAME . '_acl_validate_parent_node',
  );
}

/**
 * Checks whether the current user has administrative control over fields.
 *
 * If the entity type is 'node' and they have 'bypass node access' or
 * 'administer content types' permissions, or if they have 'administer encrypted
 * fields' permission, then this function will return TRUE. Otherwise, it will
 * return FALSE.
 *
 * @param $account
 * @param $entity_type
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_user_is_admin($account, $entity_type = NULL) {
  // Is this a node?
  if (!is_null($entity_type) && ($entity_type == 'node')) {
    // First we'll check if the user has the 'superuser'
    // permissions that node provides. This way administrators
    // will be able to administer the content types.
    if (user_access('bypass node access', $account)) {
      return TRUE;
    }

    if (user_access('administer content types', $account)) {
      return TRUE;
    }
  }

  // Now check if the user has the 'access all encrypted fields' permission.
  if (user_access(GEF_PERMISSION_ACCESS_ALL, $account)) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Gets a list of permissions which define access to encrypted fields.
 *
 * @param string $permission A valid permission name (not title), such as "edit
 *                           title".
 *
 * @return array Either an array of all permissions if $permission wasn't
 *               provided, or a permission array.
 */
function greyhead_encrypted_field_acl_permissions_available($permission = NULL) {
  $permissions = &drupal_static(__FUNCTION__);

  if (is_null($permissions)) {
    $permissions = array();

    // Implement a hook_greyhead_encrypted_field_acl_permissions_available_alter.
    drupal_alter(GEF_MODULE_NAME . '_acl_permissions_available', $permissions);
  }

  // Have we been asked for a specific permission? Validate it first.
  if (!is_null($permission)) {
    if (greyhead_encrypted_field_acl_valid_permission($permission, $permissions)) {
      return $permissions[$permission];
    }

    // If we get here, we've been passed a duff permission name.
    return FALSE;
  }

  return $permissions;
}

/**
 * Implements hook_greyhead_encrypted_field_acl_permissions_available_alter.
 *
 * @param $permissions
 */
function greyhead_encrypted_field_greyhead_encrypted_field_acl_permissions_available_alter(&$permissions) {
  $permissions[GEF_PERMISSION_FULL_CONTROL] = array(
    'title' => t('Full control'),
    'description' => t('This permission allows a user to view and edit an encrypted field and its title, and edit permissions on a field.'),
    'implicit permissions' => array(
      GEF_PERMISSION_EDIT_PERMISSIONS,
      GEF_PERMISSION_VIEW_PERMISSIONS,
      GEF_PERMISSION_EDIT_VALUE,
      GEF_PERMISSION_VIEW_VALUE,
      GEF_PERMISSION_EDIT_TITLE,
      GEF_PERMISSION_VIEW_TITLE,
    ),
    'also granted by' => array(),
  );

  $permissions[GEF_PERMISSION_EDIT_PERMISSIONS] = array(
    'title' => t('Edit an encrypted field\'s permissions'),
    'implicit permissions' => array(
      //  GEF_PERMISSION_FULL_CONTROL, <-- No it doesn't - you can edit the
      //  permissions on a field to gain full access, but you can't access the
      //  whole field just by having permission to edit perms.
    ),
    'also granted by' => array(
      GEF_PERMISSION_FULL_CONTROL,
    ),
  );

  $permissions[GEF_PERMISSION_VIEW_PERMISSIONS] = array(
    'title' => t('View an encrypted field\'s permissions'),
    'implicit permissions' => array(),
    'also granted by' => array(
      GEF_PERMISSION_FULL_CONTROL,
      GEF_PERMISSION_EDIT_PERMISSIONS,
    ),
  );

  $permissions[GEF_PERMISSION_EDIT_VALUE] = array(
    'title' => t('Edit field value'),
    'implicit permissions' => array(
      GEF_PERMISSION_VIEW_VALUE,
    ),
    'also granted by' => array(
      GEF_PERMISSION_FULL_CONTROL,
      GEF_PERMISSION_EDIT_PERMISSIONS,
    ),
  );

  $permissions[GEF_PERMISSION_VIEW_VALUE] = array(
    'title' => t('View field value'),
    'implicit permissions' => array(),
    'also granted by' => array(
      GEF_PERMISSION_FULL_CONTROL,
      GEF_PERMISSION_EDIT_PERMISSIONS,
      GEF_PERMISSION_EDIT_VALUE,
    ),
  );

  $permissions[GEF_PERMISSION_EDIT_TITLE] = array(
    'title' => t('Edit field title'),
    'implicit permissions' => array(
      GEF_PERMISSION_VIEW_TITLE,
    ),
    'also granted by' => array(
      GEF_PERMISSION_FULL_CONTROL,
      GEF_PERMISSION_EDIT_PERMISSIONS,
    ),
  );

  $permissions[GEF_PERMISSION_VIEW_TITLE] = array(
    'title' => t('View field title'),
    'implicit permissions' => array(),
    'also granted by' => array(
      GEF_PERMISSION_FULL_CONTROL,
      GEF_PERMISSION_EDIT_PERMISSIONS,
      GEF_PERMISSION_EDIT_TITLE,
    ),
  );
}

/**
 * Access callback: does a given user ID match an ACL's UID entry?
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_access_user_uid(array $options) {
  return $options['property_id'] == $options['uid'];
}

/**
 * Validation callback which verifies that a given user ID exists.
 *
 * @param array $options
 *
 * @return bool TRUE if the user account exists and can be loaded; FALSE if not.
 */
function greyhead_encrypted_field_acl_validate_user_uid($options) {
  return (user_load($options['property_id']) !== FALSE);
}

/**
 * Access callback: does a given user's name match the username in the ACL?
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_access_user_name(array $options) {
  if ($user = user_load_by_name($options['property_id'])) {
    return (bool) $user->uid == $options['uid'];
  }

  return FALSE;
}

/**
 * Validation callback: does a given username exist?
 *
 * @param array $options
 *
 * @return bool TRUE if the user account exists and can be loaded; FALSE if not.
 */
function greyhead_encrypted_field_acl_validate_user_name(array $options) {
  return (user_load_by_name($options['property_id']) !== FALSE);
}

/**
 * Access callback: does a given user have a particular type of Drupal role?
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_access_role_drupal(array $options) {
  $return = FALSE;
  // What property_id do we need to test the user account for?
  switch ($options['property_id']) {
    // Find out if the user is a member of the current administraton role.
    case GEF_ROLE_DRUPAL_ADMINISTRATOR:
      $return = (bool) user_has_role(variable_get('user_admin_role'), user_load($options['uid']));
      break;

    // Find out if the user is authenticated.
    case GEF_ROLE_DRUPAL_AUTHENTICATED:
      $return = (bool) $options['uid'] > 0;
      break;

    // Find out if the user is anonymous.
    case GEF_ROLE_DRUPAL_ANONYMOUS:
      $return = (bool) ($options['uid'] == 0) || is_null($options['uid']);
      break;
  }

  return $return;
}

/**
 * Validation callback to validate that a Drupal built-in role name is valid.
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_validate_role_drupal(array $options) {
  return in_array($options['property_id'], array(
    GEF_ROLE_DRUPAL_ADMINISTRATOR,
    GEF_ROLE_DRUPAL_AUTHENTICATED,
    GEF_ROLE_DRUPAL_ANONYMOUS,
  ), TRUE);
}

/**
 * Access callback: is a given user ID a member of a particular role?
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_access_role_custom(array $options) {
  return (bool) user_has_role_by_name($options['property_id'], user_load($options['uid']));
}

/**
 * Check that the ACLs provided are valid.
 *
 * @param array $acls An array of ACL strings as the array key, with the value
 *                    being an array of data. This function will set the 'valid'
 *                    key to TRUE or FALSE for each ACL entry.
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_validate_acls(array &$acls) {
  // Start by assuming they're all valid until proven otherwise.
  $valid = TRUE;

  foreach ($acls as &$acl) {
    $valid = $valid && $acl->validate();
  }

  return $valid;
}

/**
 * Checks whether an array of permissions is valid.
 *
 * This function is designed to be used to validate the permissions array from
 * greyhead_encrypted_field_acl_explode().
 *
 * @param array $permissions
 *
 * @return bool TRUE if all permissions are recognised, or FALSE if one or more
 *              were unknown.
 */
function greyhead_encrypted_field_acl_validate_permissions(array $permissions) {
  // Start on the assumption that they're all valid, but one invalid permission
  // will result in a false.
  $valid = TRUE;

  foreach ($permissions as $permission) {
    $valid = $valid && greyhead_encrypted_field_acl_valid_permission($permission);
  }

  return $valid;
}

/**
 * Check whether a single permission string is valid.
 *
 * Wrapper function for greyhead_encrypted_field_acl_validate_permissions().
 *
 * @param $permission
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_validate_permission($permission) {
  return greyhead_encrypted_field_acl_validate_permissions(array($permission));
}

/**
 * Validate that an entity is valid.
 *
 * @param $entity_type
 * @param $entity_id
 *
 * @return bool|mixed
 */
function greyhead_encrypted_field_acl_validate_entity($entity_type, $entity_id) {
  // We use NULL to indicate that the entity isn't validated yet, or is unknown.
  $return_array = array(
    'valid_entity' => NULL,
    'valid_entity_type' => NULL,
    'valid_entity_id' => NULL,
  );

  // Load the entity type info, which tells us the access callback.
  if ($acl_entity_type_info = greyhead_encrypted_field_acl_entity_type_info($entity_type)) {
    // It's a valid entity type.
    $return_array['valid_entity_type'] = TRUE;

    // Determine the validation function name.
    $validation_callback_function = $acl_entity_type_info['validation callback'];

    if (function_exists($validation_callback_function)) {
      // Assemble the $options array.
      $options = array(
        'property_id' => $entity_id,
        'entity_uid' => NULL,
        'entity' => NULL,
      );

      // At last, we know everything we need to - check if valid.
      $return_array['valid_entity_id'] = (bool) call_user_func($validation_callback_function, $options);

      if (!$return_array['valid_entity_id']) {
        greyhead_encrypted_field_error('Entity type %type valid but ID %id failed validation in function %function.', array(
          '%function' => $validation_callback_function,
          '%type' => $entity_type,
          '%id' => $entity_id,
        ));
      }
    }
    else {
      // Entity information found but unknown validation function. We allow the
      // entity to pass validation though.
      greyhead_encrypted_field_error('Validation function %function not found for Entity type %type and ID %id.', array(
        '%function' => $validation_callback_function,
        '%type' => $entity_type,
        '%id' => $entity_id,
      ), WATCHDOG_WARNING, TRUE);
    }
  }
  else {
    // Unrecognised entity type, but we allow it to pass
    greyhead_encrypted_field_error('Unrecognised entity type %type with ID %id passed in greyhead_encrypted_field_acl_validate_entity().', array(
      '%type' => $entity_type,
      '%id' => $entity_id,
    ), WATCHDOG_WARNING, TRUE);
  }

  // Lastly, set $return_array['valid_entity'] to TRUE if both type and ID
  // validated, or FALSE if either entity type or ID were definitely invalid.
  if (($return_array['valid_entity_type'] === TRUE) && ($return_array['valid_entity_id'] === TRUE)) {
    $return_array['valid_entity'] = TRUE;
  }

  if (($return_array['valid_entity_type'] === FALSE) || ($return_array['valid_entity_id'] === FALSE)) {
    $return_array['valid_entity'] = FALSE;
  }

  return $return_array;
}

/**
 * Validation callback: is a given custom role name valid?
 *
 * Note that we don't use role IDs here, but instead the role names, for
 * portability.
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_validate_role_custom(array $options) {
  return (bool) get_role_by_name($options['property_id']);
}

/**
 * Checks if a user is of a particular type with respect to an encrypted field.
 *
 * For example, this function could be asked to check whether a user is the
 * original creator of a field.
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_access_user_type($options) {
  $return = FALSE;

  // What property_id do we need to test the user account for?
  switch ($options['property_id']) {
    // Find out if the user created the field.
    case GEF_USER_TYPE_CREATOR:
      $return = ($options['entity_uid'] == $options['uid']);
      break;

    // Find out if the user is the root user, i.e. UID 1.
    case GEF_USER_TYPE_ROOT:
      $return = (bool) $options['uid'] == 1;
      break;

    // Is the user part of the "everyone" group - always returns TRUE.
    case GEF_USER_TYPE_EVERYONE:
      $return = TRUE;
      break;
  }

  return $return;
}

/**
 * Checks whether the submitted user type is valid.
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_validate_user_type(array $options) {
  return in_array($options['property_id'], array(
    GEF_USER_TYPE_CREATOR,
    GEF_USER_TYPE_ROOT,
    GEF_USER_TYPE_EVERYONE,
  ), TRUE);
}

/**
 * Verify whether a user has a certain permission on the field's parent node.
 *
 * If the field is not attached to a node, this permission returns NULL.
 *
 * $options['property_id'] contains the $op type, e.g. view, update, delete or
 * create.
 *
 * $options['uid'] is the user UID.
 *
 * $options['entity'] should contain the field's parent node.
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_access_parent_node($options) {
  $valid_node = FALSE;

  // Is the field's parent entity a node?
  if (isset($options['entity'], $options['entity']->nid, $options['entity']->vid)) {
    // Looks like a node. Try loading it and compare the values.
    if ($node_loaded = node_load($options['entity']->nid, $options['entity']->vid)) {
      // Smells like a node...
      if (($node_loaded->nid == $options['entity']->nid)
        && ($node_loaded->vid == $options['entity']->vid)
        && ($node_loaded->title == $options['entity']->title)
      ) {
        // Quacks like a node, so it's probably a node...
        $valid_node = TRUE;
      }
    }
  }

  // If it's not valid, bail.
  if (!$valid_node) {
    return FALSE;
  }

  // Get the account to test.
  $account = user_load($options['uid']);

  // Depending on the type of operation, we need to either get the node object,
  // or just the node type.
  switch ($options['property_id']) {
    case GEF_NODE_VIEW:
    case GEF_NODE_EDIT:
    case GEF_NODE_DELETE:
      $node = $options['entity'];
      break;

    case GEF_NODE_CREATE:
      $node = $options['entity']->type;
      break;
  }

  return node_access($options['property_id'], $node, $account);
}

/**
 * Verify whether the node operation is valid.
 *
 * It should be one of view, update, delete or create.
 *
 * @param array $options
 *
 * @return bool
 */
function greyhead_encrypted_field_acl_validate_parent_node(array $options) {
  return in_array($options['property_id'], array(
    GEF_NODE_VIEW,
    GEF_NODE_EDIT,
    GEF_NODE_DELETE,
    GEF_NODE_CREATE,
  ), TRUE);
}

/**
 * Get the FAPI element for the title of the encrypted field.
 *
 * @param string    $default_value
 *
 * @return array
 */
function greyhead_encrypted_field_acl_get_label_element($default_value = '') {
  $widget_label_element = array(
    '#title' => t('Name of this encrypted field'),
    '#type' => GEF_FIELD_LABEL_FAPI_TYPE,
    '#default_value' => $default_value,
    '#maxlength' => 255,
    '#decrypted' => FALSE, // Set to TRUE if the data is decrypted.
    '#element_validate' => array(
      'greyhead_encrypted_field_encrypt',
    ),
  );

  return $widget_label_element;
}

/**
 * Get the FAPI element for the value of the encrypted field.
 *
 * @param string $default_value
 *
 * @return array
 */
function greyhead_encrypted_field_acl_get_value_element($widget_type, $default_value = '') {
  // Figure out which widget we need to generate.
  // In our case, there's only one type.
  $widget_value_element = array(
    '#title' => t('Data to encrypt'),
    '#default_value' => $default_value,
    '#type' => GEF_FIELD_VALUE_FAPI_TYPE,
    '#attributes' => array('class' => array('greyhead-encrypted-field')),
    '#decrypted' => FALSE, // Set to TRUE if the data is decrypted.
    '#element_validate' => array(
      'greyhead_encrypted_field_encrypt',
    ),
  );

  // Determine the widget type.
  switch ($widget_type) {
    case GEF_FIELD_WIDGET:
    default:
      $widget_value_element['#type'] = GEF_FIELD_VALUE_FAPI_TYPE;
      break;
  }

  return $widget_value_element;
}

/**
 * Gets textarea elements with any ACLs for the encrypted field instance.
 *
 * This function does not check access to the element.
 *
 * @param      $form_type
 * @param null $acls
 *
 * @return array
 */
function greyhead_encrypted_field_acl_get_acls_element($form_type, $acls = NULL) {
  switch ($form_type) {
    case GEF_FORM_TYPE_SITEWIDE:
      $title = 'Permissions applied to all encrypted fields on this site';
      $intro_text = 'Default access control lists which apply to all fields across the site. These permissions will be added to any field-specific permissions.';

      $element_type = GEF_FIELD_GLOBAL_ACLS_FAPI_TYPE;
      $element_key = GEF_FIELD_GLOBAL_ACLS_FAPI_KEY;

      break;

    case GEF_FORM_TYPE_SITEWIDE_READONLY:
      $title = 'Inherited permissions';

      $element_type = GEF_FIELD_GLOBAL_ACLS_FAPI_TYPE;
      $element_key = GEF_FIELD_GLOBAL_ACLS_FAPI_KEY;

      break;

    case GEF_FORM_TYPE_FIELD_INSTANCE:
      $title = 'Default permissions for this encrypted field';
      $intro_text = 'Default access control lists to use when creating new entities which contain this field. Each field value will have its own access control list.';
      $element_type = GEF_FIELD_DEFAULT_ACLS_FAPI_TYPE;
      $element_key = GEF_FIELD_DEFAULT_ACLS_FAPI_KEY;
      break;

    case GEF_FORM_TYPE_ENTITY:
    default:
      $title = 'Permissions for this encrypted field';
      $intro_text = 'Access control lists for this encrypted field value.';
      $element_type = GEF_FIELD_ACLS_FAPI_TYPE;
      $element_key = GEF_FIELD_ACLS_FAPI_KEY;
      break;
  }

  if (($form_type == GEF_FORM_TYPE_SITEWIDE)
    || ($form_type == GEF_FORM_TYPE_SITEWIDE_READONLY)
  ) {
    // If $acls is NULL, we need to get the site-wide defaults.
    if (is_null($acls)) {
      $acls = greyhead_encrypted_field_acl_default_acls(GEF_ACL_CONTEXT_SITEWIDE);
    }
  }

  // Crufty data handling; if the ACLs aren't already in strings, convert now.
  if (!is_null($acls) && !is_string($acls)) {
    $acls = greyhead_encrypted_field_acl_acls_to_strings($acls);
  }

  // Create the basic textarea.
  $widget_acls_textarea = array(
    '#type' => $element_type,
    '#title' => t($title),
    '#element_key' => $element_key,
    '#default_value' => $acls,
    // ACLs are always decrypted.
    '#decrypted' => TRUE,
  );

  // If we've been asked for the GEF_FORM_TYPE_SITEWIDE_READONLY textarea, we
  // set it to read-only and use different help text to other instances of the
  // ACLs field.
  if ($form_type == GEF_FORM_TYPE_SITEWIDE_READONLY) {
    $widget_acls_textarea['#value'] = $widget_acls_textarea['#default_value'];
    $widget_acls_textarea['#disabled'] = TRUE;

    // If the current user can administer the Encrypted Field's settings, add
    // a link to edit the default ACLs.
    $editlink_html = '';

    if (user_access(GEF_PERMISSION_ADMINISTER_MODULE)) {
      $editlink_html = ' ' . l(
          t('You can edit these default permissions here.'),
          'admin/config/system/encrypt/encryptedfield', array(
          'attributes' => array(
            'target' => '_blank',
          ),
        ));
    }

    // Create a read-only textarea containing the site-wide ACLs and append
    // it to the ACLs textarea.
    $widget_acls_textarea['#description'] = t('These ACLs are applied to all encrypted fields on the site, in addition to any extra permissions you specify above.!editlink', array(
      '!editlink' => $editlink_html,
    ));

    gefdpm(__FUNCTION__);
    gefdpm($widget_acls_textarea);
  }
  else {
    $widget_acls_textarea['#description'] = t('<p>!intro_text</p> '
      . '<p>ACLs are written in the form <em>[entity type]:[entity ID]::permissions:[permission 1],[permission 2],[...],[permission n]</em></p> '
      . '<p>For example, this ACL allows user monkeybones to edit the field and its label: <em>!entitytypeuser:monkeybones::permissions:!edittitle,!editvalue</em></p> '
      . '<p>The entity types available are: <em>!entitytypes</em>.</p> '
      . '<p>The permissions available for each entity are: <em>!permissionsavailable</em>.</p> '
      . '<p>By default, the user who created the field, '
      . 'and the site administrators, have full control over the field. Logged-'
      . 'in users can view the field title but can\'t view the encrypted data, '
      . 'and anonymous users cannot see anything.</p>', array(
      '!intro_text' => t($intro_text),
      '!entitytypeuser' => GEF_USER_NAME,
      '!edittitle' => GEF_PERMISSION_EDIT_TITLE,
      '!editvalue' => GEF_PERMISSION_EDIT_VALUE,
      '!entitytypes' => implode(', ', array_keys(greyhead_encrypted_field_acl_entity_types_info())),
      '!permissionsavailable' => implode(', ', array_keys(greyhead_encrypted_field_acl_permissions_available())),
    ));

    $widget_acls_textarea['#element_validate'] = array(
      'greyhead_encrypted_field_acl_get_acls_element_validate',
      'greyhead_encrypted_field_encrypt',
    );

    $widget_acls_textarea['#default_value'] = $acls;
  }

  return $widget_acls_textarea;
}

/**
 * Validation function which validates the ACLs submitted in an ACLs field.
 *
 * @param $element
 * @param $form_state
 */
function greyhead_encrypted_field_acl_get_acls_element_validate($element, &$form_state) {
  // Don't validate if we have a greyhead_encrypted_field_reset_default_acls
  // field and it's checked.
  if (isset($form_state['values']['greyhead_encrypted_field_reset_default_acls'])
    && $form_state['values']['greyhead_encrypted_field_reset_default_acls']) {
    return;
  }

  $acls_textarea_value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);

  $acls = greyhead_encrypted_field_acl_textfield_to_acls($acls_textarea_value);

  // If one or more ACLs are invalid, create a list of invalid ACL strings.
  $acls_valid = greyhead_encrypted_field_acl_validate_acls($acls);
  if (!$acls_valid) {
    $invalid_acls = array();

    foreach ($acls as $acl_string => $acl) {
      if (!$acl->valid) {
        gefdpm($acl);
        $invalid_acl_parts = array();

        if (!$acl->valid_entity) {
          if (!$acl->valid_entity_type) {
            $invalid_acl_parts[] = 'entity type';
          }
          if (!$acl->valid_entity_id) {
            $invalid_acl_parts[] = 'entity ID';
          }
        }

        if (!$acl->valid_permissions) {
          $invalid_acl_parts[] = 'permissions';
        }

        $options = array(
          '!acl' => $acl_string,
          '!invalid_parts' => implode(', ', $invalid_acl_parts),
        );

        $invalid_acls[] = t('ACL "!acl" invalid parts: !invalid_parts', $options);
      }
    }

    form_error($element, t('<p>The following ACLs contain errors:</p> <ul><li>' . implode('</li> <li>', $invalid_acls) . '</li></ul>'));
  }
}

/**
 * Re-encrypt submitted form data if it needs to be encrypted before saving.
 *
 * @param $element
 * @param $form_state
 */
function greyhead_encrypted_field_encrypt($element, &$form_state) {
  // Get the #decrypted boolean; if it's FALSE, we don't need to do anything;
  // if it's TRUE, we need to re-encrypt the data.
  $decrypted = $element['#decrypted'];

  if ($decrypted) {
    // We need to encrypt the element's value.
    // Get the element's value.
    $element_value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);

    // Encrypt the value.
    $element_value_encrypted = encrypt($element_value);

    // Set the value back into $form_state['values'].
    form_set_value($element, $element_value_encrypted, $form_state);
  }

  gefdpm(__FUNCTION__);
  gefdpm($element);
  gefdpm($form_state['values']);
}

/**
 * Determine the ACLs for an encrypted field.
 *
 * This function starts by checking whether $items[$items_delta] contains field
 * information, and uses the ACLs set in that field if it does.
 *
 * If $items[$items_delta] isn't set, the field is probably new, so we look in
 * $instance[GEF_SETTINGS_KEY][GEF_ACLS_KEY] for defaults and use those, if available.
 *
 * Failing that, we look at $field[GEF_SETTINGS_KEY][GEF_ACLS_KEY] for defaults,
 * which come from greyhead_encrypted_field_acl_default_acls().
 *
 * @param $field
 * @param $instance
 * @param $items
 * @param $items_delta
 *
 * @return array|null
 */
function greyhead_encrypted_field_acl_get_acls($field = NULL, $instance = NULL, $items = NULL, $items_delta = NULL) {
  if (!is_null($items) && !is_null($items_delta)
    && isset($items[$items_delta], $items[$items_delta][GEF_FIELD_ACLS_FAPI_KEY])
  ) {
    $acls = $items[$items_delta][GEF_FIELD_ACLS_FAPI_KEY];
  }
  elseif (!is_null($instance)
    && isset($instance[GEF_SETTINGS_KEY], $instance[GEF_SETTINGS_KEY][GEF_FIELD_DEFAULT_ACLS_FAPI_KEY])
  ) {
    $acls = $instance[GEF_SETTINGS_KEY][GEF_FIELD_DEFAULT_ACLS_FAPI_KEY];
  }
  else {
    $acls = greyhead_encrypted_field_acl_default_acls();
  }

  return $acls;
}

/**
 * Checks whether a user has access to view or edit an encrypted field.
 *
 * Depending on their access, the user will either be able to edit the field's
 * value, view the field but not make changes, or see a message telling them
 * that the field is not accessible.
 *
 * @see      also greyhead_encrypted_field_acl_check_widget_form_elements_access().
 *
 * @param      $widget_form_element
 * @param      $entity
 * @param      $edit_permission
 * @param      $view_permission
 * @param      $acls
 * @param null $account
 *
 * @internal param $form_element_to_check
 */
function greyhead_encrypted_field_acl_check_widget_form_element_access(&$widget_form_element, $entity, $edit_permission = NULL, $view_permission, $acls, $account = NULL) {
  // Do we have a user account?
  if (is_null($account)) {
    $account = $GLOBALS['user'];
  }

  // Debugging fun and games.
  gefdpm(__FUNCTION__);
  gefdpm($widget_form_element, __FUNCTION__ . ' $widget_form');
  gefdpm('@TODO: pass in the entity so we can try to get the owner.');

  // Do they have view access?
  if (greyhead_encrypted_field_acl_user_access($view_permission, $acls, $entity, GEF_FIELD_TYPE, $account)) {
    // Yes. If the #decrypted flag is FALSE, we need to decrypt it and update
    // the flag.
    if (!$widget_form_element['#decrypted']) {
      $widget_form_element['#default_value'] = greyhead_encrypted_field_decrypt($widget_form_element['#default_value']);
      $widget_form_element['#decrypted'] = TRUE;
    }

    // Have we been asked to check the field's edit access, and does the user
    // have access to edit the field?
    if (is_null($edit_permission)
      || !greyhead_encrypted_field_acl_user_access($edit_permission, $acls, $entity, GEF_FIELD_TYPE, $account)
    ) {
      // No. Disable the field but allow them to view its decrypted value.
      $widget_form_element['#disabled'] = TRUE;
      $widget_form_element['#value'] = $widget_form_element['#default_value'];
    }
  }
  else {
    // No access at all. Hide the field's contents by copying the label field,
    // hiding the existing label field, and changing the copied field's text.
    $widget_form_element['#value'] = $widget_form_element['#default_value'];

    // Show a sensible "no access" message.
    $widget_form_element['#default_value'] = t(GEF_NO_ACCESS, array('!thing' => t('thing')));
    $widget_form_element['#disabled'] = TRUE;
  }
}

/**
 * Log an error.
 *
 * @param     $message
 * @param     $parameters
 * @param int $severity
 */
function greyhead_encrypted_field_error($message, $parameters, $severity = WATCHDOG_ERROR, $database_only = FALSE) {
  if (!$database_only) {
    drupal_set_message(t($message, $parameters), 'error', FALSE);
  }

  watchdog(GEF_MODULE_TITLE, $message, $parameters, $severity);
}

/**
 * Wrapper around gefdpm() to allow us to turn debugging output on and off.
 *
 * @param      $input
 * @param null $name
 */
function gefdpm($input, $name = NULL) {
  if (variable_get('greyhead_encrypted_field_debug')
    && function_exists('dpm')
  ) {
    if (!is_null($name)) {
      dpm($name);
    }

    if (user_access('access devel information')) {
      $export = kprint_r($input, TRUE, $name);
      drupal_set_message($export, 'status');
    }
    return $input;
  }
}

/**
 * Implements hook_greyhead_encrypted_field_acl_validate_alter.
 *
 * Prevent the anonymous user role and everyone user types from being
 * granted full control, edit permissions, view value, or edit title/value.
 *
 * @param $acl
 */
function greyhead_encrypted_field_greyhead_encrypted_field_acl_validate_alter(&$acl) {
  $dangerous_permissions = greyhead_encrypted_field_dangerous_permissions();
  $dangerous_entities = greyhead_encrypted_field_dangerous_entities();

  // Create a bool which will be TRUE if the entity grants access to
  // anonymous users.
  $anonymous_access = FALSE;

  // Loop through the dangerous entities list to see if we have any matches.
  foreach ($dangerous_entities as $dangerous_entity_type => $dangerous_entity_ids) {
    // Don't keep searching if we already know we have an issue.
    if ($anonymous_access) {
      break;
    }

    foreach ($dangerous_entity_ids as $dangerous_entity_id) {
      // Don't keep searching if we already know we have an issue.
      if ($anonymous_access) {
        break;
      }

      if (($acl->entity_type == $dangerous_entity_type)
        && ($acl->entity_id == $dangerous_entity_id)
      ) {
        $anonymous_access = TRUE;
      }
    }
  }

  if ($anonymous_access) {
    // Loop through the ACL's permissions and remove any in the above list.
    $permissions_removed = array();

    foreach ($acl->permissions as $row => $permission) {
      if (in_array($permission, $dangerous_permissions, TRUE)) {
        $permissions_removed[] = $permission;
        unset($acl->permissions[$row]);
      }
    }

    if (!empty($permissions_removed)) {
      $parameters = array(
        '%entity_type' => $acl->entity_type,
        '%entity_id' => $acl->entity_id,
        '%permissions_removed' => implode(', ', $permissions_removed),
      );

      greyhead_encrypted_field_error('The following unsafe permissions were removed from the %entity_type "%entity_id" ACL: %permissions_removed.', $parameters, WATCHDOG_WARNING);
    }
  }
}

/**
 * Wrapper around decrypt() which doesn't fall on its arse when the value to be
 * decrypted is ''.
 *
 * @param $value
 *
 * @return string
 */
function greyhead_encrypted_field_decrypt($value) {
  // If the value of this field is serialized, it's probably encrypted.
  if (!(unserialize($value) === FALSE)) {
    $value = @decrypt($value);
  }

  return $value;
}
