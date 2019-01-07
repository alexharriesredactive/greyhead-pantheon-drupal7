<?php

/**
 * @file
 * Custom content type.
 *
 * This content type is nothing more than a title and a body that is entered
 * by the user and run through standard filters. The information is stored
 * right in the config, so each custom content is unique.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Homepage grid button'),
  'no title override' => TRUE,
  'category' => t('Widgets'),
  'edit form' => 'homepage_button_message_pane_edit_form',
  'render callback' => 'homepage_button_message_pane_render',
  'all contexts' => TRUE,
);

function homepage_button_homepage_button_content_type_render($subtype, $conf, $context = NULL) {
	$block->content = ($conf['small_caption']);
	return $block;
}

function homepage_button_homepage_button_content_type_edit_form(&$form, &$form_state) {
	$conf = $form_state['conf'];
	$form['small_caption'] = array(
	'#type' => 'textfield',
	'#title' => 'Small Caption',
	'#size' => 60,
	'#description' => t('The first caption that will be displayed at the top'),
	'#default_value' => '',
	);
	return $form;
}

function homepage_button_homepage_button_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
