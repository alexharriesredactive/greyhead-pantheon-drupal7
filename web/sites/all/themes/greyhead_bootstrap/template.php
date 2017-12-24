<?php

/**
 * @file
 * template.php
 */

/**
 * Implements hook_form_alter().
 *
 * @param array $form
 * @param array $form_state
 * @param null  $form_id
 */
function greyhead_bootstrap_form_alter(array &$form, array &$form_state = array(), $form_id = NULL) {
  if ($form_id) {
    switch ($form_id) {
      // Adjust the search form's submit button with a custom theme function
      // which we use to change the button to a search icon.
      case 'search_form':
        // Implement a theme wrapper to add a submit button containing a search
        // icon directly after the input element.
//        $form['basic']['keys']['#theme_wrappers'] = array('greyhead_bootstrap_search_form_wrapper');
        break;
      case 'search_block_form':
//        $form['search_block_form']['#theme_wrappers'] = array('greyhead_bootstrap_search_form_wrapper');
        break;
      
      // Rewrite the topics filter on news pages to place the label inline.
      case 'views_exposed_form':
        // Are we showing the news listing form? These are the views form IDs.
        $views_forms_we_want_to_finagle = array(
          'views-exposed-form-news-page-news-home',
          'views-exposed-form-news-news-by-date',
        );
        
        if (in_array($form['#id'], $views_forms_we_want_to_finagle)) {
          // The form contains a drop-down select at $form['topics'], and the
          // options array is in ['#options']['All']. We want to change the
          // _value_ of the 'All' topics to read t('Topics').
          $form['topics']['#options']['All'] = t('Topics');
        }
        break;
    }
  }
}

/**
 * Theme function implementation for MYTHEME_search_form_wrapper.
 *
 * @param $variables
 *
 * @return string
 */
