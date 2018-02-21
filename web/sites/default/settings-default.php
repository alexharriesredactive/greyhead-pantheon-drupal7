<?php

/**
 * @file
 * Drupal site-specific configuration file.
 */

// Explicitly state the site's identifier. This is also used as the name of the
// S3 bucket.
define('SITE_MACHINE_NAME', 'clarion-default');

// Work out the directory name of this multisite:
define('MULTISITE_IDENTIFIER', substr(dirname(__FILE__), strrpos(dirname(__FILE__), '/') + 1));

// Include the common settings.php file. Paths relative to index.php!
require_once '../configuration/common.settings.php';

/**
 * S3FS module config.
 */

// Set the bucket name.
$conf['s3fs_bucket'] = SITE_MACHINE_NAME; //'clarion-default';

// If we know the env and aren't in production or drush use the test bucket.
// @todo: How can we tell if we're in Drush on live?
if (defined('CURRENT_ENVIRONMENT') && defined('ENVIRONMENT_TYPE_LIVE') && (CURRENT_ENVIRONMENT !== ENVIRONMENT_TYPE_LIVE) && (CURRENT_ENVIRONMENT !== ENVIRONMENT_TYPE_UNKNOWN) && (CURRENT_ENVIRONMENT !== ENVIRONMENT_TYPE_DRUSH)) {
  // Disabled for now: $conf['s3fs_bucket'] .= '-test';
}

/**
 * Redirects.
 *
 * @see /configuration/includes/redirects.php for examples.
 */

/**
 * Drupal will automagically add database connection details and the Drupal
 * hash salt below during install.php.
 *
 * You will need to move the connection array into /local_databases.php.
 *
 * You can delete the Drupal hash salt as it will be provided in the local
 * configuration system.
 *
 * @todo: can automate this process so we don't have this manual step.
 */
