<?php
/**
 * Drupal database configuration file - local_databases.template.php
 *
 * This file should be renamed to "local_databases.php" and moved into the
 * project root (i.e. one directory above Drupal's index.php).
 *
 * Note that the key of the configuration array ("{{MULTISITE_IDENTIFIER}}")
 * maps to the MULTISITE_IDENTIFIER constant, but this script is only intended
 * for use where one project checkout maps to one Drupal website; for security,
 * performance, and to preserve the general sanity of all developers concerned,
 * we don't run Drupal multisites except for local development purposes.
 *
 * An example of a populated array:
 *
 * $local_databases = array(
 *   'awesome-website' => array(
 *     'dev.awesome-website.greyheaddev.com' => array(
 *       'database' => 'awesome_website',
 *       'username' => 'awesome_website_user',
 *       'password' => 'awesome_website_password',
 *     ),
 *   ),
 * );
 *
 * Each database can specify the following keys; if not specified, the default
 * value (shown next to each key) will be used:
 *
 * database:  ''
 * username:  ''
 * password:  ''
 * host:      '127.0.0.1'
 * port:      ''
 * driver:    'mysql'
 * prefix:    ''
 *
 * By:   Alex Harries
 * Date: 18th March 2016
 */

/**
 * @TODO: script configuration of this file.
 */

$local_databases = [
  // This comment and the two lines below must stay here for automagic deployments to work :)
  // '{{AD_MULTISITE_IDENTIFIER}}' => array('{{AD_DOMAIN}}' => array('database' => '{{AD_DATABASENAME}}', 'username' => '{{AD_DATABASEUSERNAME}}', 'password' => '{{AD_DATABASEPASSWORD}}')),
  // {{BUILDSHINSERT}}

  // Example on one line:
  // 'myawesomewebsite' => array('myawesomewebsite.co.uk' => array('database' => 'myawesomewebsite_db', 'username' => 'myawesomewebsite_user', 'password' => 'websitesaremyawesome')),

  // Note the use of the "AD_" prefix to distinguish the variables above from
  // the search-replaceable strings, below, which are used by a different
  // script.

  // The following lines can be added to if you want to manually configure
  // multiple sites on one codebase, e.g. in a local development environment.
  '{{MULTISITE_IDENTIFIER}}' => [
    '{{DOMAIN}}' => [
      'database' => '{{DATABASENAME}}',
      'username' => '{{DATABASEUSERNAME}}',
      'password' => '{{DATABASEPASSWORD}}',
      'port' => '3306',
    ],
  ],

  // E.g. uncomment the below array block to add another multisite for this
  // build.
//  '{{MULTISITE_IDENTIFIER}}' => array(
//    '{{DOMAIN}}' => array(
//      'database' => '{{DATABASENAME}}',
//      'username' => '{{DATABASEUSERNAME}}',
//      'password' => '{{DATABASEPASSWORD}}',
//      'port' => '3306',
//    ),
//  ),
];