function greyhead_bootstrap_search_form_wrapper($variables) {
  $output = '<div class="input-group">';
  $output .= $variables['element']['#children'];
  $output .= '<span class="input-group-btn">';
  $output .= '<button type="submit" class="btn btn-default">';
// We can be sure that the font icons exist in CDN.
//  if (theme_get_setting('bootstrap_cdn')) {
  $output .= _bootstrap_icon('search');
//  }
//  else {
  $output .= ' <span class="hidden">' . t('Search') . '</span>';
//  }
  $output .= '</button>';
  $output .= '</span>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements hook_preprocess_image_style().
 *
 * Add in our img-responsive class to all images.
 *
 * @param $variables
 */
function greyhead_bootstrap_preprocess_image_style(&$variables) {
  $variables['attributes']['class'][] = 'img-responsive';
}

/**
 * Implements hook_preprocess_node().
 *
 * Add the correct date format to the submitted.
 *
 * @param $variables
 */
function greyhead_bootstrap_preprocess_node(&$variables) {
  // Convenience variables rock.
  $node = &$variables['node'];
  
  /**
   * Add template suggestions for the node templating system: this adds template
   * suggestions such as:
   *
   * node--landing-page--teaser.tpl.php
   * node--landing-page--full.tpl.php
   */
  $variables['theme_hook_suggestions'][] = 'node__' . $node->type . '__' . $variables['view_mode'];
  
  $variables['submitted'] = format_date($variables['revision_timestamp'], 'date_only');
}

/**
 * Implements hook_preprocess_page().
 *
 * @param $variables
 */
function greyhead_bootstrap_preprocess_page(&$variables) {
  // In this theme, we have made the sidebars 4 columns wide, so we need to
  // adjust the number of centre columns. Note that we have no sidebars on the
  // homepage, so add in a check to make sure we fix the homepage to 12.
  if (drupal_is_front_page()) {
    $variables['content_column_class'] = ' class="col-sm-12"';
  }
  else {
    if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
      $variables['content_column_class'] = ' class="col-sm-4"';
    }
    elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
      $variables['content_column_class'] = ' class="col-sm-8"';
    }
    else {
      $variables['content_column_class'] = ' class="col-sm-12"';
    }
  }
  
  /**
   * Preprocess the breadcrumbs. Ordinarily, these are built in
   * theme_process_page(), but we are using the Menu HTML module on sites such
   * as the NHS NW London, and that results in escaped HTML such as this
   * appearing in the breadcrumbs:
   *
   * <li><a href="/yourhealth">Your &lt;br /&gt;health</a></li>
   *
   * ... which looks a bit pants, really :)
   */
  if (!isset($variables['breadcrumb'])) {
    $variables['breadcrumb'] = theme('breadcrumb', array('breadcrumb' => greyhead_bootstrap_drupal_get_breadcrumb()));
  }
  
  /**
   * If search is allowed, create a search form which we will add into the
   * header.
   */
  $variables['header_search'] = '';
  if (user_access('search content')) {
    $variables['header_search'] = module_invoke('search', 'block_view', 'search');
  }
  
  // Reference $node directly for simplesness. ;)
  if (isset($variables['node'])) {
    $node = &$variables['node'];
  }
  
  // Set some empty variables so we don't get undefined variable errorage.
  $variables['icon'] = $variables['brand_colour'] = '';
  
  // Add a number of template suggestions based on node type and nid.
  // Code from https://www.drupal.org/node/1142800#comment-4433994
  // Do we have a node?
  if (isset($node)) {
    // Reference suggestions cuz it's very long.
    $suggestions = &$variables['theme_hook_suggestions'];
    
    // Get path arguments.
    $args = arg();
    
    // Remove first argument of "node".
    unset($args[0]);
    
    // Set type.
    $type = "page__type_{$node->type}";
    
    // Bring it all together.
    $suggestions = array_merge(
      $suggestions,
      array($type),
      theme_get_suggestions($args, $type)
    );
    
    // if the url is: 'http://domain.com/node/123/edit'
    // and node type is 'blog'..
    //
    // This will be the suggestions:
    //
    // - page__node
    // - page__node__%
    // - page__node__123
    // - page__node__edit
    // - page__type_blog
    // - page__type_blog__%
    // - page__type_blog__123
    // - page__type_blog__edit
    //
    // Which connects to these templates:
    //
    // - page--node.tpl.php
    // - page--node--%.tpl.php
    // - page--node--123.tpl.php
    // - page--node--edit.tpl.php
    // - page--type-blog.tpl.php          << this is what you want.
    // - page--type-blog--%.tpl.php
    // - page--type-blog--123.tpl.php
    // - page--type-blog--edit.tpl.php
    //
    // Latter items take precedence.
    
    // If we're on the homepage or a landing page, unset and hide the sidebars,
    // and reset the css on the main page content to 12 columns.
    if (($node->type == 'homepage') || ($node->type == 'landing_page')) {
      // We're displaying a homepage or landing page:
      hide($variables['page']['sidebar_first']);
      hide($variables['page']['sidebar_second']);
      $variables['content_column_class'] = ' class="col-sm-12"';
    }
  }
  
  // Override the primary nav set by Bootstrap - we use menu_tree_all_data()
  // instead of menu_tree() so that we can get all menu links, not just the
  // expanded items in the current path.
  if ($variables['primary_nav'] && isset($variables['main_menu'])) {
    // Build links.
    $primary_nav_unprocessed = menu_tree_all_data(variable_get('menu_main_links_source', 'main-menu'));
    
    // Add the active path to the tree - see
    // https://api.drupal.org/comment/50288#comment-50288
    if (function_exists('menu_tree_add_active_path') && is_array($primary_nav_unprocessed)) {
      menu_tree_add_active_path($primary_nav_unprocessed);
    }
    
    $variables['primary_nav'] = menu_tree_output($primary_nav_unprocessed);
    
    // Provide default theme wrapper function.
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }
  
  // Remove the "container" class from the navbar classes so the header can
  // expand to full width.
  $index = array_search('container', $variables['navbar_classes_array']);
  if (!($index === FALSE)) {
//    unset($variables['navbar_classes_array'][$index]);
    $variables['navbar_classes_array'][$index] = 'container-fluid';
  }
  
  $variables['navbar_classes'] = implode(' ', $variables['navbar_classes_array']);
  
  // Add icon font to the page.
  drupal_add_html_head_link(array(
    'rel' => 'stylesheet',
    'href' => '//fonts.googleapis.com/css?family=Della+Respira|Raleway:400,400i,700,700i|Tangerine',
    'type' => 'text/css',
  ));
}

