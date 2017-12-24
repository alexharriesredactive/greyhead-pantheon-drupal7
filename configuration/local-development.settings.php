<?php

/**
 * Local development environment settings.
 */

//$conf['theme_debug'] = TRUE;

/**
 * Options for the mail_redirect module.
 */
$conf['mail_redirect_opt'] = "address";
$conf['mail_redirect_address'] = "alex+greyhead-redirected-email-localdev@greyhead.co.uk";
$conf['greyhead_configuration']['overridden_variables'][] = 'mail_redirect_opt';
$conf['greyhead_configuration']['overridden_variables'][] = 'mail_redirect_address';

/**
 * Override Maillog module settings for dev.
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
$conf['greyhead_configuration']['overridden_variables'][] = 'robotstxt';

if (defined('CURRENT_SUBENVIRONMENT') && (CURRENT_SUBENVIRONMENT == 'MAMP')) {
  // Override the default database IP address:
  include SETTINGS_FILE_PATH . 'includes/mamp-localhost-db-address-override.php';

  // Override the temporary files location on MAMP:
//  $conf['file_temporary_path'] = '/Applications/MAMP/tmp/php';
}

/**
 * Disable the Shield module in local environments.
 */
$conf['shield_user'] = $conf['shield_pass'] = '';
$conf['greyhead_configuration']['overridden_variables'][] = 'shield_user';
$conf['greyhead_configuration']['overridden_variables'][] = 'shield_pass';

/**
 * Some development settings... :)
 */
$conf['error_level'] = '2';
$conf['greyhead_configuration']['overridden_variables'][] = 'error_level';

//$conf['preprocess_css'] = '0'; <-- We don't set these because we set CSS and JS in a Feature instead.
//$conf['preprocess_js'] = '0';
//$conf['page_compression'] = '0';
//$conf['block_cache'] = '0';

$conf['cache'] = '0'; // Gotcha warning - don't force this to TRUE or you will break Boost.module.
$conf['greyhead_configuration']['overridden_variables'][] = 'cache';

$conf['advagg_enabled'] = FALSE;
$conf['greyhead_configuration']['overridden_variables'][] = 'advagg_enabled';
