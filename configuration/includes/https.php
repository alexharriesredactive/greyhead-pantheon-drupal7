<?php

/**
 * @file
 * https.php
 *
 * This file should be loaded after redirects.php has been included so we
 * can redirect to https if desired.
 */

// Are we on an HTTPS request?
$greyhead_is_https = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off')
  || ($_SERVER['SERVER_PORT'] == 443);

defined('GREYHEAD_IS_HTTPS') || define('GREYHEAD_IS_HTTPS', $greyhead_is_https);

defined('GREYHEAD_HTTPS_INCLUDED') || define('GREYHEAD_HTTPS_INCLUDED', TRUE);

// If $_SERVER['HTTPS'] is TRUE but we explicitly don't want to run HTTPS,
// reset this value.
if (defined('GREYHEAD_SITE_DOES_NOT_SUPPORT_HTTPS') && GREYHEAD_SITE_DOES_NOT_SUPPORT_HTTPS) {
  $_SERVER['HTTPS'] = FALSE;
}
