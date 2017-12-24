<?php

/**
 * @file
 * redirects.php
 *
 * This file replaces the .htaccess redirects which were set up in the
 * Acquia Drupal 7 codebase.
 *
 * @todo These redirects should be split up into each site's settings.php
 * files.
 *
 * @see https://pantheon.io/docs/domains/ for code examples and documentation
 * on why we're doing this (Nginx, basically, which is no bad thing).
 *
 * @example Redirect greyhead.co.uk to bluehead.co.uk:
 * greyhead_configuration_redirect_domain('greyhead.co.uk', 'bluehead.co.uk');
 *
 * @example Redirect greyhead.co.uk/london to london.greyhead.co.uk:
 * greyhead_configuration_redirect_domain('greyhead.co.uk',
 *   'london.greyhead.co.uk', FALSE, '/london');
 *
 * @example Redirect greyhead.co.uk to bluehead.co.uk/manchester:
 * greyhead_configuration_redirect_domain('greyhead.co.uk', 'bluehead.co.uk',
 *   FALSE, '/', '/manchester');
 */

// Load HTTPS detection if not already loaded.
if (!defined('GREYHEAD_HTTPS_INCLUDED')) {
  $file_path = __DIR__ . '/https.php';
  if (file_exists($file_path) && is_readable($file_path)) {
    require $file_path;
  }
}

if (!function_exists('greyhead_configuration_redirect_subdirectory_same_domain')) {
  /**
   * Redirect a subdirectory to another directory on the same domain.
   *
   * @param string $old_subdirectory
   *   The subdirectory to redirect from, with a leading slash but no trailing
   *   slash, e.g. /old
   *
   * @param string $new_subdirectory
   *   The subdirectory on the same domain name to redirect to, with a leading
   *   slash but no trailing slash, e.g. /new
   *
   * @param bool $https
   *   Set to TRUE to redirect to an HTTPS site - e.g. https://www.newsite.com
   *   Set to FALSE to redirect to HTTP.
   *   Set to NULL to auto-detect.
   *
   * @param string|null $redirect_code_string
   *   If you want to override the redirect code (which is "HTTP/1.0 301 Moved
   *   Permanently") by default, provide it here.
   */
  function greyhead_configuration_redirect_subdirectory_same_domain($old_subdirectory, $new_subdirectory, $https = NULL, $redirect_code_string = NULL, $exact_path = FALSE) {
    greyhead_configuration_redirect_domain($_SERVER['HTTP_HOST'], $_SERVER['HTTP_HOST'], $https, $old_subdirectory, $new_subdirectory, $redirect_code_string, $exact_path);
  }
}

if (!function_exists('greyhead_configuration_redirect_domain')) {
  /**
   * Redirect an old domain name to a new one.
   *
   * @param string $old_domain
   *   The old domain name, without "http://" or trailing slash, e.g.
   *   www.old-example.com
   *
   * @param string $new_domain
   *   The new domain name, without "http://" or trailing slash, e.g.
   *   www.new-example.com
   *
   * @param bool $https
   *   Set to TRUE to redirect to an HTTPS site - e.g. https://www.newsite.com
   *   Set to FALSE to redirect to HTTP.
   *   Set to NULL to auto-detect.
   *
   * @param string $old_subdirectory
   *   If you want to only redirect from a subdirectory on the old
   *   domain, provide it here with a leading slash but no trailing slash, e.g
   *   /old - defaults to "/" which means "redirect the whole domain".
   *
   * @param string $new_subdirectory
   *   If you want to redirect to a subdirectory on the new domain,
   *   provide it here with a leading slash but no trailing slash, e.g. /new
   *   Defaults to "/" which means "redirect to the root of the domain".
   *
   * @param string|null $redirect_code_string
   *   If you want to override the redirect code (which is "HTTP/1.0 301 Moved
   *   Permanently") by default, provide it here.
   *
   * @param bool $exact_path
   *   Set to TRUE if you want to redirect only the exact subdirectory and not
   *   its children. For example, redirect /monkeys but not /monkeys/bananas
   *
   *   Note that if you want to redirect /monkeys and /monkeys/<something> but
   *   NOT /monkeysbananas, you need two calls to this function:
   *
   *   greyhead_configuration_redirect_domain('www.example.com', 'www.example.com', FALSE, '/monkeys', '/new-subdirectory', NULL, TRUE);
   *   greyhead_configuration_redirect_domain('www.example.com', 'www.example.com', FALSE, '/monkeys/', '/new-subdirectory', NULL, FALSE);
   */
  function greyhead_configuration_redirect_domain($old_domain, $new_domain, $https = NULL, $old_subdirectory = '/', $new_subdirectory = '/', $redirect_code_string = NULL, $exact_path = FALSE) {
    // Check if we're running via command line and exit if we are.
    if (php_sapi_name() == "cli") {
      return;
    }

    // Do we have a redirect code string?
    if (!is_null($redirect_code_string)) {
      $redirect_code_string = 'HTTP/1.0 301 Moved Permanently';
    }

    // Recursive loop detection - don't allow redirection if the old and new
    // domains are the same, and the old and new subdirectories are also the same.
    if (($old_domain == $new_domain) && ($old_subdirectory == $new_subdirectory)) {
      die('Recursive loop error: greyhead_configuration_redirect_domain() called with the same from and to domain and subdirectory (from domain: ' . $old_domain . ' - to domain: ' . $new_domain . ' - from directory: ' . $old_subdirectory . ' - to directory: ' . $new_subdirectory . ')');
    }

    // Are we on the old domain?
    if ($_SERVER['HTTP_HOST'] == $old_domain) {
      // Are we being asked to redirect only from a particular subdirectory?
      if (substr($_SERVER['REQUEST_URI'], 0, strlen($old_subdirectory)) == $old_subdirectory) {
        // Are we being asked to only redirect an exact subdirectory?
        if ($exact_path && ($_SERVER['REQUEST_URI'] !== $old_subdirectory)) {
          return;
        }

        // Create the new request URI by stripping the old directory and
        // adding any new directory.
        $request_uri = $new_subdirectory . substr($_SERVER['REQUEST_URI'], strlen($old_subdirectory));

        // Do we have an explicit HTTPS?
        if (is_null($https)) {
          $https = GREYHEAD_IS_HTTPS;
        }

        header($redirect_code_string);
        header('Location: http' . ($https ? 's' : '') . '://' . $new_domain . $request_uri);
        exit();
      }
    }
  }
}

// Include the redirector which catches hard-coded links to multisite
// directories.
require __DIR__ . '/redirects-all-multisite-file-requests-to-default.php';
