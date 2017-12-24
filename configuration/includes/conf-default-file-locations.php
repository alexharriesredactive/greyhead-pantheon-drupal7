<?php

/**
 * @file
 * default-file-locations.php
 *
 * Sets the default location of public, private and temporary files for all
 * Drupal site types.
 *
 * You can override this either by setting the variable you want to override in
 * an environment-specific settings file, or in the settings.php for a
 * particular site.
 */

// Set the public files path:
$conf['file_public_path'] = conf_path() . '/files';
$conf['greyhead_configuration']['overridden_variables'][] = 'file_public_path';

// Set the private files path:
$conf['file_private_path'] = conf_path() . '/files/privatefiles/';
$conf['greyhead_configuration']['overridden_variables'][] = 'file_private_path';

// Set the temporary files location and a flag to indicate it's been overridden.
$conf['file_temporary_path'] = conf_path() . '/files/privatefiles/tmp';
$conf['greyhead_configuration']['overridden_variables'][] = 'file_temporary_path';

// Check that all directories exist and are writeable; if not, attempt to create
// but fail silently.
require_once GREYHEAD_DRUPAL_ROOT . '/includes/common.inc';

// Directories to check.
$directories_to_check = array(
  'file_public_path',
  'file_private_path',
  'file_temporary_path',
);

// Store our results in case we want to debug them later.
$results = array();
foreach ($directories_to_check as $directory_to_check) {
  $results[$directory_to_check] = greyhead_configuration_prepare_directory(GREYHEAD_DRUPAL_ROOT . '/' . $conf[$directory_to_check]);
}

$conf['greyhead_configuration']['directories_check'] = $results;

/**
 * Code stolen shamelessly from file_prepare_directory().
 *
 * @param $directory
 *
 * @return bool
 */
function greyhead_configuration_prepare_directory($directory) {
  // Check if directory exists.
  if (!is_dir($directory)) {
    // Let mkdir() recursively create directories and use the default directory
    // permissions.
    if (@mkdir($directory, 0770, TRUE)) {
      return greyhead_configuration_chmod($directory, 0770);
    }
    return FALSE;
  }

  // The directory exists, so check to see if it is writable.
  $writable = is_writable($directory);
  if (!$writable) {
    return greyhead_configuration_chmod($directory, 0770);
  }

  return $writable;
}

/**
 * Lightweight version of drupal_chmod().
 *
 * @param $directory
 * @param $mode
 *
 * @return bool
 */
function greyhead_configuration_chmod($directory, $mode) {
  if (@chmod($directory, $mode)) {
    return TRUE;
  }

  return FALSE;
}
