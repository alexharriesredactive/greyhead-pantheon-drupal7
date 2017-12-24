<?php
/**
 * @file
 * Enables modules and site configuration for a greyhead site installation.
 */

/**
 * Replace the database selection task with a custom form which has the details
 * pre-populated if available in $_SESSION['multisitemaker'].
 *
 * @param $tasks
 * @param $install_state
 */
function greyheadprofile_install_tasks_alter(&$tasks, $install_state) {
  $tasks['install_settings_form']['function'] = 'greyheadprofile_install_settings_form';
}

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function greyheadprofile_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}

/**
 * Replacement version of the install settings form which has the database
 * details pre-populated if available in $_SESSION['multisitemaker'].
 *
 * @param $form
 * @param $form_state
 * @param $install_state
 *
 * @return mixed
 */
function greyheadprofile_install_settings_form($form, &$form_state, &$install_state) {
  // Check for $_GET variables to pre-set the database connection details; if
  // set, copy them into $_SESSION['multisitemaker'] and redirect to the page
  // without these sensitive details in the URL.
  if (isset($_GET['new_database'], $_GET['new_username'], $_GET['new_password'],
    $_GET['hostname'], $_GET['port'], $_GET['driver'])) {

    $db_settings = array(
      'new_database' => $_GET['new_database'],
      'new_username' => $_GET['new_username'],
      'new_password' => $_GET['new_password'],
      'hostname' => $_GET['hostname'],
      'port' => $_GET['port'],
      'driver' => $_GET['driver'],
    );

//    // Get our protolol.
//    $protocol = 'http';
//    if (isset($_SERVER['HTTPS'])) {
//      if ($_SERVER['HTTPS'] == 'on') {
//        $protocol = 'https';
//      }
//    }
//
//    $redirect_options = array(
//      'profile=greyheadprofile',
//      'locale=en',
//    );
//
//    $redirect_url = $protocol . '://' . $_SERVER['HTTP_HOST'] . '/install.php?' . implode('&', $redirect_options);
//
//    echo '<pre>' . print_r($_SESSION, TRUE) . '</pre>';
//
//    header('Location: ' . $redirect_url, TRUE, 302);
//    exit();
  }

//  echo '<pre>' . print_r($_SESSION, TRUE) . '</pre>';
//  exit();

  global $databases;
  $profile = $install_state['parameters']['profile'];
  $install_locale = $install_state['parameters']['locale'];

  drupal_static_reset('conf_path');
  $conf_path = './' . conf_path(FALSE);
  $settings_file = $conf_path . '/settings.php';
  $database = isset($databases['default']['default']) ? $databases['default']['default'] : array();

  drupal_set_title(st('Database configuration'));

  $drivers = drupal_get_database_types();
  $drivers_keys = array_keys($drivers);

  $form['driver'] = array(
    '#type' => 'radios',
    '#title' => st('Database type'),
    '#required' => TRUE,
    '#default_value' => !empty($database['driver']) ? $database['driver'] : current($drivers_keys),
    '#description' => st('The type of database your @drupal data will be stored in.', array('@drupal' => drupal_install_profile_distribution_name())),
  );
  if (count($drivers) == 1) {
    $form['driver']['#disabled'] = TRUE;
    $form['driver']['#description'] .= ' ' . st('Your PHP configuration only supports a single database type, so it has been automatically selected.');
  }

  // Add driver specific configuration options.
  foreach ($drivers as $key => $driver) {
    $form['driver']['#options'][$key] = $driver->name();

    $form['settings'][$key] = $driver->getFormOptions($database);
    $form['settings'][$key]['#prefix'] = '<h2 class="js-hide">' . st('@driver_name settings', array('@driver_name' => $driver->name())) . '</h2>';
    $form['settings'][$key]['#type'] = 'container';
    $form['settings'][$key]['#tree'] = TRUE;
    $form['settings'][$key]['advanced_options']['#parents'] = array($key);
    $form['settings'][$key]['#states'] = array(
      'visible' => array(
        ':input[name=driver]' => array('value' => $key),
      ),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => st('Save and continue'),
    '#limit_validation_errors' => array(
      array('driver'),
      array(isset($form_state['input']['driver']) ? $form_state['input']['driver'] : current($drivers_keys)),
    ),
    '#validate' => array('install_settings_form_validate'),
    '#submit' => array('install_settings_form_submit'),
  );

  $form['errors'] = array();
  $form['settings_file'] = array(
    '#type' => 'value',
    '#value' => $settings_file,
  );

  // If we have database details in $multisitemaker, update the form
  // fields and disable them.
  if (isset($db_settings)) {
    $driver = $db_settings['driver'];

    $form['settings'][$driver]['database']['#type'] = 'value';
    $form['settings'][$driver]['database']['#value'] = $db_settings['new_database'];

    $form['settings'][$driver]['username']['#type'] = 'value';
    $form['settings'][$driver]['username']['#value'] = $db_settings['new_username'];

    $form['settings'][$driver]['password']['#type'] = 'value';
    $form['settings'][$driver]['password']['#value'] = $db_settings['new_password'];

    $form['settings'][$driver]['advanced_options']['host']['#type'] = 'value';
    $form['settings'][$driver]['advanced_options']['host']['#value'] = $db_settings['hostname'];

    $form['settings'][$driver]['advanced_options']['port']['#type'] = 'value';
    $form['settings'][$driver]['advanced_options']['port']['#value'] = $db_settings['port'];

    $form['driver']['#type'] = 'value';
    $form['driver']['#value'] = $driver;

    // Also hide the table prefix field and the advanced options field set.
    $form['settings'][$driver]['advanced_options']['db_prefix']['#type'] = 'value';
    $form['settings'][$driver]['advanced_options']['db_prefix']['#value'] = '';

    // Can we hide the advanced options fieldgroup?
    $can_hide = TRUE;
    foreach (element_children($form['settings'][$driver]['advanced_options']) as $field_name) {
      // Have we found an element which isn't hidden?
      if ($form['settings'][$driver]['advanced_options'][$field_name]['#type'] != 'value') {
        $can_hide = FALSE;
      }
    }

    if ($can_hide) {
      $form['settings'][$driver]['advanced_options']['#access'] = FALSE;
    }

    // If we have a driver, unset other database types.
    foreach ($form['settings'] as $driver_fapi_key => $settings) {
      if ($driver_fapi_key != $driver) {
        unset($form['settings'][$driver_fapi_key]);
      }
    }

    // Add some blurb to the top of the form.
    $form['please_click_save_and_continue'] = array(
      '#type' => 'markup',
      '#markup' => st('<h3>Please click "Save and continue"</h3> <p>There shouldn\'t be any options you need to set on this page, so please just click "Save and continue" here.</p>'),
      '#weight' => -10,
    );
  }

//  echo '<pre>' . print_r($form, TRUE) . '</pre>';

  return $form;
}
