<?php

/**
 * @file
 * Admin callbacks for the Quantcast module.
 */

/**
 * Implements hook_admin_settings().
 */
function quantcast_admin_settings_form() {
  // Tabs.
  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
    '#title'          => t('General settings'),
  );

  // General settings.
  $form['tabs']['account'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('General settings'),
  );
  $form['tabs']['account']['quantcast_account'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Quantcast P-Code'),
    '#default_value'  => variable_get('quantcast_account', ''),
    '#required'       => TRUE,
    '#description'    => t('If you do not already have a Quantcast P-Code, please acquire it by registering at <a href="@qc_url">Quantcast.com</a>.', array('@qc_url' => 'http://www.quantcast.com/user/signup')),
  );

  // Role settings.
  $form['tabs']['roles']['role_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );
  $form['tabs']['roles']['role_settings']['quantcast_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add Quantcast script for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('quantcast_visibility_roles', 0),
  );
  $form['tabs']['roles']['role_settings']['quantcast_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('quantcast_roles', array()),
    '#options' => array_map('check_plain', user_roles()),
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  // Advanced settings.
  $form['tabs']['advanced'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Advanced settings'),
    '#collapsible'    => TRUE,
    '#collapsed'      => TRUE,
  );
  $form['tabs']['advanced']['quantcast_js_scope'] = array(
    '#type'           => 'select',
    '#title'          => t('Javascript Scope'),
    '#description'    => t('Quantcast recommends adding its Javascript code to the footer, right before &lt;/body&gt; tag and any other measurement tags. Splitting the tag into two components should additionally optimize the performance of the Quantcast asynchronous tag. See <a href="@qc_implementation_guide_url">Web Measurement Implementation Guide</a> for more details.', array('@qc_implementation_guide_url' => 'http://www.quantcast.com/learning-center/guides/webmeasurement')),
    '#options'        => array(
      'page_top'        => t('Header'),
      'page_bottom'     => t('Footer'),
      'split'           => t('Split'),
    ),
    '#default_value'  => variable_get('quantcast_js_scope', 'page_bottom'),
  );

  return system_settings_form($form);
}
