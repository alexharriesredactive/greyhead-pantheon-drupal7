<?php
/**
 * @file: greyhead_customisations.inc
 *
 *      Utility functions which aren't Drupal hooks live here.
 */

/**
 * Gets one or more menu link IDs (mlid) of a particular Drupal path or alias.
 *
 * For paths which resolve to multiple links in the menu, the first will be
 * returned if $limit is 1 or not set; otherwise, all results will be returned
 * as an array with menu name and mlids, ordered by menu_name, plid and then
 * weight.
 *
 * @param        $path
 *
 * @param string $menu_name A menu name to filter results by. Optional.
 *
 * @param int $limit Maximum number of mlids to return. Defaults to 1.
 *
 * @return \the menu link ID if found, or FALSE if not.
 */
function greyhead_customisations_get_mlid_from_path($path = NULL, $menu_name = NULL, $limit = 1) {
  // If no path has been provided, use the current one.
  if (is_null($path)) {
    $path = $_GET['q'];
  }

  $normal_path = drupal_get_normal_path($path);

  $query = db_select('menu_links', 'm')
    ->fields('m', ['mlid', 'menu_name']);

  if (!is_null($menu_name)) {
    $query->condition('menu_name', $menu_name, '=');
  }

  $query->condition('link_path', $normal_path, '=')
    ->condition('hidden', 0, '=')
    ->orderBy('menu_name', 'ASC')
    ->orderBy('plid', 'ASC')
    ->orderBy('weight', 'ASC');

  if (is_numeric($limit) && ($limit > 0)) {
    $query->range(0, $limit);
  }

  $mlids = $query->execute()
    ->fetchAll();

  if (!empty($mlids)) {
    if ($limit == 1) {
      return $mlids[0]->mlid;
    }
    else {
      return $mlids;
    }
  }

  return FALSE;
}

/**
 * Gets a list of node IDs of pages which live below the current page in the
 * menu structure.
 *
 * @param null $mlid
 * @param string $menu_name
 * @param null $node_type
 *
 * @return string
 */
