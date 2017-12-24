<?php

/**
 * @file
 * Adestra Webform Admin Settings.
 */

/**
 * Return the Adestra webform overview page.
 */

function adestra_webform_admin_overview() {
  $webform_types = webform_variable_get('webform_node_types');

  $nodes = array();
  if ($webform_types) {
    $nodes = db_select('node', 'n')
      ->fields('n')
      ->condition('n.type', $webform_types, 'IN')
      ->execute()
      ->fetchAllAssoc('nid');
  }
  $render['webforms'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Title'),
      array(
        'data' => t('Operations'),
        'colspan' => 2,
      ),
    ),
  );

  $rows = array();
  foreach ($nodes as $node) {
    $render['webforms']['#rows'][] = array(
      l($node->title, 'node/' . $node->nid),
      l(t('View'), 'node/' . $node->nid . '/webform/adestra'),
      node_access('update', $node) ? l(t('Edit'), 'node/' . $node->nid . '/edit') : '',
    );
  }

  if (empty($render['webforms']['#rows'])) {
    $webform_types = webform_variable_get('webform_node_types');
    if (empty($webform_types)) {
      $message = t('Webform is currently not enabled on any content types.') . ' ' . t('Visit the <a href="!url">Webform settings</a> page and enable Webform on at least one content type.', array('!url' => url('admin/config/content/webform')));
    }
    else {
      $webform_type_list = webform_admin_type_list();
      $message = t('There are currently no webforms on your site. Create a !types piece of content.', array('!types' => $webform_type_list));
    }

    $render['#rows'][] = array(
      array('data' => $message, 'colspan' => 7),
    );
  }

  return $render;
}

function adestra_webform_summary($node) {
  if (empty($node->webform['components'])) {
    return t('This webform contains no components, please use the !form-components tab to add a new one.', array('!form-components' => l(t('Form Components'), 'node/' . $node->nid . '/webform')));
  }
  $render['attribute_table'] = array(
    '#theme' => 'table',
    '#caption' => t('Attribute Handling'),
    '#header' => array(
      t('Component'),
      t('Type'),
      t('Value Provided'),
      t('Value Not Provided'),
      t('Operations'),
    )
  );

  $render['subscribe_table'] = array(
    '#theme' => 'table',
    '#caption' => t('List Handling'),
    '#header' => array(
      t('Component'),
      t('Type'),
      t('Value Provided'),
      t('Value Not Provided'),
      t('Operations'),
    )
  );
  
  $render['unsub_table'] = array(
  '#theme' => 'table',
  '#caption' => t('Unsub List Handling'),
  '#header' => array(
    t('Component'),
    t('Type'),
    t('Value Provided'),
    t('Value Not Provided'),
    t('Operations'),
  )
  );

  foreach ($node->webform['components'] as $cid => $component) {

    $render['attribute_table']['#rows'][$cid] = array(
      'name' => $component['name'],
      'type' => $component['type'],
      'empty' => ((!empty($component['extra']['adestra']['attribute']['not_empty']['attribute']) && !empty($component['extra']['adestra']['attribute']['not_empty']['value'])) ? $component['extra']['adestra']['attribute']['not_empty']['attribute'] . " =><br />" . $component['extra']['adestra']['attribute']['not_empty']['value'] : ''),
      'not_empty' => ((!empty($component['extra']['adestra']['attribute']['empty']['attribute']) && !empty($component['extra']['adestra']['attribute']['empty']['value'])) ? $component['extra']['adestra']['attribute']['empty']['attribute'] . " =><br />" . $component['extra']['adestra']['attribute']['empty']['value'] : ''),
      'edit' => l(t('Edit'), 'node/' . $node->nid . '/webform/components/' . $cid, array('fragment' => 'edit-extra-adestra-attribute-type', 'query' => drupal_get_destination())),
    );

    $subscribe_empty = array();
    if (!empty($component['extra']['adestra']['subscribe']['not_empty']['sub_lists'])) {
      $subscribe_empty[]= t('Subscribe') . " =><br />" . $component['extra']['adestra']['subscribe']['not_empty']['sub_lists'];
    }
    if (!empty($component['extra']['adestra']['subscribe']['not_empty']['unsub_lists'])) {
      $subscribe_empty[]= t('Unsubscribe') . " =><br />" . $component['extra']['adestra']['subscribe']['not_empty']['unsub_lists'];
    }
    $subscribe_not_empty = array();
    if (!empty($component['extra']['adestra']['subscribe']['empty']['sub_lists'])) {
      $subscribe_not_empty[]= t('Subscribe') . " =><br />" . $component['extra']['adestra']['subscribe']['empty']['sub_lists'];
    }
    if (!empty($component['extra']['adestra']['subscribe']['empty']['unsub_lists'])) {
      $subscribe_not_empty[]= t('Unsubscribe') . " =><br />" . $component['extra']['adestra']['subscribe']['empty']['unsub_lists'];
    }
    $render['subscribe_table']['#rows'][$cid] = array(
      'name' => $component['name'],
      'type' => $component['type'],
      'empty' => implode("<br />", $subscribe_empty),
      'not_empty' => implode("<br />", $subscribe_not_empty),
      'edit' => l(t('Edit'), 'node/' . $node->nid . '/webform/components/' . $cid, array('fragment' => 'edit-extra-adestra-attribute-type', 'query' => drupal_get_destination())),
    );
    
    $unsub_empty = array();
    if (!empty($component['extra']['adestra']['unsub']['not_empty']['sub_unsublists'])) {
      $unsub_empty[]= t('Subscribe') . " =><br />" . $component['extra']['adestra']['unsub']['not_empty']['sub_unsublists'];
    }
    if (!empty($component['extra']['adestra']['unsub']['not_empty']['unsub_unsublists'])) {
      $unsub_empty[]= t('Unsubscribe') . " =><br />" . $component['extra']['adestra']['unsub']['not_empty']['unsub_unsublists'];
    }
    $unsub_not_empty = array();
    if (!empty($component['extra']['adestra']['unsub']['empty']['sub_unsublists'])) {
      $unsub_not_empty[]= t('Subscribe') . " =><br />" . $component['extra']['adestra']['unsub']['empty']['sub_unsublists'];
    }
    if (!empty($component['extra']['adestra']['unsub']['empty']['unsub_unsublists'])) {
      $unsub_not_empty[]= t('Unsubscribe') . " =><br />" . $component['extra']['adestra']['unsub']['empty']['unsub_unsublists'];
    }
    $render['unsub_table']['#rows'][$cid] = array(
        'name' => $component['name'],
        'type' => $component['type'],
        'empty' => implode("<br />", $unsub_empty),
        'not_empty' => implode("<br />", $unsub_not_empty),
        'edit' => l(t('Edit'), 'node/' . $node->nid . '/webform/components/' . $cid, array('fragment' => 'edit-extra-adestra-attribute-type', 'query' => drupal_get_destination())),
    );
  }
  $render['markup'] = array(
    '#markup' => t('To add a new component, please use the !form-components tab.', array('!form-components' => l(t('Form Components'), 'node/' . $node->nid . '/webform'))),
  );
  return $render;
}