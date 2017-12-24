<?php

/**
 * @file
 * environment-type-detection-not-pantheon.php
 *
 * This file will use Pantheon's environment variables to set the environment.
 */

/**
 * Map site types to custom settings file names.
 */
$custom_settings_file_names_by_environment_type = [
  ENVIRONMENT_TYPE_LOCAL => SETTINGS_FILE_LOCAL,
  ENVIRONMENT_TYPE_DRUSH => SETTINGS_FILE_DRUSH,
  ENVIRONMENT_TYPE_DEV => SETTINGS_FILE_DEV,
  ENVIRONMENT_TYPE_RC => SETTINGS_FILE_RC,
  ENVIRONMENT_TYPE_LIVE => SETTINGS_FILE_LIVE,
  SUBENVIRONMENT_TYPE_MAMP => SETTINGS_FILE_MAMP,
  SUBENVIRONMENT_TYPE_ACQUIA => SETTINGS_FILE_ACQUIA,
];

/**
 * Get the list of live site URLs, if any, from the settings.site_urls.info
 * file.
 */
$site_urls_and_environment_types = greyhead_configuration_get_site_urls_and_environment_types([MULTISITE_IDENTIFIER]);

$conf['greyhead_configuration']['SETTINGS_SITE_URLS'] = $site_urls_and_environment_types['sites'];
$conf['greyhead_configuration']['ENVIRONMENT_TYPES'] = $site_urls_and_environment_types['environment_types'];

$path_parts = explode('/', $_SERVER['REQUEST_URI']);

/**
 * Work out what host we're on.
 *
 * If a file exists in the multisite directory, or above it (to make it easier
 * to mark a whole server as being a dev/staging/local server), with one of the
 * following names, we set the environment type regardless of anything else:
 *
 * - sites/[multisite directory]/ENVIRONMENT_TYPE_LIVE.txt
 * - sites/ENVIRONMENT_TYPE_LIVE.txt
 * - sites/[multisite directory]/ENVIRONMENT_TYPE_RC.txt
 * - sites/ENVIRONMENT_TYPE_RC.txt
 * - sites/[multisite directory]/ENVIRONMENT_TYPE_DEV.txt
 * - sites/ENVIRONMENT_TYPE_DEV.txt
 * - sites/[multisite directory]/ENVIRONMENT_TYPE_LOCAL.txt
 * - sites/ENVIRONMENT_TYPE_LOCAL.txt
 */
$environment_stub = 'ENVIRONMENT_TYPE_';
$file_extension = '.txt';

// Create a list of the environment types.
$environment_types = [
  'LIVE',
  'RC',
  'DEV',
  'LOCAL',
];

// Map the array key names in settings.site_urls.info to their environments.
$settings_site_urls_info_domain_name_type_mappings = [
  'prod' => ENVIRONMENT_TYPE_LIVE,
  // The production URL without "www.".
  'prod-nowww' => ENVIRONMENT_TYPE_LIVE,
  'stag' => ENVIRONMENT_TYPE_RC,
  'stage' => ENVIRONMENT_TYPE_RC,
  'dev' => ENVIRONMENT_TYPE_DEV,
  'ra' => ENVIRONMENT_TYPE_UNKNOWN,
  'local' => ENVIRONMENT_TYPE_LOCAL,
];

// Map the array key names in settings.site_urls.info to their settings files.
$settings_site_urls_info_settings_file_mappings = [
  'prod' => SETTINGS_FILE_LIVE,
  // The production URL without "www.".
  'prod-nowww' => SETTINGS_FILE_LIVE,
  'stag' => SETTINGS_FILE_STAGE,
  'stage' => SETTINGS_FILE_STAGE,
  'dev' => SETTINGS_FILE_DEV,
  'ra' => SETTINGS_FILE_RA,
  'local' => SETTINGS_FILE_LOCAL,
];

// Create a list of possible file locations.
$possible_environment_file_locations = [
  greyhead_configuration_get_path_to_sites_directory() . MULTISITE_IDENTIFIER . '/',
  greyhead_configuration_get_path_to_sites_directory(),
];

// Create a list of filenames (keys) and the environment types they map to
// (values).
$filenames_to_check = [];
foreach ($environment_types as $environment_type) {
  // Assemble the environment type, e.g.
  // /var/www/sites/[multisite]/[ENVIRONMENT_TYPE_][LIVE][.txt] => [ENVIRONMENT_TYPE_][LIVE]
  // /var/www/sites/[ENVIRONMENT_TYPE_][LIVE][.txt] => [ENVIRONMENT_TYPE_][LIVE]
  foreach ($possible_environment_file_locations as $possible_environment_file_location) {
    $filenames_to_check[$possible_environment_file_location . $environment_stub . $environment_type . $file_extension]
      = $environment_type;
  }
}

// Now loop through each environment type, and then through each possible
// location, and if a file exists matching that name, set the environment
// accordingly.
foreach ($filenames_to_check as $filename_to_check => $environment) {
  if (file_exists($filename_to_check)) {
    // We've found an environment file.
    defined('ENVIRONMENT_OVERRIDE_FILE') || define('ENVIRONMENT_OVERRIDE_FILE', $filename_to_check);
    defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . constant('SETTINGS_FILE_' . $environment));
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', constant($environment_stub . $environment));
  }
}

/**
 * Are we in Drush?
 */
if (!defined('CURRENT_ENVIRONMENT') && drupal_is_cli()) {
  defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_DRUSH);
  defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_DRUSH);
}

