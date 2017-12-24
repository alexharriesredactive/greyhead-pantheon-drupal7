<?php

/**
 * Settings for use on RC servers.
 */

/**
 * Options for the mail_redirect module.
 */
$conf['mail_redirect_opt'] = "address";
$conf['mail_redirect_address'] = "alex+greyhead-redirected-email-rc@greyhead.co.uk";

/**
 * Override Maillog module settings for staging.
 */
$conf['maillog_send'] = FALSE;
$conf['maillog_log'] = TRUE;
$conf['maillog_devel'] = TRUE;

/**
 * Prevent robots crawling development sites.
 */
$conf['robotstxt'] = '# Robots.txt overridden because you\'re not on a live environment.
Disallow: /
';

/**
 * Some RC/live settings.
 */
$conf['error_level'] = '0';
//$conf['preprocess_css'] = '1'; <-- We don't set these because we set CSS and JS in a Feature instead.
//$conf['preprocess_js'] = '1';
//$conf['page_compression'] = '1';
//$conf['block_cache'] = '1';
//$conf['cache'] = '1'; <-- We don't set this because we use Boost instead.

/**
 * Load memcache, if available.
 */
if (!defined('MEMCACHE_SETTINGS_LOADED') && is_readable(SETTINGS_FILE_PATH . 'includes/local_settings.php')) {
  include_once(SETTINGS_FILE_PATH . 'includes/memcache.settings.php');
}