function greyhead_customisations_get_child_pages($mlid = NULL, $menu_name = 'main-menu', $node_type = NULL) {
  // If we don't have an mlid, get the current mlid by looking at
  // menu_get_active_trail. We take the last item in that list and only use it
  // if it's in main-menu.
  if (is_null($mlid)) {
    $active_trail = menu_get_active_trail();
    $active_trail = array_reverse($active_trail);

    if (count($active_trail) > 0) {
      $current_menu_item = $active_trail[0];

      if (isset($current_menu_item['menu_name']) && ($current_menu_item['menu_name'] == $menu_name)) {
        // It's in the specified menu; okay to continue.
        $mlid = $current_menu_item['mlid'];
        $menu_name = $current_menu_item['menu_name'];
      }
    }
  }

  // Set up some variaboos.
  $results = [];

  if (!is_null($mlid)) {
    // This code adapted from http://stackoverflow.com/a/30730118 with thanks :)
    $children = db_select('menu_links', 'm')
      ->fields('m', ['mlid', 'plid', 'link_path', 'link_title'])
      ->condition('menu_name', $menu_name, '=')
      ->condition('plid', $mlid, '=')
      ->condition('hidden', 0, '=')
      ->orderBy('weight', 'ASC')
      ->orderBy('link_title')
      ->execute()
      ->fetchAll();

    $counter = 0;

    foreach ($children as $child_link) {
      /**
       * Now that we have a list of links, we need to go through them and
       * get the following:
       *
       * Title - which may come from a custom field,
       *
       * Summary - which may be summary or trimmed body content, a separate
       *   field, or may be empty or non-existent,
       *
       * Image - which also may or may not exist.
       *
       * For nodes, we need to do a node load which may be quite expensive
       * for large menu structures, so a good database caching layer needs to
       * be implemented.
       */
      $counter++;

      // We will attempt to use the cache if block or page caching is enabled.
      $cacheable = (variable_get('block_cache') || variable_get('page_cache'));

      // Generate the cache ID for this data.
      $cid = __FUNCTION__ . ':menu-' . $menu_name . ':mlid-' . $child_link->mlid;

      // If caching of blocks or pages is enabled, check the cache for this
      // data. If not §available, generate it now.
      if ($cacheable && $result = cache_get($cid)) {
        $result = $result->data;
      }
      else {
        $result = _greyhead_customisations_child_page_template($counter);
        $node = NULL;

        $menu_link = menu_link_load($child_link->mlid);

        // If we have no access to the link, continue - skip over it.
        if (!$menu_link['access']) {
          continue;
        }

        // Get the path to the menu link. This will need to be run through
        // url().
        $result['path'] = $child_link->link_path;

        if (greyhead_customisations_get_nid_from_path($child_link->link_path)) {
          // Get the node ID.
          $nid = substr($child_link->link_path, 5);

          // Load the node.
          $node = node_load($nid);

          // Add the node in to the result.
          $result['node'] = $node;

          // Has the node been marked to be hidden from landing pages?
          if (greyhead_customisations_node_hidden_from_landing_page($nid)) {
            continue;
          }

          // If a node type has been specified, is this node of the correct type?
          if (!is_null($node_type) && ($node->type !== $node_type)) {
            continue;
          }

          // Get the title.
          $possible_title_fields = [
            'field_short_page_title',
          ];

          // Expose a hook:
          // hook_greyhead_customisations_child_pages_title_fields_alter
          drupal_alter('greyhead_customisations_child_pages_title_fields', $possible_title_fields);

          foreach ($possible_title_fields as $possible_title_field) {
            // Special case if the field's name is "title":
            if ($possible_title_field == 'title') {
              $result['title'] = $node->title;
              break;
            }
            elseif ($title_field_values = field_get_items('node', $node, $possible_title_field)) {
              $result['title'] = field_view_value('node', $node, $possible_title_field, $title_field_values[0]);
              $result['title'] = $result['title']['#markup'];
              break;
            }
          }

          if (is_null($result['title'])) {
            $result['title'] = $node->title;
          }

          // Get the summary, if any.
          $possible_summary_fields = [
            'field_content_page_summary',
          ];

          // Expose a hook:
          // hook_greyhead_customisations_child_pages_summary_fields_alter
          drupal_alter('greyhead_customisations_child_pages_summary_fields', $possible_summary_fields);

          foreach ($possible_summary_fields as $possible_summary_field) {
            if ($summary_field_values = field_get_items('node', $node, $possible_summary_field)) {
              $result['summary'] = field_view_value('node', $node, $possible_summary_field, $summary_field_values[0]);
              $result['summary'] = $result['summary']['#markup'];
              break;
            }
          }

          if (is_null($result['summary']) && isset($node->body)) {
            // Get the body body as a summary or trimmed to 250 chars.
            // Code from https://www.drupal.org/node/1264420#comment-5703230
            $result['summary'] = field_view_field('node', $node, 'body', [
              'label' => 'hidden',
              'type' => 'text_summary_or_trimmed',
              'settings' => ['trim_length' => 250],
            ]);

            // This should give us a render array with an HTML-formatted string.
            // We want to get this string and strip its tags; it can be found in
            // ['#items'][0]['safe_value'].
            $result['summary'] = strip_tags($result['summary'][0]['#markup']);
          }

          // Get the image, if any.
          $possible_image_fields = [
            'field_content_page_image',
            'field_content_image',
          ];

          // Expose a hook:
          // hook_greyhead_customisations_child_pages_image_fields_alter
          drupal_alter('greyhead_customisations_child_pages_image_fields', $possible_image_fields);

          foreach ($possible_image_fields as $possible_image_field) {
            if ($image_field_values = field_get_items('node', $node, $possible_image_field)) {
              $result['image'] = field_view_value('node', $node, $possible_image_field, $image_field_values[0]);
              $result['image'] = $result['image']['#item']['uri'];
              break;
            }
          }

          // Get the brand colour, if any.
          $possible_colour_fields = [
            'field_brand_colour',
          ];

          foreach ($possible_colour_fields as $possible_colour_field) {
            if ($colour_field_values = field_get_items('node', $node, $possible_colour_field)) {
              // If the colour isn't the default colour, don't override it.
              if (!$result['colour_is_default']) {
                continue;
              }

              $result['colour'] = $colour_field_values[0]['value'];
              $result['colour_is_default'] = TRUE;

              break;
            }
          }
        }
        else {
          // It's not a node. If we've been asked to only return nodes of a
          // certain type, we can ignore this menu link and continue.
          if (!is_null($node_type)) {
            continue;
          }

          // As it's not a node, we just get title and optional description.
          $result['title'] = $menu_link['link_title'];
          $result['summary'] = '';
        }

        // If not, we can just apply any classes which have been set on the
        // link.
        if ($result['colour_is_default']) {
          // Check for the existence of ['options']['attributes']['class'] and,
          // if it exists, add it into the classes key.
          if (isset($menu_link['options'])
            && isset($menu_link['options']['attributes'])
            && isset($menu_link['options']['attributes']['class'])
          ) {
            $result['class'] = $menu_link['options']['attributes']['class'];

            // Implode the classes array, if it's an array.
            if (is_array($result['class'])) {
              $result['class'] = implode(' ', $result['class']);

              // Note that we don't set $result['colour_is_default'] to TRUE here,
              // but it's possible that we have found a class in this menu link
              // which will change the colour. This is a bit of a kludge. Hashtag
              // worried face.
            }
          }
        }

        // If we couldn't get the node summary, or it's not a node, we get it now.
        // If the menu link has a title attribute set, we will use that as the
        // summary text for the link - look in ['options']['attributes']['title'].
        // So much faff to check for an array key without throwing errors.
        if (empty($result['summary']) && isset($menu_link['options'], $menu_link['options']['attributes'], $menu_link['options']['attributes']['title'])) {
          $result['summary'] = $menu_link['options']['attributes']['title'];
        }

        // Alias the path.
        $result['path'] = url($result['path']);

        // Expose a hook: greyhead_customisations_child_page_alter.
        drupal_alter('greyhead_customisations_child_page', $result, $menu_link, $node);

        //Create an expiry timestamp one hour in the future.
        $expire_timestamp = time() + (60 * 60);

        if ($cacheable) {
          cache_set($cid, $result, 'cache', $expire_timestamp);
        }
      }

      // Append the result array to the results array.
      $results[] = $result;
    }
  }

  // Return the results which we will run through our theme function.
  return $results;
}

