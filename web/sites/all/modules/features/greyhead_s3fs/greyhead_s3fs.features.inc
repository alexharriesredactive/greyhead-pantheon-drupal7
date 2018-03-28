<?php
/**
 * @file
 * greyhead_s3fs.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function greyhead_s3fs_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
