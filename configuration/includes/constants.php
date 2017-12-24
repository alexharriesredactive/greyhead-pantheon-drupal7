<?php

/**
 * @file
 * constants.php
 *
 * Configuration constants.
 */

/**
 * Make sure we have a multisite identifier so we can use it to segregate
 * file directories.
 */
defined('MULTISITE_IDENTIFIER') || define('MULTISITE_IDENTIFIER', 'default');

define('SETTINGS_FILE_DRUSH', 'drush.settings.php');
define('SETTINGS_FILE_LOCAL', 'local-development.settings.php');
define('SETTINGS_FILE_DEV', 'dev.settings.php');
define('SETTINGS_FILE_RC', 'rc.settings.php');
define('SETTINGS_FILE_STAGE', 'rc.settings.php');
define('SETTINGS_FILE_RA', 'ra.settings.php');
define('SETTINGS_FILE_LIVE', 'live.settings.php');
define('SETTINGS_FILE_MAMP', 'mamp.settings.php');
define('SETTINGS_FILE_ACQUIA', 'acquia.settings.php');
define('SETTINGS_FILE_PANTHEON', 'pantheon.settings.php');

// Define our site types.
define('ENVIRONMENT_TYPE_LOCAL', 'local');
define('ENVIRONMENT_TYPE_DRUSH', 'Drush');
define('ENVIRONMENT_TYPE_DEV', 'development');
define('ENVIRONMENT_TYPE_RC', 'release candidate');
define('ENVIRONMENT_TYPE_RA', 'RA');
define('ENVIRONMENT_TYPE_STAGE', 'staging');
define('ENVIRONMENT_TYPE_LIVE', 'production');
define('ENVIRONMENT_TYPE_UNKNOWN', 'unknown');
define('SUBENVIRONMENT_TYPE_MAMP', 'MAMP');
define('SUBENVIRONMENT_TYPE_UNKNOWN', 'unknown');
define('SUBENVIRONMENT_TYPE_ACQUIA', 'Acquia');
define('SUBENVIRONMENT_TYPE_PANTHEON', 'Pantheon');

// Define where we can find the database settings processor.
define('DATABASE_SETTINGS_FILENAME', 'database.settings.php');
