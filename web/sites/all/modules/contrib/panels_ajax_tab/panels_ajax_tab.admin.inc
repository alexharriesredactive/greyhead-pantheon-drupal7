<?php

/**
 * @file
 * Administration page callbacks for the Panels AJAX tabs module.
 */

/**
 * List all the configured tabs.
 *
 * This listing will help site-builder to set the visibility access rule for
 * selected/configured tabs.
 */
function panels_ajax_tab_admin_overview() {

  $panels_ajax_tabs = panels_ajax_tab_config_cache();
  $rows = array();
  foreach ($panels_ajax_tabs as $panels_ajax_tab) {
    $container_id = $panels_ajax_tab['container_id'];
    $rows[] = array(
      $panels_ajax_tab['container_id'],
      l(t('Edit'), 'admin/config/user-interface/panels-ajax-tab/' . $container_id . '/edit'),
    );
  }

  $header = array(t('Configured tab'), t('Operations'));
  $output = '<h3>' . t('Panels AJAX tabs') . '</h3>';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('There are no Panels AJAX tabs'),
  ));
  return $output;
}

/**
 * Determine a unique context type from a context_string.
 */
function _panels_ajax_tab_get_context_type($context_string) {
  $context = array_slice(explode('_', $context_string), 1, -1);
  $context_string = implode('_', $context);
  $context_string = str_replace('entity_id', 'entity', $context_string);
  return $context_string;
}

/**
 * Get a contexts from configured context string(s).
 */
function _panels_ajax_tab_get_contexts($contexts) {
  $context_objects = array();
  $context_type = '';
  if (!empty($contexts)) {
    if (is_string($contexts) && $contexts != 'empty') {
      $context_type = _panels_ajax_tab_get_context_type($contexts);
      $context_objects[] = ctools_context_create_empty($context_type);
    }

    if (is_array($contexts)) {
      foreach ($contexts as $context) {
        if ($context != 'empty') {
          $context_type = _panels_ajax_tab_get_context_type($context);
          $context_objects[] = ctools_context_create_empty($context_type);
        }
      }
    }
  }
  return $context_objects;
}

/**
 * Panels AJAX tabs settings form.
 */
