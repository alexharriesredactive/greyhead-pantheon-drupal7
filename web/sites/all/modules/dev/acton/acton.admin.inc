<?php

/**
 * @file
 * Act-On administration forms.
 */

/**
 * Act-On administration page.
 */
function acton_admin_form($form, &$form_state) {
  // Act-On Configuration.
  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Act-On Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  #$form['configuration']['acton_url'] = array(
  #  '#type' => 'textfield',
  #  '#title' => t('Act-On Post Url'),
  #  '#description' => t("The url of the Act-On software."),
  #  '#default_value' => variable_get('acton_url', ''),
  #  '#required' => TRUE,
  #  '#size' => 60,
  #);

  $form['configuration']['add_url'] = array(
    '#type'  => 'textfield',
    '#description' => t("A post url configured on Act-On software."),
    '#title' => 'Post URL',
  );

  $form['configuration']['submit_add_url'] = array(
    '#type'   => 'submit',
    '#value'  => 'Add',
    '#submit' => array('acton_add_url_submit'),
  );

  $form['configuration']['acton_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug mode'),
    '#description' => t('Enabling debug mode will show which fields are being submitted to Act-On after a submit'),
    '#default_value' => variable_get('acton_debug', 0),
  );

  // populate post url and webform list
  $form['post_url_fieldset'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Act-On Post Urls'),
    '#collapsible'  => TRUE,
  );
  acton_post_urls($form);

  return system_settings_form($form);
}

/**
 * acton_post_urls()
 * @description
 *  get the configured post urls from the database
 * @param &$form
 */
function acton_post_urls(&$form) {
  $result = db_select('acton_post_url', 'pu')
              ->fields('pu')
              ->execute();
  
  // make a field set for each row and allow url adding
  while ($row = $result->fetchAssoc()) {
    $form['post_url_fieldset']['url_'.$row['id'].'_fieldset'] = array(
      '#type' => 'fieldset',
      '#attributes' => array('style'=>'background:#f1f1f1;'),
      '#title'   => t('post url'),
      '#collapsible' => False,
      '#collapsed' => False,
    );

    $form['post_url_fieldset']['url_'.$row['id'].'_fieldset']['url'] = array(
      '#markup'  => "<div><em>" . $row['url'] . "</em></div>",
    );

    // Show a list of added webforms
    $form['post_url_fieldset']['url_'.$row['id'].'_fieldset']['webform_fieldset'] = array(
      '#type'         => 'fieldset',
      '#title'        => 'Webforms',
      '#collapsible'  => True,
      '#collapsed'    => True,
    );
    if ($options = acton_webforms_checkbox_options($row['id'])) {
      $form['post_url_fieldset']['url_'.$row['id'].'_fieldset']['webform_fieldset']['webforms'] = array(
        '#type'     => 'tableselect',
        '#header'   => array('webform'),
        '#options'  => $options,
      );
      $form['post_url_fieldset']['url_'.$row['id'].'_fieldset']['webform_fieldset']['delete_webforms'] = array(
        '#type'   => 'submit',
        '#submit' => array('acton_delete_webforms'),
        '#value'  => t('Delete'),
        '#name'   => 'delete_webform_' . $row['id'],
      );
    }

    /// Select box of available webforms
    $form['post_url_fieldset']['url_'.$row['id'].'_fieldset']['webform_fieldset']['add_new'] = array(
      '#type'     => 'select',
      '#options'  => acton_build_webform_options(), 
      '#title'    => t('Add Webform'),
    );
    $form['post_url_fieldset']['url_'.$row['id'].'_fieldset']['webform_fieldset']['submit'] = array(
      '#type'   => 'submit',
      '#submit' => array('acton_add_webform_submit'),
      '#value'  => t('Submit'),
      '#name'   => 'add_webform_' . $row['id'],
    );
    $form['post_url_fieldset']['url_'.$row['id'].'_fieldset']['delete'] = array(
      '#type'   => 'submit',
      '#submit' => array('acton_delete_post_url'),
      '#name'   => 'delete_post_url_' . $row['id'],
      '#value'  => t('Delete Post URL'),
    );
  }

  if (count($form['post_url_fieldset']) <= 3) {
    $form['post_url_fieldset']['message'] = array(
      '#markup' => '<span><em>Add some post urls...</em></span>'
    );
  }

}

/**
 * acton_webforms_checkbox_options()
 * @description
 *  return a list of checkbox options for webforms
 *  associated to specified post url.
 * @param Integer $id
 *  the Post URL ID
 */
function acton_webforms_checkbox_options($id) {
  $options = array();
  $webforms = acton_get_webforms_by_post_id($id);
  if (count($webforms)) {
    foreach($webforms as $webform) {
      $options[$webform->nid] = array(
        $webform->title,
      );
    }
  }
  return $options;
}

