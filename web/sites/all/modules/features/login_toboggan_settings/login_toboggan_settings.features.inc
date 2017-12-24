<?php
/**
 * @file
 * login_toboggan_settings.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function login_toboggan_settings_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