/**
 * Returns a template array for the child pages listings.
 *
 * @param null $counter
 *
 * @return array
 */
function _greyhead_customisations_child_page_template($counter = NULL) {
  return [
    'title' => NULL,
    'summary' => NULL,
    'image' => NULL,
    'path' => NULL,
    'colour' => 'blue',
    // This flag is used so we know if a default colour is being used; we set
    // this to FALSE when we have found a colour override and want to stop any
    // other colour overrides from taking precedent.
    'colour_is_default' => TRUE,
    'class' => NULL,
    'counter' => $counter,
    'node' => NULL,
  ];
}

/**
 * Calculates the total size of attachments in field $field_name in a node.
 *
 * @param      $nid
 * @param      $field_name
 *
 * @param bool $human_friendly
 *
 * @return mixed FALSE if not a valid attachment, or a file-size if one can be
 *               determined.
 */
function greyhead_customisations_get_attachments_size($nid, $field_name, $human_friendly = TRUE) {
  $attachments_size = FALSE;

  if ($node = node_load($nid)) {
    // Get any attachments.
    $attachments = field_get_items('node', $node, $field_name);

    // If we have attachments, loop through each and add the total size in bytes
    // to $attachments_size.
    if (is_array($attachments) && (count($attachments) > 0)) {
      foreach ($attachments as $attachment) {
        $attachments_size += $attachment['filesize'];
      }

      // If we've been asked for a human-friendly output, format as KB, MB, etc.
      if ($human_friendly) {
        $attachments_size = greyhead_customisations_format_size($attachments_size);
      }
    }
  }

  return $attachments_size;
}

