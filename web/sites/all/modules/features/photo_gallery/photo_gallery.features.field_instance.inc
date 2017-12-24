<?php
/**
 * @file
 * photo_gallery.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function photo_gallery_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-photo_gallery-field_photo_gallery_columns'.
  $field_instances['node-photo_gallery-field_photo_gallery_columns'] = array(
    'bundle' => 'photo_gallery',
    'default_value' => array(
      0 => array(
        'value' => 3,
      ),
    ),
    'deleted' => 0,
    'description' => 'Choose how many columns of images should be shown on the gallery page.',
    'display' => array(
      'cck_blocks' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'default' => array(
        'label' => 'hidden',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_key',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_photo_gallery_columns',
    'label' => 'How many columns of images',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-photo_gallery-field_photo_gallery_images'.
  $field_instances['node-photo_gallery-field_photo_gallery_images'] = array(
    'bundle' => 'photo_gallery',
    'deleted' => 0,
    'description' => 'Upload one or more images to this gallery.',
    'display' => array(
      'cck_blocks' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'default' => array(
        'label' => 'hidden',
        'module' => 'colorbox',
        'settings' => array(
          'colorbox_caption' => 'auto',
          'colorbox_caption_custom' => '',
          'colorbox_gallery' => 'post',
          'colorbox_gallery_custom' => '',
          'colorbox_image_style' => 'photo_gallery_1200x1200',
          'colorbox_multivalue_index' => NULL,
          'colorbox_node_style' => 'photo_gallery_600x600',
          'colorbox_node_style_first' => '',
        ),
        'type' => 'colorbox',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_photo_gallery_images',
    'label' => 'Images',
    'required' => 1,
    'settings' => array(
      'alt_field' => 1,
      'default_image' => 0,
      'file_directory' => 'photo-galleries/images',
      'file_extensions' => 'png gif jpg jpeg',
      'imagecrop' => array(
        'job_vacancy_image' => 0,
        'news_article_image' => 0,
        'page_banner' => 0,
      ),
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '100x10',
      'title_field' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'photo_gallery_600x600',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Choose how many columns of images should be shown on the gallery page.');
  t('How many columns of images');
  t('Images');
  t('Upload one or more images to this gallery.');

  return $field_instances;
}