/**
 * Implements hook_preprocess_html().
 *
 * @param $variables
 */
function greyhead_bootstrap_preprocess_html(&$variables) {
  // Add a meta-tag to tell IE 8 not to panic when rendering pages.
  // Code from https://api.drupal.org/comment/18004#comment-18004
  // <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1,IE=8">
  $meta_ie_render_engine = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'content' => 'IE=edge,chrome=1,IE=8',
      'http-equiv' => 'X-UA-Compatible',
    ),
  );
  
  // Add header meta tag for IE to head
  drupal_add_html_head($meta_ie_render_engine, 'meta_ie_render_engine');
  
  // Workaround to prevent console.logs from throwing IE errors of DOOOM.
  $js = 'if (!window.console) console = {log: function() {}};';
  drupal_add_js($js, array(
    'type' => 'inline',
    'scope' => 'header',
    'weight' => -1000,
  ));
  
  // Check if the classes array exists and append our new class.
  if (!array_key_exists('classes_array', $variables)) {
    $variables['classes_array'] = array();
  }
  
  if (!is_array($variables['classes_array'])) {
    $variables['classes_array'] = (array) $variables['classes_array'];
  }
  
  // Add the page URL as a class to the body tag, so we can target pages by
  // their URLs.
  $variables['classes_array'][] = 'path-' . greyhead_customisations_get_class_from_url(drupal_get_path_alias($_GET['q']));
}

///**
// * Implements hook_menu_breadcrumb_alter.
// *
// * Remove any HTML in the menu link titles which has been set by the menu_html
// * contrib module.
// *
// * @param $variables
// */
//function greyhead_bootstrap_menu_breadcrumb_alter(&$active_trail, $item) {
//  foreach ($active_trail as &$active_trail_item) {
//    $active_trail_item['title'] = strip_tags($active_trail_item['title']);
//  }
//}

/**
 * Gets the breadcrumb trail for the current page.
 *
 * This function uses a custom implementation of menu_get_active_breadcrumb()
 * which removes rather than escapes any HTML in the link title.
 *
 * @return array|null
 */
function greyhead_bootstrap_drupal_get_breadcrumb() {
  $breadcrumb = greyhead_bootstrap_drupal_set_breadcrumb();
  
  if (!isset($breadcrumb)) {
    $breadcrumb = greyhead_bootstrap_menu_get_active_breadcrumb();
  }
  
  // Make the function hook_alteraboo, as a
  // hook_greyhead_bootstrap_breadcrumb(). In an ideal world, the
  // menu_trail_by_path module would provide alter hooks and we wouldn't have
  // to do this, but it doesn't, so we do. Sad face.
  drupal_alter('greyhead_bootstrap_breadcrumb', $breadcrumb);
  
  return $breadcrumb;
}

/**
 * Sets the breadcrumb trail for the current page.
 *
 * This is a separate implementation of drupal_set_breadcrumb() which we use
 * to make sure we rebuild a breadcrumb which doesn't contain any HTML.
 *
 * @param $breadcrumb
 *   Array of links, starting with "home" and proceeding up to but not including
 *   the current page.
 *
 * @return null
 */
function greyhead_bootstrap_drupal_set_breadcrumb($breadcrumb = NULL) {
  $stored_breadcrumb = &drupal_static(__FUNCTION__);
  
  if (isset($breadcrumb)) {
    $stored_breadcrumb = $breadcrumb;
  }
  return $stored_breadcrumb;
}

/**
 * Gets the breadcrumb for the current page, as determined by the active trail.
 *
 * This is a custom implementation of menu_get_active_breadcrumb()
 * which removes rather than escapes any HTML in the link title.
 *
 * @return array
 */
