<?php
/**
 * @file
 * photo_gallery.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function photo_gallery_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function photo_gallery_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function photo_gallery_image_default_styles() {
  $styles = array();

  // Exported image style: photo_gallery_1200x1200.
  $styles['photo_gallery_1200x1200'] = array(
    'label' => 'Photo gallery - fit within 1200x1200',
    'effects' => array(
      16 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1200,
          'height' => 1200,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: photo_gallery_300x300.
  $styles['photo_gallery_300x300'] = array(
    'label' => 'Photo gallery - fit within 300x300',
    'effects' => array(
      13 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 300,
          'height' => 300,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: photo_gallery_600x600.
  $styles['photo_gallery_600x600'] = array(
    'label' => 'Photo gallery - fit within 600x600',
    'effects' => array(
      14 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 600,
          'height' => 600,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: photo_gallery_900x900.
  $styles['photo_gallery_900x900'] = array(
    'label' => 'Photo gallery - fit within 900x900',
    'effects' => array(
      15 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 900,
          'height' => 900,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function photo_gallery_node_info() {
  $items = array(
    'photo_gallery' => array(
      'name' => t('Photo gallery'),
      'base' => 'node_content',
      'description' => t('Create a photo gallery page to show one or more images. Photos will be scaled and cropped to display in a one, two, three or four column layout. Clicking an image will open a "lightbox" pop-over window with a larger view of the image, and the image\'s title (optional).'),
      'has_title' => '1',
      'title_label' => t('Gallery title'),
      'help' => t('Create a photo gallery page to show one or more images. Photos will be scaled and cropped to display in a one, two, three or four column layout. Clicking an image will open a "lightbox" pop-over window with a larger view of the image, and the image\'s title (optional).'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
