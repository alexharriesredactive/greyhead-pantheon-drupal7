<?php

/**
 * @file
 * environment-type-detection-not-pantheon.php
 *
 * This file will use magic and witchcraft to work out the environment.
 */

switch (PANTHEON_ENVIRONMENT) {
  case 'dev':
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_DEV);
    break;

  case 'test':
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_RC);
    break;

  case 'live':
  default:
    defined('CURRENT_ENVIRONMENT') || define('CURRENT_ENVIRONMENT', ENVIRONMENT_TYPE_LIVE);
    break;
}

defined('CURRENT_SUBENVIRONMENT') || define('CURRENT_SUBENVIRONMENT', SUBENVIRONMENT_TYPE_PANTHEON);

if (is_readable(SETTINGS_FILE_PATH . SETTINGS_FILE_PANTHEON)) {
  include SETTINGS_FILE_PATH . SETTINGS_FILE_PANTHEON;
}
