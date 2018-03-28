<?php
/**
 * @file
 * Adestra Contact Admin Settings.
 */

/**
 * Return the Adestra contact overview page.
 */

function adestra_contact_admin_overview() {

  $header = array(
    'id' => t('Contact ID'),
    'email' => t('Email'),
  );

  $contacts = adestra_fetch_contacts();
  $paged_contacts = adestra_fetch_contacts(NULL, array(), array(
    'page' => isset($_GET['page']) ? $_GET['page'] + 1 : 1,
    'page_size' => 20,
  ));

  // Prepare the list of nodes.
  $options = array();
  if (is_array($paged_contacts)) {
    foreach ($paged_contacts as $contact) {
      $options[$contact['id']] = array(
        'id' => $contact['id'],
        'email' => $contact['email'],
      );
    }
  }

  $render['contacts'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No content available.'),
  );

  pager_default_initialize(count($contacts), 20);
  $render['pager'] = array('#theme' => 'pager');
  return $render;
}

function adestra_contact_administer_contact($account) {

  $id = adestra_contact_get_adestra_id($account, TRUE);
  $contact = adestra_fetch_contact($id);

  $headers = array(
    'key' => t('Key'),
    'value' => t('Value'),
  );

  $rows = array();
  if (is_array($contact)) {
    foreach ($contact as $key => $value) {
      $rows[] = array(
        'key' => $key,
        'value' => $value
      );
    }
  }

  $render['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('User does not have an Adestra account. Select Create to add them.'),
  );
  if (user_access('administer adestra contacts')) {
    $render['form'] = drupal_get_form('adestra_contact_administer_contact_form', $account);
  }
  return $render;
}

function adestra_contact_administer_contact_form($form, &$form_state, $account) {

  $form['#account'] = $account;

  $form['operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
  );
  $form['operations']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  if (adestra_contact_get_adestra_id($account, TRUE)) {
    $form['operations']['new_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Change Adestra ID to:'),
    );
    $form['operations']['actions']['change'] = array(
      '#type' => 'submit',
      '#value' => t('Change ID'),
    );
  }
  else{
    $form['operations']['actions']['create'] = array(
      '#type' => 'submit',
      '#value' => t('Create a new account'),
    );
  }

  return $form;
}

function adestra_contact_administer_contact_form_validate($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Change ID'):
      /*
       * TODO ADD Check to see if ID exists
       * $contacts = adestra_search_contact(NULL, array('id' => $form_state['values']['new_id']));
       */
    break;
  }
}

function adestra_contact_administer_contact_form_submit($form, &$form_state) {
  $account = $form['#account'];
  switch ($form_state['values']['op']) {
    case t('Change ID'):
      $id = $form_state['values']['new_id'];
    break;
    case t('Create a new account'):
      $id = adestra_create_contact(NULL, array('email' => $account->mail), 'email');
    break;
  }
  adestra_contact_set_adestra_id($form['#account'], $id);
}

function adestra_contact_user_field_summary() {
  $instances = field_info_instances('user', 'user');

  $render['attribute_table'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Label'),
      t('Name'),
      t('Attribute'),
      t('Operations'),
    )
  );
  $render['attribute_table']['#rows'] = array(
    'uid' => array(
      t('User Id'),
      'uid',
      variable_get_value('adestra_contact_attribute_uid'),
      l(t('Edit'), 'admin/config/services/adestra/contact/settings', array('query' => drupal_get_destination()))
    ),
    'email' => array(
      t('Email'),
      'mail',
      variable_get_value('adestra_contact_attribute_uid'),
      ''
    ),
    'username' => array(
      t('Username'),
      'username',
      variable_get_value('adestra_contact_attribute_username'),
      l(t('Edit'), 'admin/config/services/adestra/contact/settings', array('query' => drupal_get_destination()))
    ),
  );

  foreach ($instances as $key => $instance) {
    $render['attribute_table']['#rows'][$key] = array(
      'label' => $instance['label'],
      'key' => $key,
      $instance['settings']['adestra']['attribute_name'],
      (module_exists('field_ui') ? l(t('Edit'), 'admin/config/people/accounts/fields/' . $key, array('fragment' => 'edit-instance-settings-adestra', 'query' => drupal_get_destination())) : ''),
    );
  }
  if (module_exists('field_ui')) {
    $render['markup'] = array(
      '#markup' => t('To add a new field, please use the !manage-fields tab.', array('!manage-fields' => l(t('Manage Fields'), 'admin/config/people/accounts/fields'))),
    );
  }
  return $render;
}