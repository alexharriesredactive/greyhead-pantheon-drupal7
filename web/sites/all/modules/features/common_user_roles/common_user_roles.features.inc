<?php
/**
 * @file
 * common_user_roles.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function common_user_roles_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
