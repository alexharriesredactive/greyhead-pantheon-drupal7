<?php

/**
 * Implements template_preprocess_html().
 *
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 *
 * @internal param $hook The name of the template being rendered ("html" in this case.)
 */
function zurb_foundation_fordsport_preprocess_html(&$variables) {
  // Add conditional CSS for IE. To use uncomment below and add IE css file
  drupal_add_css(path_to_theme() . '/css/ie.css', array('weight' => CSS_THEME, 'browsers' => array('!IE' => FALSE), 'preprocess' => FALSE));

  // Need legacy support for IE downgrade to Foundation 2 or use JS file below
  // drupal_add_js('http://ie7-js.googlecode.com/svn/version/2.1(beta4)/IE7.js', 'external');

  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));

  // Add a class to the body tag to indicate if we have a header
  // image or not.
  if (!is_array($variables['classes_array'])) {
    $variables['classes_array'] = array();
  }

  // Add touch icons. www.iconifier.net made the images :)
  $apple_icon = array(
    '#tag' => 'link',
    '#attributes' => array(
      'href' => base_path() . path_to_theme() . '/images/favicons/apple-touch-icon.png',
      'rel' => 'apple-touch-icon-precomposed',
    ),
  );

  drupal_add_html_head($apple_icon, 'apple-touch-icon');

  $apple_icon_sizes = array(57, 72, 76, 114, 120, 144, 152);

  foreach ($apple_icon_sizes as $size) {
    $apple = array(
      '#tag' => 'link',
      '#attributes' => array(
        'href' => base_path() . path_to_theme() . '/images/favicons/apple-touch-icon-' . $size . 'x' . $size . '.png',
        'rel' => 'apple-touch-icon-precomposed',
        'sizes' => $size . 'x' . $size,
      ),
    );

    drupal_add_html_head($apple, 'apple-touch-icon-' . $size);
  }
}

/**
 * Implements template_preprocess_page
 *
 */
