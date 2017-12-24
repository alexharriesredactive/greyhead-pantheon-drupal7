<?php

/**
 * @file
 * conf-maillog-defaults.php
 */

/**
 * Set the defaults for the Maillog module.
 */
$conf['maillog_send'] = TRUE;
$conf['maillog_log'] = FALSE;
$conf['maillog_devel'] = FALSE;
$conf['greyhead_configuration']['overridden_variables'][] = 'maillog_send';
$conf['greyhead_configuration']['overridden_variables'][] = 'maillog_log';
$conf['greyhead_configuration']['overridden_variables'][] = 'maillog_devel';
