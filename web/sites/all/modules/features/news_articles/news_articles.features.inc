<?php
/**
 * @file
 * news_articles.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function news_articles_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function news_articles_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function news_articles_image_default_styles() {
  $styles = array();

  // Exported image style: news_article_image.
  $styles['news_article_image'] = array(
    'label' => 'News article image',
    'effects' => array(
      3 => array(
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => 800,
          'height' => 600,
          'xoffset' => '',
          'yoffset' => '',
          'resizable' => 1,
          'downscaling' => 1,
          'aspect_ratio' => 1.333,
          'disable_if_no_data' => 1,
        ),
        'weight' => 1,
      ),
      4 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 800,
          'height' => 600,
        ),
        'weight' => 2,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function news_articles_node_info() {
  $items = array(
    'article' => array(
      'name' => t('News article'),
      'base' => 'node_content',
      'description' => t('Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
