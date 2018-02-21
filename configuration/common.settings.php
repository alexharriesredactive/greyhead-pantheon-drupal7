<?php

/**
 * @file
 * Drupal site-specific configuration file.
 *
 * IMPORTANT NOTE:
 * This file may have been set to read-only by the Drupal installation program.
 * If you make changes to this file, be sure to protect it again after making
 * your modifications. Failure to remove write permissions to this file is a
 * security risk.
 *
 * The configuration file to be loaded is based upon the rules below. However
 * if the multisite aliasing file named sites/sites.php is present, it will be
 * loaded, and the aliases in the array $sites will override the default
 * directory rules below. See sites/example.sites.php for more information about
 * aliases.
 *
 * The configuration directory will be discovered by stripping the website's
 * hostname from left to right and pathname from right to left. The first
 * configuration file found will be used and any others will be ignored. If no
 * other configuration file is found then the default configuration file at
 * 'sites/default' will be used.
 *
 * For example, for a fictitious site installed at
 * http://www.drupal.org:8080/mysite/test/, the 'settings.php' file is searched
 * for in the following directories:
 *
 * - sites/8080.www.drupal.org.mysite.test
 * - sites/www.drupal.org.mysite.test
 * - sites/drupal.org.mysite.test
 * - sites/org.mysite.test
 *
 * - sites/8080.www.drupal.org.mysite
 * - sites/www.drupal.org.mysite
 * - sites/drupal.org.mysite
 * - sites/org.mysite
 *
 * - sites/8080.www.drupal.org
 * - sites/www.drupal.org
 * - sites/drupal.org
 * - sites/org
 *
 * - sites/default
 *
 * Note that if you are installing on a non-standard port number, prefix the
 * hostname with that number. For example,
 * http://www.drupal.org:8080/mysite/test/ could be loaded from
 * sites/8080.www.drupal.org.mysite.test/.
 *
 * @see example.sites.php
 * @see conf_path()
 */

/**
 * Make sure $conf is defined.
 */
if (empty($conf) || !is_array($conf)) {
  $conf = [];
}

/**
 * If we're in Pantheon, extract the Pressflow Settings into $conf early so
 * we can override them later.
 *
 * Also @see https://pantheon.io/docs/read-environment-config/
 */
if (defined('PANTHEON_ENVIRONMENT')) {
  // Extract Pantheon environmental configuration for Domain Access.
  if (!empty($_SERVER['PRESSFLOW_SETTINGS'])) {
    extract(json_decode($_SERVER['PRESSFLOW_SETTINGS'], TRUE));
  }
}

/**
 * Initialise the $conf['greyhead_configuration'] array.
 */
$conf['greyhead_configuration'] = [];

/**
 * The overridden_variables array can be used to list which Drupal variables
 * have been overridden in this configuration stage; the
 * greyhead_customisations.module can then form_alter these fields and disable
 * them to let administrators know they've been overridden, and can't be
 * changed.
 */
$conf['greyhead_configuration']['overridden_variables'] = [];

/**
 * Include the correct settings file. Paths relative to index.php!
 */
define('SETTINGS_FILE_PATH', GREYHEAD_DRUPAL_ROOT . '/' . GREYHEAD_CONFIGURATION_DIRECTORY_LOCATION);

/**
 * Include the HTTPS detection bits and bobs.
 */
include SETTINGS_FILE_PATH . 'includes/https.php';

/**
 * Include the constants file to set all the other constants.
 */
include SETTINGS_FILE_PATH . 'includes/constants.php';

/**
 * Load the configuration which is normally found in the vanilla Drupal
 * settings.php file.
 */
include SETTINGS_FILE_PATH . 'includes/conf-drupal-settings-php-settings.php';

/**
 * Load the configuration which is normally found in the vanilla Drupal
 * settings.php file.
 */
include SETTINGS_FILE_PATH . 'includes/conf-s3fs.php';

/**
 * Fast 404 - set $conf['greyhead_drupal_fast_404_enabled'] to TRUE to enable.
 */
$conf['greyhead_drupal_fast_404_enabled'] = FALSE;

$file_path = SETTINGS_FILE_PATH . 'includes/conf-fast-404.php';
if ($conf['greyhead_drupal_fast_404_enabled'] && file_exists($file_path)
  && is_readable($file_path)) {
  include $file_path;
}

/**
 * Link field TLDs adjustments.
 */
include SETTINGS_FILE_PATH . 'includes/conf-link-fields-tlds.php';

/**
 * Set the defaults for the Maillog module.
 */
include SETTINGS_FILE_PATH . 'includes/conf-maillog-defaults.php';

/**
 * Get the file location defaults.
 */
include SETTINGS_FILE_PATH . 'includes/conf-default-file-locations.php';

/**
 * Include CAPTCHA overrides if configured.
 */
include SETTINGS_FILE_PATH . 'includes/conf-captcha-disable.php';

/**
 * Detect the current environment.
 */
include SETTINGS_FILE_PATH . 'includes/environment-type-detection.php';

/**
 * Now we need to load the database file.
 */
if (!defined('DATABASE_SETTINGS_INCLUDED')
  && defined('DATABASE_SETTINGS_FILENAME')
  && !defined('PANTHEON_ENVIRONMENT')) {

  // Load in the database settings.
  include SETTINGS_FILE_PATH . DATABASE_SETTINGS_FILENAME;

  // Leave a flag so we know we have included database.settings.php.
  define('DATABASE_SETTINGS_INCLUDED', TRUE);
}

/**
 * Finally, allow local overrides now, in a file alongside local_databases.php
 * called local_settings.php (i.e. one level above Drupal's index.php).
 *
 * @see local_settings.template.php for more info.
 *
 * @todo: This should be configurable in the same way that GREYHEAD_CONFIGURATION_DIRECTORY_LOCATION works.
 */
$file_path = GREYHEAD_DRUPAL_ROOT . '/../local_settings.php';
if (!defined('LOCAL_SETTINGS_INCLUDED')
  && file_exists($file_path)
  && is_readable($file_path)) {
  include $file_path;

  define('LOCAL_SETTINGS_INCLUDED', 'LOCAL_SETTINGS_INCLUDED');
}

/**
 * Allow the inclusion of a Drupal local settings file.
 */
$file_path = GREYHEAD_DRUPAL_ROOT . '/' . conf_path() . '/settings.local.php';
if (file_exists($file_path)
  && is_readable($file_path)) {
  include $file_path;
}

/**
 * Leave a marker to indicate we've included these settings.
 */
defined('GREYHEAD_CONFIGURATION_LOADED') || define('GREYHEAD_CONFIGURATION_LOADED', TRUE);
