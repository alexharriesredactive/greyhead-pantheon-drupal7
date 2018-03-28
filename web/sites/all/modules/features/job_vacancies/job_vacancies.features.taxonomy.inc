<?php
/**
 * @file
 * job_vacancies.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function job_vacancies_taxonomy_default_vocabularies() {
  return array(
    'companies' => array(
      'name' => 'Companies',
      'machine_name' => 'companies',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'language' => 'und',
      'i18n_mode' => 4,
    ),
    'job_roles' => array(
      'name' => 'Job roles',
      'machine_name' => 'job_roles',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'language' => 'und',
      'i18n_mode' => 4,
    ),
  );
}
