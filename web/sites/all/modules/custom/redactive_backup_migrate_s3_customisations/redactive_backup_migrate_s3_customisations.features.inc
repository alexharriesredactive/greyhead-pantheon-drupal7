<?php

/**
 * @file
 * redactive_backup_migrate_s3_customisations.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function redactive_backup_migrate_s3_customisations_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "backup_migrate" && $api == "backup_migrate_exportables") {
    return array("version" => "1");
  }
}
