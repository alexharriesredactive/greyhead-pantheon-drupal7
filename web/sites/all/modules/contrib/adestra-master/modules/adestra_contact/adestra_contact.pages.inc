<?php

/**
 * @file
 * Adestra Contact Pages.
 */

function adestra_contact_subscriptions_form($form, &$form_state, $account) {

  $id = adestra_contact_get_adestra_id($account, TRUE);
  $fetched_contact_lists = adestra_fetch_contact_lists($id);
  $adestra_contact_lists_editable = array_filter(explode("\n", trim(variable_get_value('adestra_contact_lists_editable'))));

  if (empty($adestra_contact_lists_editable)) {
    $form['no_preferences']['#markup'] = t('Currently there are no available preferences.');
    return $form;
  }

  $default_value = array();
  $options = array();

  foreach ($adestra_contact_lists_editable as $list) {
    $listinfo = explode("|", $list);
    $options[$listinfo[0]] = $listinfo[1];
    if (in_array($listinfo[0], $fetched_contact_lists)) {
      $default_value[] = $listinfo[0];
    }
  }

  $form = array();

  $form['#account'] = $account;

  $form['adestra_lists'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $default_value
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update preferences'),
  );
  return $form;
}

function adestra_contact_subscriptions_form_submit($form, &$form_state) {

  $account = $form['#account'];
  $id = adestra_contact_get_adestra_id($account, TRUE);

  $original_lists = $form['adestra_lists']['#default_value'];
  // Build an array of the groups a user has selected
  // FAPI returns all groups but sets the value to 0 for
  // those unselected. We want a simplier array only
  // including selected groups. This is so we can use
  // the simple array_diff method below.
  $new_lists = array();
  foreach ($form_state['values']['adestra_lists'] AS $value) {
    if ($value) {
      $new_lists[$value] = $value;
    }
  }

  $unsubscribe_lists = array_diff($original_lists, $new_lists);
  $subscribe_lists = array_diff($new_lists, $original_lists);

  adestra_subscribe_contact_multiple($id, $subscribe_lists);
  adestra_unsubscribe_contact_multiple($id, $unsubscribe_lists);


}