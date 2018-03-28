<?php
/**
 * @file: mamp-localhost-db-address-override.php
 *
 *      This file changes the localhost database IP from 127.0.0.1 to localhost,
 *      which is necessary to allow Drupal running on MAMP installations to be
 *      able to connect to MAMP's MySQL socket.
 *
 *      This doesn't _SEEM_ to negatively affect Drupal/Drush when running on
 *      other *nix installations, touch wood, etc.
 */
if (isset($databases['default']) && $databases['default']['default']['host'] == '127.0.0.1') {
  $databases['default']['default']['host'] = 'localhost';
}