/**
 * acton_delete_webforms()
 * @description
 *  deletes selected webforms on the currently selected Post URL.
 * @param Array &$form
 * @param Array &$form_state
 */
function acton_delete_webforms(&$form, &$form_state) {
  foreach($form_state['values'] as $key=>$val) {
    preg_match('/delete_webform_([0-9])+/', $key, $matches);
    // find out which post url we're deleting from
    if (!empty($matches[1])) {
      $post_url_id = $matches[1];
      if (!empty($form_state['values']['webforms'])) {
        $webforms = $form_state['values']['webforms'];
        foreach($webforms as $key=>$val) {
          if($key == $val) {
            db_delete('acton_drupal_webforms') 
              ->condition('url_id', $post_url_id, '=')
              ->condition('nid', $val, '=')
              ->execute();
          }
        }
      }
      break;
    }
  }
}

/**
 * acton_delete_post_url()
 * @description
 *  deletes a post url and all associated child webform
 *  references.
 * @param Array &$form
 * @param Array &$form_state
 */
function acton_delete_post_url(&$form, &$form_state) {
  foreach($form_state['values'] as $key=>$val) {
    preg_match('/delete_post_url_([0-9])+/', $key, $matches);
    // find out which post url we're deleting
    if (!empty($matches[1])) {
      $post_url_id = $matches[1];
      # delete the post url
      db_delete('acton_post_url')
        ->condition('id', $post_url_id)
        ->execute();
      # delete child webforms
      db_delete('acton_drupal_webforms')
        ->condition('url_id', $post_url_id)
        ->execute();
    }
  }
}

/**
 * acton_build_webform_options()
 * @description
 *  return select input options with all available
 *  webforms.
 */
function acton_build_webform_options() {
  $options = array(0 => '-- select --');
  $nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type', 'webform', '=')
    ->execute()
    ->fetchCol();

  $webforms = node_load_multiple($nids);

  foreach($webforms as $webform) {
    $options[$webform->nid] = $webform->title;
  }

  return $options;
}  

/**
 * acton_get_webforms_by_post_id().
 * @description
 *  get an array of webforms associated with a Post URL
 * @param Array &$form
 * @param Array &$form_state
 */
function acton_get_webforms_by_post_id($id) {
  $webforms = array();
  $data = db_select('acton_drupal_webforms', 'wf')
  ->fields('wf')
  ->condition('url_id', $id, '=')
  ->execute()
  ->fetchAll();

  foreach($data as $row) {
    $webforms[] = node_load($row->nid);
  }

  return $webforms;
}

/**
 * acton_add_webform_submit().
 * @description
 *  handles submitting a new webform to an existing
 *  Post ULrl
 * @param Array &$form
 * @param Array &$form_state
 */
function acton_add_webform_submit(&$form, &$form_state) {
  foreach($form_state['values'] as $key=>$val) {
    preg_match('/add_webform_([0-9])+/', $key, $matches);
    // find out which post url we're adding to
    if (!empty($matches[1])) {
      $post_url_id = $matches[1];
      $webform_nid =  $form_state['values']['add_new'];
      // selected webform required
      if ($webform_nid) {
        // check the entry doesn't exist first
        $exists = db_select('acton_drupal_webforms', 'wf')
          ->fields('wf')
          ->condition('nid', $webform_nid, '=')
          ->condition('url_id', $post_url_id, '=')
          ->execute()
          ->fetchCol();
        // build if no existing entry
        if (!count($exists)) {
          db_insert('acton_drupal_webforms')
            ->fields(array(
              'nid'     => $webform_nid,
              'url_id'  => $post_url_id,))
            ->execute();
          drupal_set_message('Webform added to Post URL');
        }
        else {
          form_set_error('form', 'The webform has already been added to this Post URL');
        }
      }
      else {
        form_set_error('form', 'You must select a webform to add');
      }
      break;
    }
  }
}


/**
 * acton_add_url_submit().
 * @description
 *  handles submitting a new acton url
 * @param Array &$form
 * @param Array &$form_state
 */
function acton_add_url_submit(&$form, &$form_state) {
  $url         = filter_xss($form_state['values']['add_url']);
  $error       = FALSE;

  if (empty($url)) {
    form_set_error('form', 'A Post URL is required when adding a new URL');
    return;
  }

  $existing_url = db_select('acton_post_url', 'pu')
    ->fields('pu')
    ->condition('url', $url, '=')
    ->execute()
    ->fetchAssoc();

  if (!$existing_url) {
    db_insert('acton_post_url')
      ->fields(array(
        'url' => $url))
      ->execute();
    drupal_set_message('New Post URL Added');
    return;
  }

  form_set_error('form', 'This URL has already been added!');
}