/**
 * Customised version of format_size() with control over decimal places.
 *
 * Also, for values under 1KB in size, simply shows "<1 KB".
 *
 * @param $size
 *   A size in bytes.
 * @param $langcode
 *   Optional language code to translate to a language other than what is used
 *   to display the page.
 *
 * @return
 *   A translated string representation of the size.
 */
function greyhead_customisations_format_size($size, $decimal_places = 0, $langcode = NULL) {
  if ($size < DRUPAL_KILOBYTE) {
    return t('&lt;1 KB');
  }
  else {
    $size = $size / DRUPAL_KILOBYTE; // Convert bytes to kilobytes.
    $units = [
      t('@size KB', [], ['langcode' => $langcode]),
      t('@size MB', [], ['langcode' => $langcode]),
      t('@size GB', [], ['langcode' => $langcode]),
      t('@size TB', [], ['langcode' => $langcode]),
      t('@size PB', [], ['langcode' => $langcode]),
      t('@size EB', [], ['langcode' => $langcode]),
      t('@size ZB', [], ['langcode' => $langcode]),
      t('@size YB', [], ['langcode' => $langcode]),
    ];
    foreach ($units as $unit) {
      if (round($size, 2) >= DRUPAL_KILOBYTE) {
        $size = $size / DRUPAL_KILOBYTE;
      }
      else {
        break;
      }
    }
    return str_replace('@size', round($size, $decimal_places), $unit);
  }
}

/**
 * Helper function: converts a URL or path into a class-friendly name.
 *
 * Replaces slashes with --, and any other non-alphanumerics with a hyphen.
 *
 * @param $path
 *
 * @return mixed
 */
function greyhead_customisations_get_class_from_url($path) {
  return preg_replace('/[^\da-z]/i', '--', $path);
}

/**
 * Checks whether a node has been set to be hidden from child pages.
 *
 * This function checks for the existence of a boolean node field called
 * 'field_landing_page_hide' which is set to 1 when the node should NOT be
 * displayed on landing pages, and 0 when it should to be shown.
 *
 * @param $nid
 *
 * @return bool
 */
function greyhead_customisations_node_hidden_from_landing_page($nid) {
  $field_landing_page_hide = FALSE;

  // Check for a valid nid.
  if ($node = node_load($nid)) {
    $field_landing_page_hide = FALSE;
    if ($field_landing_page_hide_items = field_get_items('node', $node, 'field_landing_page_hide')) {
      // Only set to TRUE if the value is 1; otherwise, stick with the
      // default of FALSE. We know that this field only stores a 1 or a 0,
      // so we can cheat by directly retrieving the value.
      if ($field_landing_page_hide_items[0]['value'] == 1) {
        $field_landing_page_hide = TRUE;
      }
    }
  }

  return $field_landing_page_hide;
}

/**
 * Gets the node ID from a node path or path alias.
 *
 * @param $path
 *
 * @return bool|int
 */
function greyhead_customisations_get_nid_from_path($path = NULL) {
  // If no path has been given, use the current path.
  if (is_null($path)) {
    $path = $_GET['q'];
  }

  $path = drupal_get_normal_path($path);

  if (substr($path, 0, 5) == 'node/') {
    // Get the node ID.
    return intval(substr($path, 5));
  }

  return FALSE;
}

/**
 * Create a custom-formatted date field, with the dates in the format
 * "18th Dec to 12th Jan 2016". We compare the start and end dates and try
 * not to repeat ourselves. Here are some examples:
 *
 * Sun 12th Dec 2015 - Mon 19th Jan 2016
 * Sun 12th Nov - Mon 19th Dec 2015
 * Sun 12th - Mon 19th Dec 2015
 *
 * @param $start_timestamp int The timestamp for the start date.
 * @param $end_timestamp   int The ending timestamp.
 *
 * @return string A string containing the start and end dates.
 */
