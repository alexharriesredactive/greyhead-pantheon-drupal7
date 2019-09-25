<?php

/**
 * @file
 * redactive_s3_database_backups_customisations.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function redactive_s3_database_backups_customisations_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "backup_migrate" && $api == "backup_migrate_exportables") {
    return array("version" => "1");
  }
}
