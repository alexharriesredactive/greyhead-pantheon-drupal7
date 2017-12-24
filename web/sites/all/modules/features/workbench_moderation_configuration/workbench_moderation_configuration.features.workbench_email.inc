<?php
/**
 * @file
 * workbench_moderation_configuration.features.workbench_email.inc
 */

/**
 * Implements hook_workbench_email_export().
 */
function workbench_moderation_configuration_workbench_email_export() {
  $items = array(
    'draft:needs_review:administrator' => array(
      'from_name' => 'draft',
      'to_name' => 'needs_review',
      'subject' => NULL,
      'message' => NULL,
      'author' => 0,
      'automatic' => 1,
      'role' => 'administrator',
    ),
    'needs_review:draft:original author' => array(
      'from_name' => 'needs_review',
      'to_name' => 'draft',
      'subject' => '[site:name]: your page has been processed',
      'message' => 'Hello,

Unfortunately, the page you submitted for approval has not been approved - please go to the page and view any moderation comments to find out why it was not published:

[current-page:title]
[current-page:url]

Alternatively, you can find the page on the content overview, at:
[site:url]/admin/content

Best wishes,

[site:name]',
      'author' => 1,
      'automatic' => 1,
      'role' => 'original author',
    ),
    'needs_review:published:administrator' => array(
      'from_name' => 'needs_review',
      'to_name' => 'published',
      'subject' => NULL,
      'message' => NULL,
      'author' => 0,
      'automatic' => 1,
      'role' => 'administrator',
    ),
    'needs_review:published:original author' => array(
      'from_name' => 'needs_review',
      'to_name' => 'published',
      'subject' => '[site:name]: your page has been processed',
      'message' => 'Hello,

The page you submitted for approval has been approved:

[current-page:title]
[current-page:url]

Best wishes,

[site:name]',
      'author' => 1,
      'automatic' => 1,
      'role' => 'original author',
    ),
  );
  return $items;
}