function greyhead_customisations_get_from_to_date_short($start_timestamp, $end_timestamp) {
  // Create an array to keep our start data in.
  $dates = [
    'start' => [
      'timestamp' => NULL,
      'day_name' => NULL,
      'day_number' => NULL,
      'month' => NULL,
      'year' => NULL,
    ],
  ];

  // Clone the start array for the end array.
  $dates['end'] = $dates['start'];

  // Get the timestamps.
  $dates['start']['timestamp'] = $start_timestamp;
  $dates['end']['timestamp'] = $end_timestamp;

  // Generate the days, months, etc. We always include the day name and
  // number for start and end dates.
  $dates['start']['day_name'] = date('D', $dates['start']['timestamp']);
  $dates['start']['day_number'] = date('jS', $dates['start']['timestamp']);
  $dates['start']['month'] = date('F', $dates['start']['timestamp']);
  $dates['start']['year'] = date('Y', $dates['start']['timestamp']);

  $dates['end']['day_name'] = date('D', $dates['end']['timestamp']);
  $dates['end']['day_number'] = date('jS', $dates['end']['timestamp']);
  $dates['end']['month'] = date('F', $dates['end']['timestamp']);
  $dates['end']['year'] = date('Y', $dates['end']['timestamp']);

  $festival_dates_custom_formatted = $dates['start']['day_name'] . ' '
    . $dates['start']['day_number'] . ' ';

  // If the month is different, or if the year is different, between start and
  // end dates, also include the month.
  if (!($dates['start']['month'] == $dates['end']['month'])
    || !($dates['start']['year'] == $dates['end']['year'])
  ) {
    $festival_dates_custom_formatted .= $dates['start']['month'] . ' ';
  }

  // If the year is different between start and end dates, include it now.
  if ($dates['start']['year'] != $dates['end']['year']) {
    $festival_dates_custom_formatted .= $dates['start']['year'] . ' ';
  }

  // Now add the ending date in full.
  $festival_dates_custom_formatted .= ' - ' . $dates['end']['day_name'] . ' '
    . $dates['end']['day_number'] . ' ' . $dates['end']['month'] . ' '
    . $dates['end']['year'];

  return $festival_dates_custom_formatted;
}

/**
 * Print debug information to the screen. Handy when dpm() isn't working, e.g.
 * due to some crufty contrib module resetting the messages variable due to
 * crappy code written by a drunken idiot. Or something :)
 *
 * @param null $variable
 */
function gdpm($variable = NULL, $label = NULL) {
  echo 'gdpm:';

  if (!is_null($label)) {
    echo ' [' . $label . ']';
  }

  echo '<pre>' . print_r($variable, TRUE) . '</pre>';
}

/**
 * Add iconifier.net icons to a page.
 *
 * @param $path_to_icons        string The path to the icons, relative to the
 *   Drupal root. This value will be run through url().
 *
 * @param $apple_icon_sizes     array An array of icon sizes, e.g. array(16,
 *   32, 64)
 */
function greyhead_customisations_add_favicons($path_to_icons, $apple_icon_sizes) {
  // Add favicon.
  $favicon = [
    '#tag' => 'link',
    '#attributes' => [
      'href' => file_create_url($path_to_icons . '/favicon.ico'),
      'rel' => 'shortcut icon',
    ],
  ];

  drupal_add_html_head($favicon, 'image/vnd.microsoft.icon');

  // Add touch icons. www.iconifier.net made the images :)
  $apple_icon = [
    '#tag' => 'link',
    '#attributes' => [
      'href' => file_create_url($path_to_icons . '/apple-touch-icon.png'),
      'rel' => 'shortcut icon',
    ],
  ];

  drupal_add_html_head($apple_icon, 'apple-touch-icon');

  foreach ($apple_icon_sizes as $size) {
    $apple = [
      '#tag' => 'link',
      '#attributes' => [
        'href' => file_create_url($path_to_icons . '/apple-touch-icon-' . $size . 'x' . $size . '.png'),
        'rel' => 'apple-touch-icon',
        'sizes' => $size . 'x' . $size,
      ],
    ];

    drupal_add_html_head($apple, 'apple-touch-icon-' . $size);
  }

  // Windoze 10 tile icon.
  $win10_tile_colour = [
    '#tag' => 'meta',
    '#attributes' => [
      'content' => '#FFFFFF',
      'name' => 'msapplication-TileColor',
    ],
  ];

  drupal_add_html_head($win10_tile_colour, 'msapplication-TileColor');

  $win10_tile_image = [
    '#tag' => 'meta',
    '#attributes' => [
      'content' => file_create_url($path_to_icons . '/apple-touch-icon-144x144.png'),
      'name' => 'msapplication-TileImage',
    ],
  ];

  drupal_add_html_head($win10_tile_image, 'msapplication-TileImage');
}

