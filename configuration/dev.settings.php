<?php

/**
 * Settings for use on dev servers.
 */

/**
 * Options for the mail_redirect module.
 */
$conf['mail_redirect_opt'] = "address";
$conf['mail_redirect_address'] = "alex+greyhead-redirected-email-dev@greyhead.co.uk";
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
