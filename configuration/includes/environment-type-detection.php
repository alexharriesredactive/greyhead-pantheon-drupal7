<?php

/**
 * @file
 * environment-type-detection.php
 *
 * This file contains the code which determines what environment we're using.
 *
 * This allows us to override various configuration variables for e.g. local
 * development environments, dev, staging server, etc.
 */

/**
 * Are we on Pantheon?
 */
if (defined('PANTHEON_ENVIRONMENT')) {
  include SETTINGS_FILE_PATH . 'includes/environment-type-detection-pantheon.php';
}
else {
  include SETTINGS_FILE_PATH . 'includes/environment-type-detection-not-pantheon.php';
}
