<?php

/**
 * @file
 * conf-captcha-disable.php
 *
 * Allows you to disable CAPTCHA challenges depending on the environment.
 *
 * By default, we disable CAPTCHA challenges in local environments.
 *
 * You can add additional environments to ignore CAPTCHA here; for example,
 * we disable dev and stage envs because Zoocha don't add their dev and
 * stage envs' domain names into their reCAPTCHA config, so without this
 * the login form is broken.
 *
 * Note that you must explicitly include "ENVIRONMENT_TYPE_LOCAL" if you
 * set $conf['greyhead_disable_captcha_for_environments'] here.
 *
 * @example:
 *
    $conf['greyhead_disable_captcha_for_environments'] = [
      ENVIRONMENT_TYPE_LOCAL,
      ENVIRONMENT_TYPE_DEV,
      ENVIRONMENT_TYPE_STAGE,
    ];
 *
 * @see
 */

//$conf['greyhead_disable_captcha_for_environments'] = [
//  ENVIRONMENT_TYPE_LOCAL,
//  ENVIRONMENT_TYPE_DEV,
//  ENVIRONMENT_TYPE_STAGE,
//];
