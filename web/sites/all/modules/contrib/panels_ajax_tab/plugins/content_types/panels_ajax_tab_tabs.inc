<?php

/**
 * @file
 * Contains the content type plugin for a Ajax tabs.
 */

$plugin = array(
  'title'            => t('AJAX Tab - Tabs'),
  'admin title'      => 'panels_ajax_tab_tabs_content_type_admin_title',
  'admin info'       => 'panels_ajax_tab_tabs_content_type_admin_info',
  'description'      => t('List of Tabs to control a container'),
  'single'           => TRUE,
  'category'         => t('AJAX Tabs'),
  'required context' => array(
    new ctools_context_optional(t('Entity Context'), 'entity'),
  ),
  'render callback'  => 'panels_ajax_tab_tabs_render',
  'edit form'        => 'panels_ajax_tab_tabs_edit_form',
);

/**
 * Rendering function for the 'panels_ajax_tab_tabs' content type.
 */
function panels_ajax_tab_tabs_render($subtype, $conf, $panel_args, &$contexts) {
  ctools_include('context');
  ctools_include('plugins', 'panels');
  $block = new stdClass();
  $block->title = '';
  $block->content = '';

  // Add required JavaScript and CSS.
  drupal_add_js(drupal_get_path('module', 'panels_ajax_tab') . '/js/panels_ajax_tab.js');
  drupal_add_css(drupal_get_path('module', 'panels_ajax_tab') . '/css/panels_ajax_tab.css');

  // Add in default url_enabled.
  if (!isset($conf['url_enabled'])) {
    $conf['url_enabled'] = 1;
  }

  // Generate the context string.
  if (!$contexts[0]->data) {
    $context_string = 'none';
  }
  else {
    $entity_type = $contexts[0]->type[2];
    $entity_ids = entity_extract_ids($entity_type, $contexts[0]->data);
    $entity_id = array_shift($entity_ids);
    $context_string = $entity_type . ':' . $entity_id;
  }

  // Mix in any missing settings. Some settings could be missing due to upgrade
  // path.
  $old_settings = variable_get('panels_ajax_tab', array(
    'clean_url' => FALSE,
    'clean_url_delim' => '/',
    'panes' => array(),
  ));
  if (!isset($conf['clean_url'])) {
    $conf['clean_url'] = $old_settings['clean_url'];
  }
  if (!isset($conf['clean_url_delim'])) {
    $conf['clean_url_delim'] = $old_settings['clean_url_delim'];
  }

  // Sort and load the mini-panels.
  $tabs = array();
  uasort($conf['mini_panels'], 'drupal_sort_weight');

  // Get the visibility rule, if avaiable.
  $visibility_settings = _panels_ajax_tab_get_container_visibility_settings($conf['container_id']);

  $i = 0;
  foreach ($conf['mini_panels'] as $panel_name => $panel_conf) {
    if ($panel_conf['selected']) {

      if (!panels_ajax_tab_ctools_access($panel_name, $visibility_settings, $contexts)) {
        continue;
      }

      // Set the URL ID - note the first one never has one regardless of what
      // has been set.
      if ($i) {
        $url_id = $panel_conf['url_id'] ? $panel_conf['url_id'] : $panel_name;
        $url_base = drupal_is_front_page() ? base_path() . variable_get('site_frontpage') : url($_GET['q']);
      }
      else {
        $url_id = '';
        $url_base = drupal_is_front_page() ? base_path() : url($_GET['q']);
      }

      if (!$url_id || !$conf['url_enabled']) {
        $href = $conf['clean_url'] ? $url_base : '';
      }
      elseif ($conf['clean_url']) {
        $href = $url_base . $conf['clean_url_delim'] . $url_id;
      }
      else {
        $href = '#' . $url_id;
      }

      $mini = panels_mini_load($panel_name);
      $tabs[] = array(
        'mini_panel' => $mini,
        'title' => $panel_conf['tab_title'] ? $panel_conf['tab_title'] : $mini->admin_title,
        'url_id' => $url_id,
        'url_enabled' => $conf['url_enabled'],
        'href' => $href,
      );

      $i++;
    }
  }

  // Render the tabs.
  $block->content .= theme('panels_ajax_tab_tabs', array(
    'tabs' => $tabs,
    'tab_container_id' => $conf['container_id'],
    'context_string' => $context_string,
    'clean_url' => $conf['clean_url'],
  ));

  return $block;
}

