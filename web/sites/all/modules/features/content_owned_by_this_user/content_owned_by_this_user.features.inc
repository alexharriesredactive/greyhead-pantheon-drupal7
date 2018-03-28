<?php
/**
 * @file
 * content_owned_by_this_user.features.inc
 */

/**
 * Implements hook_views_api().
 */
function content_owned_by_this_user_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
