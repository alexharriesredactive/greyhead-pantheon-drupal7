<?php
/**
 * @file
 * news_articles.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function news_articles_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'article';
  $context->description = 'Context activated when viewing articles';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'article' => 'article',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'cck_blocks-field_image' => array(
          'module' => 'cck_blocks',
          'delta' => 'field_image',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context activated when viewing articles');
  $export['article'] = $context;

  return $export;
}
