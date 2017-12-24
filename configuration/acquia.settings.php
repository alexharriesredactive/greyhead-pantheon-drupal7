<?php

/**
 * @file
 * Acquia-specific settings.
 */

/**
 * Map the environment type to the Acquia environment type.
 */

// Map the array key names in settings.site_urls.info to their environments.
$acquia_environment_type_mappings = [
  ENVIRONMENT_TYPE_LIVE => 'prod',
  ENVIRONMENT_TYPE_RC => 'stg',
  ENVIRONMENT_TYPE_DEV => 'dev',
  ENVIRONMENT_TYPE_RA => 'ra',
];

/**
 * Override the private and temp files path.
 */

// E.g. /mnt/files/greyhead01.prod/sites/greyhead.com.london/files-private
$conf['file_private_path'] = '/mnt/files/' . $_ENV['AH_SITE_GROUP'] . '.' . $_ENV['AH_SITE_ENVIRONMENT'] . '/sites/' . MULTISITE_IDENTIFIER . '/files-private';

// E.g. /mnt/tmp/greyhead01
$conf['file_temporary_path'] = '/mnt/tmp/' . $_ENV['AH_SITE_GROUP'];

// Remove the greyhead_configuration overridden_variables entries.
//if ($array_key = array_search('file_private_path', $conf['greyhead_configuration']['overridden_variables'])) {
//  unset($conf['greyhead_configuration']['overridden_variables'][$array_key]);
//}
//
//if ($array_key = array_search('file_temporary_path', $conf['greyhead_configuration']['overridden_variables'])) {
//  unset($conf['greyhead_configuration']['overridden_variables'][$array_key]);
//}

/**
 * Acquia database settings.
 */
// If the settings file name isn't defined already, we can assume it's the
// multisite identifier followed by "-settings.inc".
defined('AH_SETTINGS_FILE_NAME') || define('AH_SETTINGS_FILE_NAME', MULTISITE_IDENTIFIER . '-settings.inc');

// Same for the settings file dir.
defined('AH_SETTINGS_FILE_DIR') || define('AH_SETTINGS_FILE_DIR', '/var/www/site-php/' . $_ENV['AH_SITE_GROUP'] . '/');

if (is_dir('/var/www/site-php')) {
  if (file_exists(AH_SETTINGS_FILE_DIR . AH_SETTINGS_FILE_NAME)) {
    require AH_SETTINGS_FILE_DIR . AH_SETTINGS_FILE_NAME;
  }
}

/**
 * Determine Acquia environment and set universal Acquia Cloud settings.
 */
$ah_env = 'undefined';
if (!empty($_ENV['AH_SITE_ENVIRONMENT'])) {
  $ah_env = $_ENV['AH_SITE_ENVIRONMENT'];

  $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
  $conf['cache_default_class'] = 'MemCacheDrupal';
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
}

/**
 * Force settings based on environment
 */
switch ($ah_env) {
  case 'prod':
    break;

  case 'test':
  case 'dev':
  default:
    $conf['apachesolr_environments']['acquia_search_server_1']['conf']['apachesolr_read_only'] = 1;
    break;
}

/**
 * Make sure Drush keeps working.
 *
 * Modified from function drush_verify_cli()
 */
$cli = (php_sapi_name() == 'cli');

/**
 * Acquia Network keys
 *
 * The Acquia Network keys are required to connect your website to Acquia
 * services, including Acquia Search and Insight.
 *
 * https://docs.acquia.com/network/install
 */
//$conf['acquia_identifier'] = '';
//$conf['acquia_key'] = '';
