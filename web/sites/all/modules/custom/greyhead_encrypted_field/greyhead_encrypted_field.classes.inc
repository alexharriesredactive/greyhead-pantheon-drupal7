<?php

/**
 * @file
 * greyhead_encrypted_field.classes.inc
 */
class greyhead_acl {
  public $entity_type = NULL;
  public $entity_id = NULL;
  public $permissions = array();
  public $valid = NULL;
  public $valid_entity = NULL;
  public $valid_entity_type = NULL;
  public $valid_entity_id = NULL;
  public $valid_permissions = NULL;

  /**
   * Creates a validated ACL array.
   *
   * Given an entity type, entity ID and an array of zero or more permissions,
   * this function will check that all information provided is valid and, if so,
   * return an ACL array.
   *
   * This function is used by greyhead_encrypted_field_acl_string_to_acl().
   *
   * @param       $entity_type
   * @param       $entity_id
   * @param       $permissions_array_or_string
   */
  function __construct($entity_type, $entity_id, $permissions_array_or_string) {
    gefdpm(__FUNCTION__);
    gefdpm($entity_type);
    gefdpm($entity_id);
    gefdpm($permissions_array_or_string);
    $this->entity_type = $entity_type;
    $this->entity_id = $entity_id;

    // Cast $permissions_array_or_string to an array.
    $this->permissions = $this->permissions_to_array($permissions_array_or_string);

    $this->valid = GEF_ACL_NOT_VALIDATED;

    // Implement a hook_greyhead_encrypted_field_acl_build_alter.
    drupal_alter('greyhead_encrypted_field_acl_build', $this);

    // Validate the ACL.
    $this->validate();
  }

  /**
   * Convert a permissions array or string into an array.
   *
   * @param $permissions_array_or_string
   *
   * @return array
   */
  function permissions_to_array($permissions_array_or_string) {
    if (is_string($permissions_array_or_string)) {
      $permissions_array_or_string = $this->value_string_to_array($permissions_array_or_string);
    }

    return (array)$permissions_array_or_string;
  }

  /**
   * Convert an ACL entity to a string.
   * @return string
   * @throws \Exception
   */
  function to_string() {
    // Does the ACL need validating?
    if ($this->valid === GEF_ACL_NOT_VALIDATED) {
      $this->validate();
    }

    // Is the ACL invalid?
    if ($this->valid !== GEF_ACL_VALID) {
      return FALSE;
    }

    // Awesome. Initialise the acl parts array.
    $acl_parts = array();

    // Implode the ACL, starting with the entity type and ID, e.g.
    // user-uid:123
    $acl_parts[] = $this->entity_type
      . GEF_PERMISSIONS_KEY_VALUE_SEPARATOR
      . $this->entity_id;

    // Now implode the permissions.
    $acl_parts[] = GEF_PERMISSIONS_LABEL . GEF_PERMISSIONS_KEY_VALUE_SEPARATOR
      . $this->value_array_to_string($this->permissions);

    // Implode the ACL string.
    return implode(GEF_PERMISSIONS_PART_SEPARATOR, $acl_parts);
  }

  /**
   * Check whether this ACL defines a permission for ANY user.
   *
   * This function is used to quickly check whether a requested permission is
   * available in an ACL.
   *
   * Note that if one of the permissions granted is "full control", this function
   * will return TRUE, because the full control permission allows the user to do
   * anything.
   *
   * @param array $permissions
   *
   * @return array An array of zero or more permissions which this ACL provides.
   */
  function defines_permissions(array $permissions) {
    return array_intersect($permissions, $this->permissions);
  }

  /**
   * Converts an ACL's value string into an array.
   *
   * All ACL values - entity types, entity IDs, and permissions, are stored in
   * indexed arrays, e.g.:
   *
   * 'entity-type' => array(0 => 'user-uid'),
   * 'user-uid' => array(0 => 123),
   * 'permissions' => array(0 => 'administer monkeys', 1 => 'edit any monkey'),
   *
   * @param $value_string
   *
   * @return array
   */
  function value_string_to_array($value_string) {
    return (array) explode(GEF_PERMISSIONS_VALUE_SEPARATOR, $value_string);
  }

  /**
   * Converts an ACL's value array into a string.
   *
   * All ACL values - entity types, entity IDs, and permissions, are stored in
   * indexed arrays, e.g.:
   *
   * 'entity-type' => array(0 => 'user-uid'),
   * 'user-uid' => array(0 => 123),
   * 'permissions' => array(0 => 'administer monkeys', 1 => 'edit any monkey'),
   *
   * @param $value_array
   *
   * @return array
   */
  function value_array_to_string($value_array) {
    return implode(GEF_PERMISSIONS_VALUE_SEPARATOR, $value_array);
  }

  /**
   * Check that the ACL is valid.
   *
   * @return bool
   */
  function validate() {
    // Make sure the entity type and entity ID are valid.
    $valid_entity = $this->validate_entity();

    // Now validate the permissions.
    $valid_permissions = $this->validate_permissions();

    $this->valid = $valid_entity && $valid_permissions;

    // Implement a hook_greyhead_encrypted_field_acl_validate_alter. We use this
    // to prevent the anonymous user role and everyone user types from being
    // granted full control, edit permissions, view value, or edit title/value.
    drupal_alter('greyhead_encrypted_field_acl_validate', $this);

    if (!$this->valid) {
      $this->error('ACL for entity type %type ID %id failed validation - see previous messages for root cause.', array(
        '%type' => $this->entity_type,
        '%id' => $this->entity_id,
      ), WATCHDOG_ERROR, TRUE);
    }

    return $this->valid;
  }

  /**
   * Checks whether the ACL's entity type and ID are valid.
   *
   * @param bool $return_array Set to TRUE to return an array with keys
   *                           'valid_entity', 'valid_entity_type', and
   *                           'valid_entity_id'; otherwise, this function just
   *                           returns the value of 'valid_entity' which is TRUE
   *                           when the other two keys are both valid, or FALSE
   *                           if not.
   *
   * @return bool|array        A boolean or an array of booleans depending on
   *                           $return_array.
   */
  function validate_entity($return_array = FALSE) {
    $validation_array = greyhead_encrypted_field_acl_validate_entity(
      $this->entity_type,
      $this->entity_id);

    // Copy the validation values into $this.
    $this->valid_entity = $validation_array['valid_entity'];
    $this->valid_entity_type = $validation_array['valid_entity_type'];
    $this->valid_entity_id = $validation_array['valid_entity_id'];

    gefdpm($validation_array);
    gefdpm($this);

    return $return_array ? $validation_array : $validation_array['valid_entity'];
  }

  /**
   * Checks whether the ACL's permissions are valid.
   *
   * @return bool TRUE if all permissions are recognised, or FALSE if one or more
   *              were unknown.
   */
  function validate_permissions() {
    $this->valid_permissions = greyhead_encrypted_field_acl_validate_permissions($this->permissions);
    return $this->valid_permissions;
  }

  /**
   * Log an error.
   *
   * @param     $message
   * @param     $parameters
   * @param int $severity
   */
  function error($message, $parameters, $severity = WATCHDOG_ERROR, $database_only = FALSE) {
    greyhead_encrypted_field_error($message, $parameters, $severity, $database_only);
  }
}
