<?php
/**
 * @file
 * imageapi_optimize_settings.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function imageapi_optimize_settings_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