/**
 * Provide the administrative title of a Ajax tab.
 */
function panels_ajax_tab_tabs_content_type_admin_title($subtype, $conf) {
  return "Ajax Tab - " . $conf['container_id'];
}

/**
 * Display the administrative information for a Ajax tab .
 */
function panels_ajax_tab_tabs_content_type_admin_info($subtype, $conf, $contexts) {

  $data = new stdClass();
  $data->content = '';

  // Sort and load the mini-panels.
  $minipanels = array();
  uasort($conf['mini_panels'], 'drupal_sort_weight');
  foreach ($conf['mini_panels'] as $panel_name => $panel_conf) {
    if ($panel_conf['selected']) {
      $minipanels[] = panels_mini_load($panel_name)->admin_title;
    }
  }

  $content = '';
  $data->title = 'Panels Ajax Tabs - ' . implode(', ', $minipanels);

  return $data;
}

/**
 * Plugin configuration form.
 */
function panels_ajax_tab_tabs_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // These settings are deprecated.
  $old_settings = variable_get('panels_ajax_tab', array(
    'clean_url' => FALSE,
    'clean_url_delim' => '/',
    'panes' => array(),
  ));

  $panels = panels_mini_load_all();
  foreach ($panels as $panel_name => $panel) {
    $panels[$panel_name] = (array) $panel;
    $panels[$panel_name] = empty($conf['mini_panels'][$panel_name]) ? $panels[$panel_name] : array_merge($panels[$panel_name], $conf['mini_panels'][$panel_name]);
  }
  uasort($panels, 'panels_ajax_tab_compare_weight');

  $form['mini_panels']['#tree'] = TRUE;
  foreach ($panels as $mini_panel) {
    $form['mini_panels'][$mini_panel['name']] = array(

      'selected' => array(
        '#type' => 'checkbox',
        '#default_value' => $mini_panel['selected'],
      ),

      'name' => array(
        '#markup' => $mini_panel['admin_title'],
      ),

      'tab_title' => array(
        '#type' => 'textfield',
        '#default_value' => $mini_panel['tab_title'],
      ),

      'url_id' => array(
        '#type' => 'textfield',
        '#default_value' => $mini_panel['url_id'],
      ),

      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $mini_panel['weight'],
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
    );
  }

  $form['prev_container_id'] = array(
    '#type' => 'hidden',
    '#value' => $conf['container_id'],
    '#required' => TRUE,
  );

  $form['container_id'] = array(
    '#title' => t('Container ID'),
    '#type' => 'textfield',
    '#description' => 'If you are using tabs in separate pane, you must supply a unique container id string.',
    '#default_value' => $conf['container_id'],
    '#required' => TRUE,
  );

  $visibility_settings = array();
  $is_old = 0;

  // Get the pid of this pane and set the flag, $is_old.
  // If the pane is in temporary cache then set that means pane is new and set
  // the $is_old to 0, and if pane is in permanent cache set $is_old to 1.
  if ($form_state['op'] == 'edit' && !empty($form_state['form_info']['path'])) {
    $current_path_args = arg();
    $pid = array_pop($current_path_args);

    $display_content = $form_state['display']->content;
    if (isset($display_content[$pid])) {
      $is_old = property_exists($display_content[$pid], 'did') && $display_content[$pid]->did == 0 ? 0 : 1;
    }
  }

  // Here we are showing the visibility settings navigation link, if the pane
  // is in permanent cache and non empty container_id.
  if (!empty($conf['container_id']) && $is_old) {
    $container_id = $conf['container_id'];
    $form['visibility_settings_link'] = array(
      '#prefix' => '<div class="form-item">',
      '#suffix' => '</div>',
      '#type' => 'markup',
      '#markup' => t('Please <a href="@link" target="_blank">click here</a> to change/update the visibility settings for this container.', array('@link' => '/admin/config/user-interface/panels-ajax-tab/' . $conf['container_id'] . '/edit')),
    );
    $visibility_settings = _panels_ajax_tab_get_container_visibility_settings($container_id);
  }

  $form['visibility_settings'] = array(
    '#type' => 'hidden',
    '#value' => $visibility_settings,
  );

  $form['url_enabled'] = array(
    '#title' => t('URL integration'),
    '#type' => 'checkbox',
    '#default_value' => isset($conf['url_enabled']) ? $conf['url_enabled'] : 1,
    '#description' => 'Enabling URL integration will allow users to link to a specific ajax-tab opening first.',
  );

  $form['clean_url'] = array(
    '#title' => t('Clean URLs'),
    '#type' => 'checkbox',
    '#default_value' => isset($conf['clean_url']) ? $conf['clean_url'] : $old_settings['clean_url'],
    '#description' => 'Checking "Clean URLs" will cause the URL identifiers to be used to give the tabs a clean URL, and also allows the user to link to a specific tab without using an anchor link. It also makes it possible for web crawlers to crawl your tabs. Be careful to use a unique-looking URL identifier, as panels-ajax-tabs is greedy about claiming ownership over URL patterns. For example don\'t use "contact" if you also have a page somewhere that is at "/about/contact", instead use something like "tab-contact". When using clean URLs and features, you will additionally need to export the variable <tt>panels_ajax_tab</tt>',
    '#states' => array(
      'invisible' => array(
        ':input[name="url_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['clean_url_delim'] = array(
    '#title' => t('Clean URL delimiter'),
    '#type' => 'textfield',
    '#size' => 1,
    '#default_value' => isset($conf['clean_url_delim']) ? $conf['clean_url_delim'] : $old_settings['clean_url_delim'],
    '#description' => 'Delimiter to be used for clean URLs',
    '#states' => array(
      'invisible' => array(
        ':input[name="url_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Set the theme.
  $form['#theme'] = 'panels_ajax_tab_tabs_edit_form';

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function panels_ajax_tab_tabs_edit_form_submit(&$form, &$form_state) {
  // Remove unnecessary values.
  form_state_values_clean($form_state);
  $form_state['conf']['mini_panels'] = $form_state['values']['mini_panels'];
  $form_state['conf']['container_id'] = $form_state['values']['container_id'];
  $form_state['conf']['url_enabled'] = $form_state['values']['url_enabled'];
  $form_state['conf']['clean_url'] = $form_state['values']['clean_url'];
  $form_state['conf']['clean_url_delim'] = $form_state['values']['clean_url_delim'];
  $form_state['conf']['visibility_settings'] = $form_state['values']['visibility_settings'];

  // Rebuild the cache.
  register_shutdown_function('panels_ajax_tab_config_cache');

  // Save the global settings into a variable.
  // This variable is no longer used, but we keep it up-to-date for backwards
  // compatibility reasons.
  $old_settings = variable_get('panels_ajax_tab', array(
    'clean_url' => FALSE,
    'clean_url_delim' => '/',
    'panes' => array(),
  ));
  $old_settings['clean_url'] = $form_state['values']['clean_url'];
  $old_settings['clean_url_delim'] = $form_state['values']['clean_url_delim'];

  if (isset($old_settings['panes'][$form_state['values']['prev_container_id']])) {
    unset($old_settings['panes'][$form_state['values']['prev_container_id']]);
  }
  // Sort and save the tabs into a variable.
  uasort($form_state['conf']['mini_panels'], 'drupal_sort_weight');
  foreach ($form_state['conf']['mini_panels'] as $name => $mini) {
    if ($mini['selected']) {
      $old_settings['panes'][$form_state['values']['container_id']][$name] = $mini['url_id'];
    }
  }
  variable_set('panels_ajax_tab', $old_settings);
}

/**
 * Callback function to sort mini-panels by both weight & if they are selected.
 */
function panels_ajax_tab_compare_weight($a, $b) {
  $a_selected = (is_array($a) && isset($a['selected'])) ? $a['selected'] : 0;
  $b_selected = (is_array($b) && isset($b['selected'])) ? $b['selected'] : 0;

  // If they are both selected, or both not-selected, sort by weight.
  if ($a_selected == $b_selected) {
    $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
    $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
    if ($a_weight == $b_weight) {
      return 0;
    }
    return ($a_weight < $b_weight) ? -1 : 1;
  }
  else {
    return ($a_selected > $b_selected) ? -1 : 1;
  }
}