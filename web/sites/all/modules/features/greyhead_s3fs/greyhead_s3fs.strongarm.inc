<?php
/**
 * @file
 * greyhead_s3fs.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function greyhead_s3fs_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'file_default_scheme';
  $strongarm->value = 's3';
  $export['file_default_scheme'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_awssdk2_default_cache_config';
  $strongarm->value = '';
  $export['s3fs_awssdk2_default_cache_config'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_awssdk2_use_instance_profile';
  $strongarm->value = 0;
  $export['s3fs_awssdk2_use_instance_profile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_cache_control_header';
  $strongarm->value = '';
  $export['s3fs_cache_control_header'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_domain';
  $strongarm->value = '';
  $export['s3fs_domain'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_encryption';
  $strongarm->value = 'AES256';
  $export['s3fs_encryption'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_hostname';
  $strongarm->value = '';
  $export['s3fs_hostname'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_ignore_cache';
  $strongarm->value = 0;
  $export['s3fs_ignore_cache'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_migrate_backup';
  $strongarm->value = 1;
  $export['s3fs_migrate_backup'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_migrate_cron_limit';
  $strongarm->value = '1000';
  $export['s3fs_migrate_cron_limit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_migrate_fields';
  $strongarm->value = array();
  $export['s3fs_migrate_fields'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_migrate_newer_first';
  $strongarm->value = 1;
  $export['s3fs_migrate_newer_first'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_migrate_newer_first_orig';
  $strongarm->value = 1;
  $export['s3fs_migrate_newer_first_orig'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_migrate_scope';
  $strongarm->value = 'publicprivate';
  $export['s3fs_migrate_scope'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_no_redirect_derivatives';
  $strongarm->value = 0;
  $export['s3fs_no_redirect_derivatives'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_no_rewrite_cssjs';
  $strongarm->value = 0;
  $export['s3fs_no_rewrite_cssjs'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_presigned_urls';
  $strongarm->value = '';
  $export['s3fs_presigned_urls'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_private_folder';
  $strongarm->value = 's3fs-private';
  $export['s3fs_private_folder'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_public_folder';
  $strongarm->value = 's3fs-public';
  $export['s3fs_public_folder'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_region';
  $strongarm->value = 'eu-west-2';
  $export['s3fs_region'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_root_folder';
  $strongarm->value = '';
  $export['s3fs_root_folder'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_saveas';
  $strongarm->value = '';
  $export['s3fs_saveas'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_torrents';
  $strongarm->value = '';
  $export['s3fs_torrents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_use_cname';
  $strongarm->value = 0;
  $export['s3fs_use_cname'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_use_customhost';
  $strongarm->value = 0;
  $export['s3fs_use_customhost'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_use_https';
  $strongarm->value = 1;
  $export['s3fs_use_https'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_use_s3_for_private';
  $strongarm->value = 0;
  $export['s3fs_use_s3_for_private'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 's3fs_use_s3_for_public';
  $strongarm->value = 0;
  $export['s3fs_use_s3_for_public'] = $strongarm;

  return $export;
}