/**
 * Disable form fields which have been overridden in /configuration.
 *
 * @TODO: can we somehow do this for strongarm'd variables, too...? At least
 *   show a warning?
 *
 * @param array $form_array A Drupal FAPI form array. This may be
 *                                      the child of a parent element if this
 *                                      function is being called recursively.
 *
 * @param array $overridden_variables An array where they KEYS are the names
 *                                      of the variables which have been
 *                                      overridden.
 *
 * @param bool $disable Set to FALSE if you only want to mark
 *                                      the field(s) as overridden; this is a
 *                                      better way of highlighting Strongarmed
 *                                      variables.
 *
 * @param string $context The type of variable override;
 *                                      currently, we can handle those
 *                                      overridden by greyhead_configuration
 *   and
 *                                      strongarm.
 */
function greyhead_customisations_highlight_overridden_form_fields(&$form_array, array $overridden_variables, $disable = TRUE, $context = 'greyhead_configuration') {
  // Store the variables data in static. If not already loaded, get to it! :)
  $overridden_variables_data = &drupal_static(__FUNCTION__ . ':' . $context);

  if (is_null($overridden_variables_data)) {
    switch ($context) {
      default:
      case 'greyhead_configuration':
        // If context is greyhead config, get the config array.
        $greyhead_configuration = variable_get('greyhead_configuration', ['overridden_variables' => []]);
        if (!(array_key_exists('overridden_variables', $greyhead_configuration))) {
          $greyhead_configuration['overridden_variables'] = [];
        }

        $overridden_variables_data = $greyhead_configuration['overridden_variables'];
        break;

      case 'strongarm':
        // If context is strongarm, get the strongarm config.
        $overridden_variables_data = strongarm_vars_load();
        break;
    }
  }

  // Loop through all child form elephants.
  foreach (element_children($form_array) as $form_child_element) {
    if (array_key_exists($form_child_element, $overridden_variables)
      && isset($form_array[$form_child_element]['#default_value'])) {

      // For Strongarm, all variables are in the $overridden_variables array,
      // so we also need to check if the variable is in code.
      if ($context == 'strongarm') {
        if (!($overridden_variables[$form_child_element]->export_type & EXPORT_IN_CODE)) {
          // This item isn't in code.
          continue;
        }
      }

      // Don't change already-disabled fields.
      if (isset($form_array[$form_child_element]['#disabled']) &&
        $form_array[$form_child_element]['#disabled']) {
        continue;
      }

      // Initialise the description and attributes fields, if they haven't
      // already been.
      if (!isset($form_array[$form_child_element]['#description'])) {
        $form_array[$form_child_element]['#description'] = '';
      }

      if (!isset($form_array[$form_child_element]['#attributes'])) {
        $form_array[$form_child_element]['#attributes'] = [];
      }

      // Set a default $variables array up.
      $variables = [
        '%module_name' => t('unknown'),
        '%value' => t('unknown'),
        '%status' => t('unknown'),
      ];

      // Are we disabling, or just marking the field as overridden?
      if ($disable) {
        $form_array[$form_child_element]['#attributes']['class'][] = 'greyhead-customisations-field-disabled';
        $form_array[$form_child_element]['#value'] = $form_array[$form_child_element]['#default_value'];
        $form_array[$form_child_element]['#disabled'] = TRUE;

        // Build a message.
        switch ($context) {
          default:
          case 'greyhead_configuration':
            $message = 'This field is disabled because this setting is overridden by the site\'s configuration files, and can only be changed through a code update.';
            break;

          case 'strongarm':
            // For Strongarm, we can deduce the Feature which is doing the
            // overriding, and whether the value has been overridden already.
            //            $message = 'This field is disabled because this setting is overridden by the %module_name Feature. The value in code is %value.';
            $message = 'This field is disabled because this setting is overridden by the %module_name Feature.';
            if (isset($overridden_variables_data[$form_child_element])) {
              if (isset($overridden_variables_data[$form_child_element]->export_module)) {
                $variables['%module_name'] = $overridden_variables_data[$form_child_element]->export_module;
              }

              if (isset($overridden_variables_data[$form_child_element]->value)) {
                $variables['%value'] = $overridden_variables_data[$form_child_element]->value;
              }

              if (isset($overridden_variables_data[$form_child_element]->type)) {
                $variables['%status'] = $overridden_variables_data[$form_child_element]->type;
              }
            }

            break;
        }

        $form_array[$form_child_element]['#description'] .= ' <div class="messages error">' . t($message, $variables) . '</div>';
      }
      else {
        // Build a message.
        switch ($context) {
          default:
          case 'greyhead_configuration':
            $message = 'This field is overridden by the site\'s configuration files, and should only be changed through a code update.';
            break;

          case 'strongarm':
            // For Strongarm, we can deduce the Feature which is doing the
            // overriding, and whether the value has been overridden already.
            $message = 'Caution: this field\'s value is being provided by the %module_name Feature via the Strongarm module. If you change it, it may be reverted on the next code deployment.';
            if (isset($overridden_variables_data[$form_child_element])) {
              if (isset($overridden_variables_data[$form_child_element]->export_module)) {
                $variables['%module_name'] = $overridden_variables_data[$form_child_element]->export_module;
              }
              if (isset($overridden_variables_data[$form_child_element]->value)) {
                $value = $overridden_variables_data[$form_child_element]->value;

                // If value's an array or object, we don't show it.
                if (is_array($value) || is_object($value)) {
                  $variables['%value'] = t('an array/object');
                }
                else {
                  $variables['%value'] = $overridden_variables_data[$form_child_element]->value;
                }
              }
              if (isset($overridden_variables_data[$form_child_element]->type)) {
                $variables['%status'] = $overridden_variables_data[$form_child_element]->type;
              }
            }

            break;
        }

        // Add a CSS class and update the description.
        $form_array[$form_child_element]['#attributes']['class'][] = 'greyhead-customisations-field-overridden';
        $form_array[$form_child_element]['#description'] .= ' <div class="messages warning">' . t($message, $variables) . '</div>';
      }
    }

    // Recurse downwards.
    greyhead_customisations_highlight_overridden_form_fields($form_array[$form_child_element], $overridden_variables, $disable, $context);
  }
}

