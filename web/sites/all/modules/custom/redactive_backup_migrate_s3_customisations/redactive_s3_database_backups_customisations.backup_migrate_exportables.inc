<?php

/**
 * @file
 * redactive_s3_database_backups_customisations.backup_migrate_exportables.inc
 */

/**
 * Implements hook_exportables_backup_migrate_destinations().
 */
function redactive_s3_database_backups_customisations_exportables_backup_migrate_destinations() {
  $export = array();

  $item = new stdClass();
  $item->disabled = FALSE; /* Edit this to true to make a default item disabled initially */
  $item->api_version = 1;
  $item->machine_name = 'redactive_s3_database_backups';
  $item->name = 'Redactive S3 Database Backups';
  $item->subtype = 's3-compatible';
  $item->location = 'https://s3.amazonaws.com/redactive-database-backups';
  $item->settings = array(
    'file_directory' => 'backup-migrate-unsanitised',
    's3_timeout' => '',
    's3_proxy' => '',
    's3_min_part_size' => '',
  );
  $export['redactive_s3_database_backups'] = $item;

  return $export;
}

/**
 * Implements hook_exportables_backup_migrate_schedules().
 */
function redactive_s3_database_backups_customisations_exportables_backup_migrate_schedules() {
  $export = array();

  $item = new stdClass();
  $item->disabled = FALSE; /* Edit this to true to make a default item disabled initially */
  $item->api_version = 1;
  $item->machine_name = 'redactive_s3_database_backups_daily';
  $item->name = 'Redactive S3 Database Backups Daily';
  $item->source_id = 'db';
  $item->destination_id = 'redactive_s3_database_backups';
  $item->copy_destination_id = '';
  $item->profile_id = 'default';
  $item->keep = -2;
  $item->period = 43200;
  $item->enabled = TRUE;
  $item->cron = 'builtin';
  $item->cron_schedule = '0 4 * * *';
  $export['redactive_s3_database_backups_daily'] = $item;

  return $export;
}