function greyhead_bootstrap_menu_get_active_breadcrumb() {
  $breadcrumb = array();
  
  // No breadcrumb for the front page.
  if (drupal_is_front_page()) {
    return $breadcrumb;
  }
  
  $item = menu_get_item();
  if (!empty($item['access'])) {
    $active_trail = menu_get_active_trail();
    
    // Allow modules to alter the breadcrumb, if possible, as that is much
    // faster than rebuilding an entirely new active trail.
    drupal_alter('menu_breadcrumb', $active_trail, $item);
    
    // Don't show a link to the current page in the breadcrumb trail.
    $end = end($active_trail);
    if ($item['href'] == $end['href']) {
      array_pop($active_trail);
    }
    
    // Remove the tab root (parent) if the current path links to its parent.
    // Normally, the tab root link is included in the breadcrumb, as soon as we
    // are on a local task or any other child link. However, if we are on a
    // default local task (e.g., node/%/view), then we do not want the tab root
    // link (e.g., node/%) to appear, as it would be identical to the current
    // page. Since this behavior also needs to work recursively (i.e., on
    // default local tasks of default local tasks), and since the last non-task
    // link in the trail is used as page title (see menu_get_active_title()),
    // this condition cannot be cleanly integrated into menu_get_active_trail().
    // menu_get_active_trail() already skips all links that link to their parent
    // (commonly MENU_DEFAULT_LOCAL_TASK). In order to also hide the parent link
    // itself, we always remove the last link in the trail, if the current
    // router item links to its parent.
    if (($item['type'] & MENU_LINKS_TO_PARENT) == MENU_LINKS_TO_PARENT) {
      array_pop($active_trail);
    }
    
    foreach ($active_trail as $parent) {
      $breadcrumb[] = l(strip_tags($parent['title']), $parent['href'], $parent['localized_options']);
    }
  }
  return $breadcrumb;
}

/**
 * Returns HTML for a menu link and submenu.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see     theme_menu_link()
 *
 * @ingroup theme_functions
 */
function greyhead_bootstrap_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';
  
  if ($element['#below']) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="">' . drupal_render($element['#below']) . '</ul>';
      // Generate as standard dropdown.
      $element['#attributes']['class'][] = '';
      $element['#localized_options']['html'] = TRUE;
      
      // Set dropdown trigger element to # to prevent inadvertant page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = '';
      $element['#localized_options']['attributes']['data-toggle'] = '';
    }
  }
  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Returns HTML for a wrapper for a menu sub-tree.
 *
 * @param array $variables
 *   An associative array containing:
 *   - tree: An HTML string containing the tree's items.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see     template_preprocess_menu_tree()
 * @see     theme_menu_tree()
 *
 * @ingroup theme_functions
 */
//function greyhead_bootstrap_menu_tree(&$variables) {
//  return '<ul class="menu nav">' . $variables['tree'] . '</ul>';
//}

/**
 * Bootstrap theme wrapper function for the primary menu links.
 */
function greyhead_bootstrap_menu_tree__primary(&$variables) {
  return '<ul class="menu nav navbar-nav">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the secondary menu links.
 */
function greyhead_bootstrap_menu_tree__secondary(&$variables) {
  return '<ul class="menu secondary">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree() for book module.
 */
//function greyhead_bootstrap_menu_tree__book_toc(&$variables) {
//  $output = '<div class="book-toc btn-group pull-right">';
//  $output .= '  <button type="button" class="btn btn-link dropdown-toggle" data-toggle="dropdown">';
//  $output .= t('!icon Outline !caret', array(
//    '!icon' => _bootstrap_icon('list'),
//    '!caret' => '<span class="caret"></span>',
//  ));
//  $output .= '</button>';
//  $output .= '<ul class="" role="menu">' . $variables['tree'] . '</ul>';
//  $output .= '</div>';
//  return $output;
//}

/**
 * Overrides theme_menu_tree() for book module.
 */
//function greyhead_bootstrap_menu_tree__book_toc__sub_menu(&$variables) {
//  return '<ul class="" role="menu">' . $variables['tree'] . '</ul>';
//}

/**
 * Stub implementation for hook_theme().
 *
 * Code from
 * http://www.nodenerd.com/customizing-drupal-search-form-make-use-gumbys-ui-kit
 *
 * @see MYTHEME_theme()
 * @see hook_theme()
 */
function greyhead_bootstrap_theme(&$existing, $type, $theme, $path) {
  // Custom theme hooks:
  // Do not define the `path` or `template`.
  $hook_theme = array(
    'search_form_wrapper' => array(
      'render element' => 'element',
    ),
  );
  
  return $hook_theme;
}