/**
 * Normalises the line endings in a block of text
 *
 * @param $text string A block of text, HTML, etc, whose line ending style is
 *                     unknown.
 *
 * @return string The text with all line endings set to "\r\n".
 */
function greyhead_customisations_normalise_line_endings($text) {
  return preg_replace('~(*BSR_ANYCRLF)\R~', "\r\n", $text);
}

/**
 * Gets the Do Not Track setting from a client web browser.
 *
 * Code from
 * https://developer.mozilla.org/en-US/docs/Web/Security/Do_not_track_field_guide/Tutorials/1_Reading_the_DNT_header
 * with love and hugs 'n stuff.
 *
 * @return bool TRUE if the client browser sent a DNT header.
 */
function greyhead_customisations_do_not_track_status() {
  // Handy to have as a function because you may use this a lot.
  // The important logic here is: Dnt can be on (1), off (0), or
  // unset. You want to make sure you account for unset so you do
  // not de-reference a null pointer somewhere in your code.
  // returns TRUE if Dnt is on and is equal to 1,
  // returns FALSE if Dnt is unset or not equal to 1.
  return (isset($_SERVER['HTTP_DNT']) && $_SERVER['HTTP_DNT'] == 1);
}

/**
 * Overrides the robots.txt menu callback to disallow entire site if not prod.
 */
