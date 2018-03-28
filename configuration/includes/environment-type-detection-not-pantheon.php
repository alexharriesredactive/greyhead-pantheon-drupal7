<?php

/**
 * @file
 * environment-type-detection-not-pantheon.php
 *
 * This file will use Pantheon's environment variables to set the environment.
 */

/**
 * Get the list of live site URLs, if any, from the settings.site_urls.info
 * file.
 */
$site_urls_and_environment_types = greyhead_configuration_get_site_urls_and_environment_types([MULTISITE_IDENTIFIER]);

$conf['greyhead_configuration']['SETTINGS_SITE_URLS'] = $site_urls_and_environment_types['sites'];
$conf['greyhead_configuration']['ENVIRONMENT_TYPES'] = $site_urls_and_environment_types['environment_types'];

/**
 * Are we in Drush?
 */
if (!defined('CURRENT_ENVIRONMENT') && drupal_is_cli()) {
  defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_DRUSH);
  defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_DRUSH);
}

/**
 * Is the current domain name listed in settings.site_urls.info?
 */
if (!defined('CURRENT_ENVIRONMENT')
  && array_key_exists($_SERVER['HTTP_HOST'], $conf['greyhead_configuration']['ENVIRONMENT_TYPES'])) {
  switch ($conf['greyhead_configuration']['ENVIRONMENT_TYPES'][$_SERVER['HTTP_HOST']]) {
    case 'prod':
    case 'prod-nowww':
      // Define the current environment.
      define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_LIVE);

      // Define the path to the local settings file, if available.
      defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_LIVE);

      break;

    case 'stage':
    case 'stag':
    case 'rc':
      // Define the current environment.
      define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_RC);

      // Define the path to the local settings file, if available.
      defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_STAGE);

      break;

    case 'dev':
      // Define the current environment.
      define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_DEV);

      // Define the path to the local settings file, if available.
      defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_DEV);

      break;

    case 'local':
      // Define the current environment.
      define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_LOCAL);

      // Define the path to the local settings file, if available.
      defined('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH') || define('ENVSPECIFIC_SETTINGS_FILE_RELATIVE_PATH', SETTINGS_FILE_PATH . SETTINGS_FILE_LOCAL);

      break;

    default:
      // Define the current environment.
      define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_UNKNOWN);
      break;
  }
}

/**
 * Still no idea what environment we're in? Set to unknown.
 */
defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_UNKNOWN);

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