function panels_ajax_tab_settings($form, $form_state, $panels_ajax_tab) {

  // Get the access/visibility rules.
  ctools_include('plugins');
  ctools_include('context');

  $contexts = _panels_ajax_tab_get_contexts($panels_ajax_tab['context']);
  $access_plugins = ctools_get_relevant_access_plugins($contexts);

  $access_options = array('none' => 'Choose visibility rule');
  foreach ($access_plugins as $id => $access_plugin) {
    $access_options[$id] = $access_plugin['title'];
  }
  asort($access_options);

  $mini_panels = $panels_ajax_tab['mini_panels'];

  $container_id = $panels_ajax_tab['container_id'];

  // Get the visibility rule, if available.
  $default_visibility_rules = _panels_ajax_tab_get_container_visibility_settings($container_id);

  $form['container_id'] = array(
    '#type' => 'hidden',
    '#value' => $container_id,
  );

  foreach ($mini_panels as $name => $mini_panel) {
    $title = !empty($mini_panel['tab_title']) ? $mini_panel['tab_title'] : $name;
    $form[$name] = array(
      '#type' => 'fieldset',
      '#title' => t('Visibility settings for @title', array('@title' => $title)),
      '#tree' => TRUE,
    );

    $form[$name]['visibility_rule'] = array(
      '#type' => 'select',
      '#title' => t('Visibility/Access Rule'),
      '#options' => $access_options,
      '#ajax' => array(
        'wrapper' => 'visibility-settings-wrapper-' . $name,
        'callback' => 'ajax_example_dynamic_sections_select_callback_new',
        'callback' => 'panels_ajax_tab_visibility_select_callback',
        'mini-name' => $name,
      ),
      '#default_value' => !empty($default_visibility_rules) ? $default_visibility_rules[$name]['visibility_rule'] : 'none',
    );
    // This fieldset just serves as a container for the part of the form
    // that gets rebuilt.
    $form[$name]['visibility_settings'] = array(
      '#type' => 'fieldset',
      // These provide the wrapper referred to in #ajax['wrapper'] above.
      '#prefix' => '<div id="visibility-settings-wrapper-' . $name . '">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );

    $form_state_values = !empty($form_state['values']) ? $form_state['values'] : array();
    // Decide the visibility rule.
    $visibility_rule = '';
    $conf = array();
    if (!empty($form_state_values)) {
      $visibility_rule = $form_state_values[$name]['visibility_rule'];
      $conf = $form_state_values[$name]['visibility_settings'];
    }
    elseif (!empty($default_visibility_rules)) {
      $visibility_rule = $default_visibility_rules[$name]['visibility_rule'];
      $conf = !empty($default_visibility_rules[$name]['visibility_settings']) ? $default_visibility_rules[$name]['visibility_settings'] : array();
    }

    if (!empty($visibility_rule) && $visibility_rule != 'none') {
      $form[$name]['visibility_settings']['#title'] = t('Settings');
      $plugin = ctools_get_access_plugin($visibility_rule);

      if ($function = ctools_plugin_get_function($plugin, 'settings form')) {
        $test_form = array();
        $test_form['settings'] = array('#tree' => TRUE);
        $form_state['plugin'] = $plugin;
        $test_form = $function($test_form, $form_state, $conf);
        if (!empty($test_form['settings'])) {
          $form[$name]['visibility_settings'] += $test_form['settings'];
        }
      }

      $form[$name]['visibility_settings']['not'] = array(
        '#type' => 'checkbox',
        '#title' => t('Reverse (NOT)'),
        '#default_value' => !empty($conf['not']),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

/**
 * Callback for the select element.
 */
function panels_ajax_tab_visibility_select_callback($form, $form_state) {
  $fieldset_name = $form_state['triggering_element']['#ajax']['mini-name'];
  $form_state['rebuild'] = TRUE;
  return $form[$fieldset_name]['visibility_settings'];
}

/**
 * Form submission handler for panels_ajax_tab_settings().
 */
function panels_ajax_tab_settings_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  $values = $form_state['values'];
  $container_id = $values['container_id'];
  unset($values['container_id']);
  $tabs_visibility[$container_id] = $values;

  $tabs_visibility = drupal_array_merge_deep(variable_get('panels_ajax_tab_visibility_rules', array()), $tabs_visibility);

  // Updated the mini-panel and/or page having panels_ajax_tab for the
  // visibility rule setting.
  _panels_ajax_tab_export_visibility_settings($container_id, $values);

  variable_set('panels_ajax_tab_visibility_rules', $tabs_visibility);

}

/**
 * Helper function to add visibility settings to panels.
 *
 * This function will update the mini-panels/pages having panel_ajax_tabs having
 * container id. This will enable one to export changes with less efforts. One
 * have to update features only. :)
 */
function _panels_ajax_tab_export_visibility_settings($container_id, $tabs_visibility) {
  // Look for panels_ajax_tab in mini panels, if found update this.
  if (module_exists('panels_mini')) {
    $panels = panels_mini_load_all();
    foreach ($panels as $panel) {
      if (!empty($panel->display->content)) {
        foreach ($panel->display->content as $pid => $pane) {
          if (in_array($pane->type, array('panels_ajax_tab_tabs', 'highwire_panel_tabs'))) {
            if (isset($pane->configuration['container_id'])
              && $pane->configuration['container_id'] == $container_id) {
              $configuration = &$pane->configuration;
              $configuration['visibility_settings'] = $tabs_visibility;
              panels_mini_save($panel);
              break 2;
            }
          }
        }
      }
    }
  }

  // Look for page-manager pages for panels_ajax_tab having $container_id, if
  // found update the task handler.
  if (module_exists('page_manager')) {
    $handlers = ctools_export_load_object('page_manager_handlers');

    foreach ($handlers as $key => &$handler) {
      $conf = $handler->conf;

      $display = !empty($conf['display']) ? $conf['display'] : NULL;
      $display = &$handler->conf['display'];

      // If this display is overridden then we got to get the overridden
      // display to alter.
      if (empty($display) && isset($conf['did'])) {
        $display = panels_load_display($conf['did']);
      }

      if (property_exists($display, 'content')) {
        foreach ($display->content as $content_pane) {
          if (in_array($content_pane->type, array('panels_ajax_tab_tabs', 'highwire_panel_tabs'))) {
            $pane_conf = &$content_pane->configuration;
            if (isset($pane_conf['container_id']) && $pane_conf['container_id'] == $container_id) {
              $pane_conf['visibility_settings'] = $tabs_visibility;
              page_manager_save_task_handler($handler);
              break 2;
            }
          }
        }
      }
    }
  }
}
