<?php

/**
 * @file
 * redirects-all-multisite-file-requests-to-default.php
 *
 * This file redirects all multisite files directory requests to
 * /sites/default/files.
 *
 * This is to catch hard-coded links such as image links in a multisite's
 * files directory when the multisite has now been moved to sites/default.
 *
 * Oh Pantheon, what a pain in the bum you are...
 */

// Get the request URI as an array.
if (!empty($_SERVER['REQUEST_URI'])) {
  $request_uri_parts = explode('/', $_SERVER['REQUEST_URI']);

  // Do we have at least four slashes in the URL? E.g. /sites/monkey/files will
  // count as three; /sites/banana/files/trevor will be four.
  if (count($request_uri_parts) >= 4) {
    // The first array key should be empty; the second should be "sites", and
    // the fourth should be "files".
    if (empty($request_uri_parts[0]) && ($request_uri_parts[1] == 'sites')
      && ($request_uri_parts[3] == 'files')) {
      // Make sure [2] isn't "all" or "default".
      if (!in_array($request_uri_parts[2], ['all', 'default'])) {
        // Ok, issue the redirect.
        greyhead_configuration_redirect_subdirectory_same_domain('/sites/' . $request_uri_parts[2] . '/files/', '/sites/default/files/');
      }
    }
  }
}
