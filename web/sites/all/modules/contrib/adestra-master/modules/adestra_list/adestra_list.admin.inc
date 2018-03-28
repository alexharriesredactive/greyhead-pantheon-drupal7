<?php

/**
 * @file
 * Adestra List Admin Settings.
 */

/**
 * Return the Adestra list overview page.
 */

function adestra_list_admin_overview() {

  $header = array(
    'id' => t('List ID'),
    'name' => t('Name'),
    'description' => t('Description'),
    'count' => t('Count'),
  );

  $lists = adestra_fetch_lists();
  $paged_list = adestra_fetch_lists(array(
    'page' => isset($_GET['page']) ? $_GET['page'] + 1 : 1,
    'page_size' => 20,
  ));

  // Prepare the list of nodes.
  $options = array();
  if (is_array($paged_list)) {
    foreach ($paged_list as $list) {
      $options[$list['id']] = array(
        'id' => $list['id'],
        'name' => $list['name'],
        'description' => $list['description'],
        'count' => $list['count'],
      );
    }
  }

  $render['lists'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No content available.'),
  );

  pager_default_initialize(count($lists), 20);
  $render['pager'] = array('#theme' => 'pager');
  return $render;

}