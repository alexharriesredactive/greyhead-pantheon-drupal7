<?php

/**
 * Settings for use when Drupal is invoked via the CLI.
 *
 * If you need to invoke Drupal as dev, rc or live, please specify the full
 * domain name of the site, instead of just the directory name this site exists
 * at under /sites/, e.g. "drush --site=epsilon-churchwalk.devfourstudio.co.uk"
 * instead of "drush --site=epsilon-churchwalk".
 */

// Override the default database IP address:
include 'includes/mamp-localhost-db-address-override.php';
