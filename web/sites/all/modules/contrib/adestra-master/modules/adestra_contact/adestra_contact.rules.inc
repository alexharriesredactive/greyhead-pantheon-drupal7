<?php

/**
 * @file
 * Adestra Contact Rules Implementations.
 */

/**
 * Implementats of hook_rules_action_info().
 */
function adestra_contact_rules_action_info() {
  $actions = array();

  $actions['adestra_contact_subscribe_user'] = array(
    'label' => t('Add contact to a list'),
    'group' => t('Adestra'),
    'parameter' => array(
      'user' => array(
        'label' => t('User'),
        'type' => 'user',
      ),
      'list_id' => array(
        'type' => 'integer',
        'label' => t('List id contact should be subscribed to')
      ),
    ),
  );

  $actions['adestra_contact_unsubscribe_user'] = array(
    'label' => t('Remove contact from a list'),
    'group' => t('Adestra'),
    'parameter' => array(
      'user' => array(
        'label' => t('User'),
        'type' => 'user',
      ),
      'list_id' => array('type' => 'integer',
        'label' => t('List id contact should be unsubscribed from')
      ),
    ),
  );

  $actions['adestra_contact_set_attribute'] = array(
    'label' => t('Set contact attribute'),
    'group' =>  t('Adestra'),
    'parameter' => array(
      'user' => array(
        'label' => t('User'),
        'type' => 'user',
      ),
      'attribute_name' => array(
        'label' => t('Attribute Name'),
        'type' => 'text',
      ),
      'value' => array(
        'label' => t('Value'),
        'type' => 'text',
      )
    ),
  );

  $actions['adestra_contact_get_attribute'] = array(
    'label' => t('Get contact attribute'),
    'group' => t('Adestra'),
    'parameter' => array(
      'user' => array(
        'label' => t('User'),
        'type' => 'user',
      ),
      'attribute_name' => array(
        'label' => t('Attribute Name'),
        'type' => 'text',
      ),
      'variable_type' => array(
        'type' => 'text',
        'label' => t('Variable Type'),
        'options list' => 'rules_data_action_variable_add_options',
        'description' => t('Specifies the type of the variable that should be added.'),
        'restriction' => 'input',
      )
    ),
    'provides' => array(
      'attribute_value' => array(
        'type' => 'unknown',
        'label' => t('Attribute value'),
      ),
    ),
  );
  return $actions;
}

function adestra_contact_get_attribute_info_alter(&$element_info, RulesAbstractPlugin $element) {
  if (isset($element->settings['variable_type']) && $type = $element->settings['variable_type']) {
    $cache = rules_get_cache();
    $type_info = $cache['data_info'][$type];
    $element_info['provides']['attribute_value']['type'] = $type;
  }
}

function adestra_contact_set_attribute($user, $attribute_name, $value) {
  $id = adestra_contact_get_adestra_id($user, TRUE);
  adestra_update_contact($id, array($attribute_name => $value));
}

function adestra_contact_get_attribute($user, $attribute_name, $variable_type) {
  $id = adestra_contact_get_adestra_id($user, TRUE);
  $contact = adestra_fetch_contact($id);
  $value = $contact[$attribute_name];
  switch ($variable_type) {
    case 'decimal': $value = (double) $value; break;
    case 'duration':
    case 'integer': $value = (int) $value; break;
    case 'date': $value = strtotime($value); break;
    case 'boolean':
      if (drupal_strtolower($value) == 'false' || drupal_strtolower($value) == 'no') {
        $value = FALSE;
      }
      else{
        $value = (bool) $value;
      }
      break;
    case 'list<text>':
    case 'list': $value = array($value); break;
    case 'list<integer>': $value = array((int) $value); break;
  }
  return array('attribute_value' => $value);
}

function adestra_contact_subscribe_user($user, $list_id) {
  $id = adestra_contact_get_adestra_id($user, TRUE);
  adestra_update_contact($id, array($attribute_name => $value));
  $result = adestra_call('contact.addList', array($id, $list_id));
}

function adestra_contact_unsubscribe_user($user, $list_id) {
  $id = adestra_contact_get_adestra_id($user, TRUE);
  adestra_update_contact($id, array($attribute_name => $value));
  $result = adestra_call('contact.removeList', array($id, $list_id));
}