function zurb_foundation_fordsport_preprocess_page(&$variables) {
  $variables['logo_img'] = '';

  if (!empty($variables['logo'])) {
    $variables['logo_img'] = theme('image', array(
      'path' => $variables['logo'],
      'alt' => strip_tags($variables['site_name']) . ' ' . t('logo'),
      'title' => strip_tags($variables['site_name']) . ' ' . t('Home'),
      'attributes' => array(
        'class' => array('logo'),
      ),
    ));
  }

  $variables['linked_logo'] = '';
  if (!empty($variables['logo_img'])) {
    $variables['linked_logo'] = l($variables['logo_img'], '<front>', array(
      'attributes' => array(
        'rel' => 'home',
        'title' => strip_tags($variables['site_name']) . ' ' . t('Home'),
      ),
      'html' => TRUE,
    ));
  }

  $variables['linked_site_name'] = '';
  if (!empty($variables['site_name'])) {
    $variables['linked_site_name'] = l($variables['site_name'], '<front>', array(
      'attributes' => array(
        'rel' => 'home',
        'title' => strip_tags($variables['site_name']) . ' ' . t('Home'),
      ),
    ));
  }

  // Top bar.
  if ($variables['top_bar'] = theme_get_setting('zurb_foundation_top_bar_enable')) {
    $top_bar_classes = array();

    if (theme_get_setting('zurb_foundation_top_bar_grid')) {
      $top_bar_classes[] = 'contain-to-grid';
    }

    if (theme_get_setting('zurb_foundation_top_bar_sticky')) {
      $top_bar_classes[] = 'sticky';
    }

    if ($variables['top_bar'] == 2) {
      $top_bar_classes[] = 'show-for-small';
    }

    $variables['top_bar_classes'] = implode(' ', $top_bar_classes);
    $variables['top_bar_menu_text'] = check_plain(theme_get_setting('zurb_foundation_top_bar_menu_text'));

    $top_bar_options = array();

    if (!theme_get_setting('zurb_foundation_top_bar_custom_back_text')) {
      $top_bar_options[] = 'custom_back_text:false';
    }

    if ($back_text = check_plain(theme_get_setting('zurb_foundation_top_bar_back_text'))) {
      if ($back_text !== 'Back') {
        $top_bar_options[] = "back_text:'{$back_text}'";
      }
    }

    if (!theme_get_setting('zurb_foundation_top_bar_is_hover')) {
      $top_bar_options[] = 'is_hover:false';
    }

    if (!theme_get_setting('zurb_foundation_top_bar_scrolltop')) {
      $top_bar_options[] = 'scrolltop:false';
    }

    $variables['top_bar_options'] = ' data-options="' . implode('; ', $top_bar_options) . '"';
  }

  // Alternative header.
  // This is what will show up if the top bar is disabled or enabled only for
  // mobile.
  if ($variables['alt_header'] = ($variables['top_bar'] != 1)) {
    // Hide alt header on mobile if using top bar in mobile.
    $variables['alt_header_classes'] = $variables['top_bar'] == 2 ? ' hide-for-small' : '';
  }

  // Menus for alternative header.
  $variables['alt_main_menu'] = '';

  if (!empty($variables['main_menu'])) {
    $variables['alt_main_menu'] = theme('links__system_main_menu', array(
      'links' => $variables['main_menu'],
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('links', 'inline-list', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  $variables['alt_secondary_menu'] = '';

  if (!empty($variables['secondary_menu'])) {
    $variables['alt_secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $variables['secondary_menu'],
      'attributes' => array(
        'id' => 'secondary-menu-links',
        'class' => array('links', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  // Top bar menus.
  $variables['top_bar_main_menu'] = '';
  if (!empty($variables['main_menu'])) {
    $variables['top_bar_main_menu'] = theme('links__topbar_main_menu', array(
      'links' => $variables['main_menu'],
      'attributes' => array(
        'id' => 'main-menu',
        'class' => array('main-nav'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  $variables['top_bar_secondary_menu'] = '';
  if (!empty($variables['secondary_menu'])) {
    $variables['top_bar_secondary_menu'] = theme('links__topbar_secondary_menu', array(
      'links' => $variables['secondary_menu'],
      'attributes' => array(
        'id' => 'secondary-menu',
        'class' => array('secondary', 'link-list'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  // Messages in modal.
  $variables['zurb_foundation_messages_modal'] = theme_get_setting('zurb_foundation_messages_modal');

  // Convenience variables
  if (!empty($variables['page']['sidebar_first'])) {
    $left = $variables['page']['sidebar_first'];
  }

  if (!empty($variables['page']['sidebar_second'])) {
    $right = $variables['page']['sidebar_second'];
  }

  // Dynamic sidebars
  if (!empty($left) && !empty($right)) {
    $variables['main_grid'] = 'large-6 push-3';
    $variables['sidebar_first_grid'] = 'large-3 pull-6';
    $variables['sidebar_sec_grid'] = 'large-3';
  }
  elseif (empty($left) && !empty($right)) {
    $variables['main_grid'] = 'large-9';
    $variables['sidebar_first_grid'] = '';
    $variables['sidebar_sec_grid'] = 'large-3';
  }
  elseif (!empty($left) && empty($right)) {
    $variables['main_grid'] = 'large-9 push-3';
    $variables['sidebar_first_grid'] = 'large-3 pull-9';
    $variables['sidebar_sec_grid'] = '';
  }
  else {
    $variables['main_grid'] = 'large-12';
    $variables['sidebar_first_grid'] = '';
    $variables['sidebar_sec_grid'] = '';
  }

  // Ensure modal reveal behavior if modal messages are enabled.
  if (theme_get_setting('zurb_foundation_messages_modal')) {
    drupal_add_js(drupal_get_path('theme', 'zurb_foundation') . '/js/behavior/reveal.js');
  }

  // Template suggestions
  if (array_key_exists('node', $variables) && isset($variables['node']->type)) {
    $nodetype = $variables['node']->type;
    $variables['theme_hook_suggestions'][] = 'page__' . $nodetype;
  }
}

/**
 * Implements template_preprocess_maintenance_page.
 */
function zurb_foundation_fordsport_preprocess_maintenance_page(&$variables) {
  // Fix for undefined variable errors.
  if (!array_key_exists('body_classes', $variables)) {
    $variables['body_classes'] = '';
  }

  if (!array_key_exists('footer_message', $variables)) {
    $variables['footer_message'] = '';
  }
}

/**
 * Get an array which can be used to track elements used.
 *
 * @param $elements_count
 *
 * @return array
 */
function _zurb_foundation_fordsport_get_elements_array($elements_count) {
  $elements_list = array();

  for ($count = 0; $count < ($elements_count - 1); $count++) {
    $elements_list[] = $count;
  }

  // Shuffle the list.
  shuffle($elements_list);

  return $elements_list;
}

/**
 * Choose an element from the elements_list and return its array key number.
 *
 * @param $elements_list
 *
 * @return int
 */
function _zurb_foundation_fordsport_choose_element(&$elements_list) {
  if (count($elements_list) > 0) {
    $element = array_shift($elements_list);
    return $element;
  }

  return FALSE;
}

/**
 * Implements template_preprocess_node
 *
 */
function zurb_foundation_fordsport_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'teaser') {
    $variables['theme_hook_suggestions'][] = 'node__' . $variables['node']->type . '__teaser';
    $variables['theme_hook_suggestions'][] = 'node__' . $variables['node']->nid . '__teaser';
  }

  // In events, hide the google map link because it has been included in the
  // event location field:
  if (($variables['type'] == 'event') && ($variables['teaser'])) {
    // Help on hiding fields from http://stackoverflow.com/questions/12514424/how-to-hide-field-from-preprocess-node-in-drupal-7
    hide($variables['content']['field_event_google_map_link']);
  }

  // If the event organiser name and web link have been filled in, hide the
  // event organiser link:
  if (($variables['type'] == 'event') &&  ($variables['view_mode'] == 'full')) {
    if (array_key_exists('field_event_organiser_name', $variables['content'])
      && array_key_exists('field_event_organiser_website', $variables['content'])) {
      $variables['content']['field_event_organiser_website']['#access'] = FALSE;
    }
  }

  // On homepages, create $hp_welcome_site_name and $hp_welcome_slogan.
  if ($variables['type'] == 'homepage') {
    $variables['hp_welcome_site_name'] = t('Welcome to !site_name', array('!site_name' => variable_get('site_name')));
    $variables['hp_welcome_slogan'] = t('!slogan', array('!slogan' => variable_get('site_slogan')));
  }

  // On homepages, add a jQuery script for the homepage only:
  if ($variables['type'] == 'homepage') {
    global $theme_key;
    drupal_add_js(drupal_get_path('theme', $theme_key) . '/js/homepage.js');
  }

  // On homepages, choose one of the welcome images and set it as the background
  // image of the #homepage-[nid]-welcome element:
  if ($variables['type'] == 'homepage') {
    // We use the images from field_hp_welcome_images for welcome, register and
    // contact divs.
    $elements_list = _zurb_foundation_fordsport_get_elements_array(count($variables['field_hp_welcome_images']));

    // Images are in $variables['field_hp_welcome_images'][0...n]. Choose one:
    $selected_welcome_image = _zurb_foundation_fordsport_choose_element($elements_list);
    if ($selected_welcome_image !== FALSE) {
      // @Todo: write media queries and use image_style_url to set definitions for each breakpoint.
      // @Todo: can we tie these breakpoints into the theme or resp_img?

      // Set up the <style> tag:
      $element = array(
        '#type' => 'markup',
        '#markup' => '
        <style type="text/css">
          #homepage-' . $variables['node']->nid . '-welcome {
            background-image: url(\'' . image_style_url('homepage_bg_img_full', $variables['field_hp_welcome_images'][$selected_welcome_image]['uri']) . '\');
          }
        </style>',
      );

      drupal_add_html_head($element, 'fordsport-homepage-welcome-background');
    }

    // Images are in $variables['field_hp_welcome_images'][0...n]. Choose one:
    $selected_register_image = _zurb_foundation_fordsport_choose_element($elements_list);
    if ($selected_register_image !== FALSE) {
      // @Todo: write media queries and use image_style_url to set definitions for each breakpoint.
      // @Todo: can we tie these breakpoints into the theme or resp_img?

      // Set up the <style> tag:
      $element = array(
        '#type' => 'markup',
        '#markup' => '
        <style type="text/css">
          #homepage-' . $variables['node']->nid . '-register {
            background-image: url(\'' . image_style_url('homepage_bg_img_full', $variables['field_hp_welcome_images'][$selected_register_image]['uri']) . '\');
          }
        </style>',
      );

      drupal_add_html_head($element, 'fordsport-homepage-register-background');
    }

    // Images are in $variables['field_hp_welcome_images'][0...n]. Choose one:
    $selected_contact_image = _zurb_foundation_fordsport_choose_element($elements_list);
    if ($selected_contact_image !== FALSE) {
      // @Todo: write media queries and use image_style_url to set definitions for each breakpoint.
      // @Todo: can we tie these breakpoints into the theme or resp_img?

      // Set up the <style> tag:
      $element = array(
        '#type' => 'markup',
        '#markup' => '
        <style type="text/css">
          #homepage-' . $variables['node']->nid . '-contact {
            background-image: url(\'' . image_style_url('homepage_bg_img_full', $variables['field_hp_welcome_images'][$selected_contact_image]['uri']) . '\');
          }
        </style>',
      );

      drupal_add_html_head($element, 'fordsport-homepage-contact-background');
    }
  }

  // On homepages, choose one of the events images and set it as the background
  // image of the #homepage-[nid]-events element:
  if ($variables['type'] == 'homepage') {
    // Images are in $variables['field_hp_welcome_images'][0...n]. Choose one:
    $selected_welcome_image = (int)rand(0, count($variables['field_hp_events_images']) - 1);

    // @Todo: write media queries and use image_style_url to set definitions for each breakpoint.
    // @Todo: can we tie these breakpoints into the theme or resp_img?

    // Set up the <style> tag:
    $element = array(
      '#type' => 'markup',
      '#markup' => '
        <style type="text/css">
          #homepage-' . $variables['node']->nid . '-events {
            background-image: url(\'' . image_style_url('homepage_bg_img_full', $variables['field_hp_events_images'][$selected_welcome_image]['uri']) . '\');
          }
        </style>',
    );

    drupal_add_html_head($element, 'fordsport-homepage-events-background');
  }

  // On homepages, render the forthcoming and past events blocks into extra
  // template variables.
  if ($variables['type'] == 'homepage') {
    $variables['events_coming_up'] = views_embed_view('events', 'block_events_coming_up');
    $variables['recent_events'] = views_embed_view('events', 'block_recent_events');
  }

  // Create the galleries view for the homepage, and get data for the background
  // image.
  if ($variables['type'] == 'homepage') {
    $variables['photo_galleries'] = views_embed_view('galleries', 'homepage_gallery_block');

    $galleries_view = views_get_view('galleries');
    $galleries_view->set_display('homepage_gallery_block');
//    $galleries_view->set_items_per_page(4);
//    $galleries_view->pre_execute();
    $galleries_view->execute();
//    print $galleries_view->render();

    // If the view has a result, get the first nid and load the node.
    if (isset($galleries_view->result) && (count($galleries_view->result) > 0)) {
      $first_gallery_nid = $galleries_view->result[0]->nid;

      $first_gallery_node = node_load($first_gallery_nid);

      // Set up the <style> tag:
      $element = array(
        '#type' => 'markup',
        '#markup' => '
        <style type="text/css">
          .page-panel.page-panel-no-bg-image.hp-galleries {
            background-image: url(\'' . image_style_url('homepage_bg_img_full', $first_gallery_node->field_gallery_best_image[LANGUAGE_NONE][0]['uri']) . '\');
          }
        </style>',
      );

      drupal_add_html_head($element, 'fordsport-homepage-galleries-background');
    }
  }

  // Hide the Google map link field:
  if ($variables['page'] && ($variables['view_mode'] == 'full')) {
    hide($variables['content']['field_event_google_map_link']);
  }
}

/**
 * Implements template_preprocess_field
 *
 */
function zurb_foundation_fordsport_preprocess_field(&$variables, $hook) {
  /* Set shortcut variables. Hooray for less typing! */
  $name = $variables['element']['#field_name'];
  $bundle = $variables['element']['#bundle'];
  $mode = $variables['element']['#view_mode'];
  $classes = & $variables['classes_array'];
  $title_classes = & $variables['title_attributes_array']['class'];
  $content_classes = & $variables['content_attributes_array']['class'];
  $item_classes = array();

  /* Global field classes */
  $classes[] = 'field-wrapper';
  $title_classes[] = 'field-label';
  $content_classes[] = 'field-items';
  $item_classes[] = 'field-item';

  /* Uncomment the lines below to see variables you can use to target a field */
  // print '<strong>Name:</strong> ' . $name . '<br/>';
  // print '<strong>Bundle:</strong> ' . $bundle  . '<br/>';
  // print '<strong>Mode:</strong> ' . $mode .'<br/>';

  /* Add specific classes to targeted fields */
  switch ($mode) {
    /* All teasers */
    case 'teaser':
      switch ($name) {
        /* Teaser read more links */
        case 'node_link':
          $item_classes[] = 'more-link';
          break;
        /* Teaser descriptions */
        case 'body':
        case 'field_description':
          $item_classes[] = 'description';
          break;
      }
      break;
  }

  switch ($name) {
    case 'field_authors':
      $title_classes[] = 'inline';
      $content_classes[] = 'authors';
      $item_classes[] = 'author';
      break;

//    // Add the "button" class to the homepage welcome CTAs:
//    case 'field_hp_welcome_ctas':
//      $title_classes[] = 'button button-title_class';
//      $content_classes[] = 'button button-content_class';
//      $item_classes[] = 'button button-item_class';
//      break;
  }

  // Apply odd or even classes along with our custom classes to each item */
  foreach ($variables['items'] as $delta => $item) {
    $variables['item_attributes_array'][$delta]['class'] = $item_classes;
    $variables['item_attributes_array'][$delta]['class'][] = $delta % 2 ? 'even' : 'odd';
  }

  // Work out whether we're showing a node and if it's in teaser form:
  if (array_key_exists('element', $variables)
    && array_key_exists('#view_mode', $variables['element'])) {
    if ($variables['element']['#view_mode'] == 'teaser') {
      $variables['teaser'] = TRUE;
      $variables['full'] = FALSE;
    }
    else {
      $variables['teaser'] = FALSE;
      $variables['full'] = TRUE;
    }
  }

  // Whew! I needed some help working out how to hide and adjust fields -
  // thanks http://definitivedrupal.org/resource/changing-field-values-preprocess-function-figuring-it-out !! :)

  // Add the Google Map link into the event location field on full node view:
  if (($variables['element']['#field_name'] == 'field_event_location')
    && isset($variables['full']) && ($variables['full'])) {
    // Find field_event_google_map_link and if present, render and append to
    // field_event_location:
    if (is_array($variables['element']['#object']->field_event_google_map_link)
      && !empty($variables['element']['#object']->field_event_google_map_link)) {
      $url = $variables['element']['#object']->field_event_google_map_link[LANGUAGE_NONE][0]['url'];
      $title = $variables['element']['#object']->field_event_google_map_link[LANGUAGE_NONE][0]['title'];
      $attributes = $variables['element']['#object']->field_event_google_map_link[LANGUAGE_NONE][0]['attributes'];

      $html = ' <span class="view-on-google-maps">&mdash; ' . l($title, $url, array('attributes' => $attributes)) . '</span>';

      $variables['items'][0]['#markup'] .= $html;
    }
  }

  // If the event organiser name and web link have been filled in, merge the two
  // fields so the organiser name is a link. We change the organiser name into
  // a link here, and hide it in zurb_foundation_fordsport_preprocess_node:
  if (($variables['element']['#field_name'] == 'field_event_organiser_name')
    && isset($variables['full']) && ($variables['full'])
    && is_array($variables['element']['#object']->field_event_organiser_website)
    && !empty($variables['element']['#object']->field_event_organiser_website)) {
    $url = $variables['element']['#object']->field_event_organiser_website[LANGUAGE_NONE][0]['url'];
    $title = $variables['element'][0]['#markup'];
    $attributes = (array)$variables['element']['#attributes'];
//      + array('class' => 'field-event-organiser-name-website-link');

    $html = l($title, $url, array('attributes' => $attributes));

    $variables['items'][0]['#markup'] = $html;
  }

//  // On homepages, add a button class to the calls to action:
//  if ($variables['element']['#field_name'] == 'field_hp_welcome_ctas') {
//    // Append the class of "button":
//    $variables['classes_array'][] = 'button';
//  }

  // Check that all elements in the classes_array are strings, not arrays.
  if (array_key_exists('classes_array', $variables)) {
    foreach ($variables['classes_array'] as &$variable) {
      // Force a cast to string:
      if (is_array($variable)) {
        $variable = implode(' ', $variable);
      }
    }
  }
}

/**
 * Implements hook_preprocess_block()
 */
//function zurb_foundation_fordsport_preprocess_block(&$variables) {
//  // Add wrapping div with global class to all block content sections.
//  $variables['content_attributes_array']['class'][] = 'block-content';
//
//  // Convenience variable for classes based on block ID
//  $block_id = $variables['block']->module . '-' . $variables['block']->delta;
//
//  // Add classes based on a specific block
//  switch ($block_id) {
//    // System Navigation block
//    case 'system-navigation':
//      // Custom class for entire block
//      $variables['classes_array'][] = 'system-nav';
//      // Custom class for block title
//      $variables['title_attributes_array']['class'][] = 'system-nav-title';
//      // Wrapping div with custom class for block content
//      $variables['content_attributes_array']['class'] = 'system-nav-content';
//      break;
//
//    // User Login block
//    case 'user-login':
//      // Hide title
//      $variables['title_attributes_array']['class'][] = 'element-invisible';
//      break;
//
//    // Example of adding Foundation classes
//    case 'block-foo': // Target the block ID
//      // Set grid column or mobile classes or anything else you want.
//      $variables['classes_array'][] = 'six columns';
//      break;
//  }
//
//  // Add template suggestions for blocks from specific modules.
//  switch($variables['elements']['#block']->module) {
//    case 'menu':
//      $variables['theme_hook_suggestions'][] = 'block__nav';
//    break;
//  }
//}

//function zurb_foundation_fordsport_preprocess_views_view(&$variables) {
//}

/**
 * Implements template_preprocess_panels_pane().
 *
 */
//function zurb_foundation_fordsport_preprocess_panels_pane(&$variables) {
//}

/**
 * Implements template_preprocess_views_views_fields().
 *
 */
//function zurb_foundation_fordsport_preprocess_views_view_fields(&$variables) {
//}

/**
 * Implements theme_form_element_label()
 * Use foundation tooltips
 */
//function zurb_foundation_fordsport_form_element_label($variables) {
//  if (!empty($variables['element']['#title'])) {
//    $variables['element']['#title'] = '<span class="secondary label">' . $variables['element']['#title'] . '</span>';
//  }
//  if (!empty($variables['element']['#description'])) {
//    $variables['element']['#description'] = ' <span data-tooltip="top" class="has-tip tip-top" data-width="250" title="' . $variables['element']['#description'] . '">' . t('More information?') . '</span>';
//  }
//  return theme_form_element_label($variables);
//}

/**
 * Implements hook_preprocess_button().
 */
//function zurb_foundation_fordsport_preprocess_button(&$variables) {
//  $variables['element']['#attributes']['class'][] = 'button';
//  if (isset($variables['element']['#parents'][0]) && $variables['element']['#parents'][0] == 'submit') {
//    $variables['element']['#attributes']['class'][] = 'secondary';
//  }
//}

/**
 * Implements hook_form_alter()
 * Example of using foundation sexy buttons
 */
//function zurb_foundation_fordsport_form_alter(&$form, &$form_state, $form_id) {
//  // Sexy submit buttons
//  if (!empty($form['actions']) && !empty($form['actions']['submit'])) {
//    $classes = (is_array($form['actions']['submit']['#attributes']['class']))
//      ? $form['actions']['submit']['#attributes']['class']
//      : array();
//    $classes = array_merge($classes, array('secondary', 'button', 'radius'));
//    $form['actions']['submit']['#attributes']['class'] = $classes;
//  }
//}

/**
 * Implements hook_form_FORM_ID_alter()
 * Example of using foundation sexy buttons on comment form
 */
//function zurb_foundation_fordsport_form_comment_form_alter(&$form, &$form_state) {
// Sexy preview buttons
//  $classes = (is_array($form['actions']['preview']['#attributes']['class']))
//    ? $form['actions']['preview']['#attributes']['class']
//    : array();
//  $classes = array_merge($classes, array('secondary', 'button', 'radius'));
//  $form['actions']['preview']['#attributes']['class'] = $classes;
//}


/**
 * Implements template_preprocess_panels_pane().
 */
// function zurb_foundation_preprocess_panels_pane(&$variables) {
// }

/**
 * Implements template_preprocess_views_views_fields().
 */
/* Delete me to enable
function THEMENAME_preprocess_views_view_fields(&$variables) {
 if ($variables['view']->name == 'nodequeue_1') {

   // Check if we have both an image and a summary
   if (isset($variables['fields']['field_image'])) {

     // If a combined field has been created, unset it and just show image
     if (isset($variables['fields']['nothing'])) {
       unset($variables['fields']['nothing']);
     }

   } elseif (isset($variables['fields']['title'])) {
     unset ($variables['fields']['title']);
   }

   // Always unset the separate summary if set
   if (isset($variables['fields']['field_summary'])) {
     unset($variables['fields']['field_summary']);
   }
 }
}

// */

/**
 * Implements hook_css_alter().
 */
//function zurb_foundation_fordsport_css_alter(&$css) {
//  // Always remove base theme CSS.
//  $theme_path = drupal_get_path('theme', 'zurb_foundation');
//
//  foreach($css as $path => $values) {
//    if(strpos($path, $theme_path) === 0) {
//      unset($css[$path]);
//    }
//  }
//}

/**
 * Implements hook_js_alter().
 */
//function zurb_foundation_fordsport_js_alter(&$js) {
//  // Always remove base theme JS.
//  $theme_path = drupal_get_path('theme', 'zurb_foundation');
//
//  foreach($js as $path => $values) {
//    if(strpos($path, $theme_path) === 0) {
//      unset($js[$path]);
//    }
//  }
//}