/**
 * Search the list of site URLs in the environment types array for matches.
 *
 * $conf['greyhead_configuration']['ENVIRONMENT_TYPES']
 */
if (!defined('CURRENT_ENVIRONMENT')) {
  foreach ($conf['greyhead_configuration']['ENVIRONMENT_TYPES'] as $site_url => $environment_type) {
    if ($site_url == $_SERVER['HTTP_HOST']) {
      // If the environment type from settings.site_urls.info matches a key
      // in the $settings_site_urls_info_domain_name_type_mappings array,
      // then it's a 100% match and we can assume it's explicitly that site
      // type.
      if (array_key_exists(strtolower($environment_type), $settings_site_urls_info_domain_name_type_mappings)) {
        defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', $settings_site_urls_info_domain_name_type_mappings[strtolower($environment_type)]);

        // Define the path to the local settings file, if available.
        defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . $settings_site_urls_info_settings_file_mappings[strtolower($environment_type)]);

        break;
      }
      // Otherwise, if a key in the
      // $settings_site_urls_info_domain_name_type_mappings array matches the
      // start of the environment type - e.g. the environment type begins
      // with "prod-" - then we can assume it's a prod environment (for
      // example).
      else {
        // @todo: These variable names are getting a bit unmanageable...
        foreach ($settings_site_urls_info_domain_name_type_mappings as $settings_site_urls_info_domain_name_type_mappings_environment_type_from_sites_file => $settings_site_urls_info_domain_name_type_mappings_environment_constant_value) {
          if (substr(strtolower($environment_type), 0, strlen($settings_site_urls_info_domain_name_type_mappings_environment_type_from_sites_file)) == $settings_site_urls_info_domain_name_type_mappings_environment_type_from_sites_file) {
            // We've matched the environment type - e.g. prod-nowww - to the
            // start of the environment type from the settings.site_urls.info
            // file.
            defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', $settings_site_urls_info_domain_name_type_mappings[$settings_site_urls_info_domain_name_type_mappings_environment_type_from_sites_file]);

            // Define the path to the local settings file, if available.
            defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . $settings_site_urls_info_settings_file_mappings[$settings_site_urls_info_domain_name_type_mappings_environment_type_from_sites_file]);

            break;
          }
        }
      }
    }
  }
}

/**
 * So if we haven't determined the environment by this point, then we now start
 * working backwards to try and identify the site by its URL. Totes obvsballs,
 * this won't work in Drush, so in an ideal world you would place an environment
 * file either in the multisite's directory, or in the sites directory above it.
 */
if (!defined('CURRENT_ENVIRONMENT')) {
  if (substr($_SERVER['HTTP_HOST'], (0 - strlen('.local'))) == '.local') {
    defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_LOCAL);
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_LOCAL);
  }
  elseif (substr($_SERVER['HTTP_HOST'], 0, strlen('dev.')) == 'dev.') {
    // We're on the Dev server:
    defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_DEV);
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_DEV);
  }
  elseif ((substr($_SERVER['HTTP_HOST'], 0, strlen('stag.')) == 'stag.') || (substr($_SERVER['HTTP_HOST'], 0, strlen('stage.')) == 'stage.')) {
    // We're on the RC server:
    defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_RC);
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_RC);
  }
  elseif (substr($_SERVER['HTTP_HOST'], 0, strlen('ra.')) == 'ra.') {
    // We're on the Dev server:
    defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_RA);
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_RA);
  }
}

/**
 * Still no idea what environment we're in? Set to unknown.
 */
if (!defined('CURRENT_ENVIRONMENT')) {
  // Cannot determine environment.
  defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_UNKNOWN);
}

/**
 * We have finished detecting the main environment type. Now we load in any
 * environment-specific settings file, if defined and the file is readable.
 */
if (defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') && is_readable(ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH)) {
  include ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH;
}

/**
 * Now we need to look at sub-environment types; for example, is this an
 * Acquia environment, or are we running a local development environment,
 * for example?
 *
 * If we're in an Acquia environment, set a flag and load its settings if
 * available.
 */
if (!empty($_ENV['AH_SITE_ENVIRONMENT'])) {
  defined('CURRENT_SUBENVIRONMENT') || define('CURRENT_SUBENVIRONMENT', SUBENVIRONMENT_TYPE_ACQUIA);

  if (is_readable(SETTINGS_FILE_PATH . SETTINGS_FILE_ACQUIA)) {
    include SETTINGS_FILE_PATH . SETTINGS_FILE_ACQUIA;
  }
}

/**
 * If we're in a Mamp environment, set a flag and load its settings if
 * available.
 */
if (!empty($GLOBALS['_ENV']['_']) && (strpos($GLOBALS['_ENV']['_'], 'MAMP') !== 0)) {
  defined('CURRENT_SUBENVIRONMENT') || define('CURRENT_SUBENVIRONMENT', SUBENVIRONMENT_TYPE_MAMP);

  if (is_readable(SETTINGS_FILE_PATH . SETTINGS_FILE_MAMP)) {
    include SETTINGS_FILE_PATH . SETTINGS_FILE_MAMP;
  }
}

/**
 * If the subenvironment isn't known, set the unknown flag now.
 */
defined('CURRENT_SUBENVIRONMENT') || define('CURRENT_SUBENVIRONMENT', SUBENVIRONMENT_TYPE_UNKNOWN);
