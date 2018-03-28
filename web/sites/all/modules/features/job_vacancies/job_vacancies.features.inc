<?php
/**
 * @file
 * job_vacancies.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function job_vacancies_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function job_vacancies_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_default_entityform_type().
 */
function job_vacancies_default_entityform_type() {
  $items = array();
  $items['job_vacancies_apply_form'] = entity_import('entityform_type', '{
    "type" : "job_vacancies_apply_form",
    "label" : "Apply for a job vacancy",
    "data" : {
      "draftable" : 0,
      "draft_redirect_path" : "",
      "draft_button_text" : "",
      "draft_save_text" : { "value" : "", "format" : "full_html" },
      "submit_button_text" : "Submit application",
      "submit_confirm_msg" : "Thank you for applying for this position. ",
      "your_submissions" : "",
      "disallow_resubmit_msg" : "",
      "delete_confirm_msg" : "",
      "page_title_view" : "",
      "preview_page" : 0,
      "submission_page_title" : "",
      "submission_text" : { "value" : "", "format" : "full_html" },
      "submission_show_submitted" : 0,
      "submissions_view" : "default",
      "user_submissions_view" : "default",
      "form_status" : "ENTITYFORM_OPEN",
      "roles" : { "1" : "1", "2" : "2", "37840264" : 0, "25241038" : 0, "200153887" : 0 },
      "resubmit_action" : "new",
      "redirect_path" : "",
      "instruction_pre" : { "value" : "", "format" : "full_html" }
    },
    "weight" : "0",
    "paths" : []
  }');
  return $items;
}

/**
 * Implements hook_image_default_styles().
 */
function job_vacancies_image_default_styles() {
  $styles = array();

  // Exported image style: job_vacancy_image.
  $styles['job_vacancy_image'] = array(
    'label' => 'Job vacancy image',
    'effects' => array(
      2 => array(
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
        'weight' => -10,
      ),
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 800,
          'height' => 600,
        ),
        'weight' => -9,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function job_vacancies_node_info() {
  $items = array(
    'job_vacancy' => array(
      'name' => t('Job vacancy'),
      'base' => 'node_content',
      'description' => t('Create a Job Vacancy to advertise a job in the <a href="/job-vacancies">Job Vacancies section</a> of the site.'),
      'has_title' => '1',
      'title_label' => t('Job title'),
      'help' => t('Create a Job Vacancy to advertise a job in the <a href="/job-vacancies">Job Vacancies section</a> of the site.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