function greyhead_customisations_robotstxt_page_callback() {
  if (greyhead_environment_warning_is_production()) {
    robotstxt_robots();
  }
  else {
    // Send a plain text header.
    drupal_add_http_header('Content-type', 'text/plain');

    echo '# ROBOTS.txt disallowed via Greyhead Customisations' . "\r\n";
    echo '# as this is not a production site.' . "\r\n";
    echo '#' . "\r\n";
    echo '# Detected site type: ' . CURRENT_ENVIRONMENT . "\r\n";
    echo '#' . "\r\n";
    echo 'User-agent: *' . "\r\n";
    echo 'Disallow: /' . "\r\n";

    // No drupal_exit() here because we don't want to allow xmlsitemap to
    // add a link to the sitemap to this page.
    exit;
  }
}

/**
 * Make all alt text on image fields mandatory.
 *
 * Loops through all form elements and, if they're image fields, make alt text
 * mandatory.
 *
 * @param $element
 *
 * @return array The element.
 */
function greyhead_customisations_image_field_alt_text_required_process($element, &$form_state, $form) {
  if (!empty($element['alt'])) {
    // The below doesn't work, because when you upload an image in an image
    // field which allows unlimited submissions, you get an "Alternate text
    // is required" error when the form is sent back to the web browser via
    // Ahah, because the fields are being validated on submission.
    $element['alt']['#required'] = TRUE;

    // Workaround: set the "alt" text to "No alternative text provided".
    if (empty($element['alt']['#default_value'])) {
      $element['alt']['#default_value'] = t('No alternative text provided');
    }

    // I did try putting #limit_validation_errors in
    // greyhead_customisations_field_widget_form_alter() with the line:
    // $element['#limit_validation_errors'] = TRUE;
    // ... but that didn't help. Pants :(
  }

  return $element;
}

/**
 * Redirect back to the user's profile page.
 */
function greyhead_customisations_user_edit_form_redirect_to_user_view() {
  global $user;

  if (!empty($user->uid)) {
    drupal_set_message(t("Thanks for resetting your password. Please remember to write it down somewhere safe so you don't forget it! :)"), 'status');
    drupal_goto('user/' . $user->uid);
  }
}

/**
 * Get the site machine name (formerly the sites directory name).
 *
 * This should be defined in the site's settings.php, e.g.:
 *
 * defined('SITE_MACHINE_NAME') || define('SITE_MACHINE_NAME', 'my-website-of-awfulness');
 */
function greyhead_customisations_site_directory() {
  return defined('SITE_MACHINE_NAME') ? SITE_MACHINE_NAME : NULL;
}

/**
 * If a form has been submitted on a page which has a "d" parameter, redirect.
 *
 * We use this function to allow $_GET['d'] to be an alias of
 * $_GET['destination'] (purely so we can be lazy and type /user?d=<path>
 * instead of having to type "destination", since "estination" is a pesky
 * combination of characters (and, more importantly, I'm lazy).
 *
 * This function is designed to be called during form processing before
 * drupal_redirect_form() is called; that function contains important logic
 * to determine when we should redirect which we don't want to skip or
 * replicate.
 *
 * @param null|string $d
 *   The path to redirect to.
 */
function greyhead_customisations_d_redirector($d = NULL) {
  // Typical usage for this function is to get the path to redirect to from
  // $_GET['d'], but you can also pass a string into this function. However,
  // if you don't pass a string in, we have to confirm that $_GET['d'] is set
  // and not empty.
  if (is_null($d)) {
    if (!empty($_GET['d'])) {
      $d = $_GET['d'];
    }
    else {
      return;
    }
  }

  // $_GET['d'] is set - override $_GET['destination'] and call
  // drupal_redirect_form().
  $_GET['destination'] = $_GET['d'];

  // This function doesn't need to do anything more as drupal_redirect_form()
  // will now issue a redirect if appropriate (e.g. if it's not a
  // programmatically-submitted form, etc).
}
