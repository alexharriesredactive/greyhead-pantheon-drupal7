<?php

/**
 * @file
 * memcache.settings.php
 *
 * This file attempts to use Memcache as a memory cache.
 *
 * This file will only do anything in live and staging environments; in other
 * environments, including unknown, it will do nothing.
 */
define('MEMCACHE_SETTINGS_LOADED', TRUE);

if ((CURRENT_ENVIRONMENT === ENVIRONMENT_TYPE_LIVE) ||
  (CURRENT_ENVIRONMENT === ENVIRONMENT_TYPE_RC)) {
  /**
   * Memcached settings, which we only enable if Memcached is configured and
   * contactable.
   *
   * Note that the IP address of the server should be set in ./local_settings.php,
   * which should live alongside the ./local_databases.php. The default address is
   * 127.0.0.1:11211.
   */
  $memcache_is_available = FALSE;

  if (class_exists('Memcache')) {
    $memcache = new Memcache;
    // Get the list of memcache servers, if any have been configured; otherwise, use
    // 127.0.0.1:11211.
    if (!array_key_exists('memcache_servers', $conf)) {
      $conf['memcache_servers']['127.0.0.1:11211'] = 'default';
    }

    // Loop through all available servers and attempt to connect. If no connection
    // possible, we don't configure for memcache at all.
    foreach ($conf['memcache_servers'] as $address => $type) {
      if ($type == 'default') {
        // Check the connection, and OR the result against any previously-
        // successful checks.
        $memcache_is_available = $memcache_is_available || @$memcache->connect($address);
      }

      if ($memcache_is_available) {
        break;
      }
    }
  }

  if ($memcache_is_available) {
    // Set a constant so we know memcache settings have been loaded.
    define('MEMCACHE_AVAILABLE', TRUE);

    $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';

    // The 'cache_form' bin must be assigned to non-volatile storage.
    $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
    $conf['cache_default_class'] = 'MemCacheDrupal';
    $conf['memcache_key_prefix'] = $_SERVER['HTTP_HOST'] . '_';
    $conf['page_cache_without_database'] = TRUE;
    $conf['page_cache_invoke_hooks'] = FALSE;

    // List of bins, from https://www.drupal.org/node/1931794.
    $conf['memcache_bins'] = array(
      'cache_performance'     =>  'default', // performance_logging module
      'cache'                 =>  'default',
      'cache_admin_menu'      =>  'default',
      'cache_apachesolr'      =>  'default',
      'cache_block'           =>  'default',
      'cache_bml'             =>  'default',
      'cache_bootstrap'       =>  'default',
      'cache_field'           =>  'default',
      'cache_filter'          =>  'default',
//  'cache_form'            =>  'default',  // This cache bin should not be assigned to volatile storage
      'cache_image'           =>  'default',
      'cache_libraries'       =>  'default',
      'cache_mailchimp_user'  =>  'default',
      'cache_menu'            =>  'default',
      'cache_page'            =>  'default',
      'cache_path'            =>  'default',
      'cache_rules'           =>  'default',
      'cache_token'           =>  'default',
//  'cache_update'          =>  'default', // Core's update module expects this to be a database table
      'cache_views'           =>  'default',
      'cache_views_data'      =>  'default',
    );
  }
}
