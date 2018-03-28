<?php
/**
 * @file
 * Page callback: jQuery ScrollIt settings.
 *
 * @see paragraphs_nav_scrollit_menu()
 */

// ScrollIt defaults.
// @See https://github.com/cmpolis/scrollIt.js/blob/master/scrollIt.js#L18
define('PARAGRAPHS_NAV_SCROLLIT_UPKEY', 38);
define('PARAGRAPHS_NAV_SCROLLIT_DOWNKEY', 40);
define('PARAGRAPHS_NAV_SCROLLIT_EASING', 'linear');
define('PARAGRAPHS_NAV_SCROLLIT_SCROLLTIME', 600);
define('PARAGRAPHS_NAV_SCROLLIT_ACTIVECLASS', 'active');
define('PARAGRAPHS_NAV_SCROLLIT_ONPAGECHANGE', NULL);
define('PARAGRAPHS_NAV_SCROLLIT_TOPOFFSET', 0);

/**
 * Implements hook_form().
 */
function paragraphs_nav_scrollit_form($form, &$form_state) {
  $form = array();

  $form['paragraphs_nav_scrollit_upkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Up Key'),
    '#default_value' => variable_get('paragraphs_nav_scrollit_upkey', PARAGRAPHS_NAV_SCROLLIT_UPKEY),
    '#description' => t('The code for Up key when navigating with keyboard. For list of available keys visit <a href="@url">http://www.javascriptkeycode.com</a> Defaults to @default.', array('@url' => url('http://www.javascriptkeycode.com'), '@default' => PARAGRAPHS_NAV_SCROLLIT_UPKEY)),
    '#required' => FALSE,
  );
  $form['paragraphs_nav_scrollit_downkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Down key'),
    '#default_value' => variable_get('paragraphs_nav_scrollit_downkey', PARAGRAPHS_NAV_SCROLLIT_DOWNKEY),
    '#description' => t('The code for Down key when navigating with keyboard. For list of available keys visit <a href="@url">http://www.javascriptkeycode.com</a> Defaults to @default.', array('@url' => url('http://www.javascriptkeycode.com'), '@default' => PARAGRAPHS_NAV_SCROLLIT_DOWNKEY)),
    '#required' => FALSE,
  );
  $form['paragraphs_nav_scrollit_easing'] = array(
    '#type' => 'textfield',
    '#title' => t('Easing'),
    '#default_value' => variable_get('paragraphs_nav_scrollit_easing', PARAGRAPHS_NAV_SCROLLIT_EASING),
    '#description' => t('The value for easing.  Defaults to "@default".', array('@default' => PARAGRAPHS_NAV_SCROLLIT_EASING)),
    '#required' => FALSE,
  );
  $form['paragraphs_nav_scrollit_scrolltime'] = array(
    '#type' => 'textfield',
    '#title' => t('Scroll Time'),
    '#default_value' => variable_get('paragraphs_nav_scrollit_scrolltime', PARAGRAPHS_NAV_SCROLLIT_SCROLLTIME),
    '#description' => t('Set the speed of scrolling. Defaults to @default.', array('@default' => PARAGRAPHS_NAV_SCROLLIT_SCROLLTIME)),
    '#required' => FALSE,
  );
  $form['paragraphs_nav_scrollit_activeclass'] = array(
    '#type' => 'textfield',
    '#title' => t('Active Class'),
    '#default_value' => variable_get('paragraphs_nav_scrollit_activeclass', PARAGRAPHS_NAV_SCROLLIT_ACTIVECLASS),
    '#description' => t('This class will be used on active menu items. Defaults to "@default".', array('@default' => PARAGRAPHS_NAV_SCROLLIT_ACTIVECLASS)),
    '#required' => FALSE,
  );
  $form['paragraphs_nav_scrollit_topoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Top Offset'),
    '#default_value' => variable_get('paragraphs_nav_scrollit_topoffset', PARAGRAPHS_NAV_SCROLLIT_TOPOFFSET),
    '#description' => t('How many pixels should be the top menu pushed down. Defaults to @default.', array('@default' => PARAGRAPHS_NAV_SCROLLIT_TOPOFFSET)),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_validate().
 */
function paragraphs_nav_scrollit_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['paragraphs_nav_scrollit_upkey']) && !empty($form_state['values']['paragraphs_nav_scrollit_upkey'])) {
    form_set_error('paragraphs_nav_scrollit_upkey', t('Scroll Time must be Numeric'));
  }
  if (!is_numeric($form_state['values']['paragraphs_nav_scrollit_downkey']) && !empty($form_state['values']['paragraphs_nav_scrollit_downkey'])) {
    form_set_error('paragraphs_nav_scrollit_downkey', t('Scroll Time must be Numeric'));
  }
  if (!is_numeric($form_state['values']['paragraphs_nav_scrollit_scrolltime']) && !empty($form_state['values']['paragraphs_nav_scrollit_scrolltime'])) {
    form_set_error('paragraphs_nav_scrollit_scrolltime', t('Scroll Time must be Numeric'));
  }
  if (!is_numeric($form_state['values']['paragraphs_nav_scrollit_topoffset']) && !empty($form_state['values']['paragraphs_nav_scrollit_topoffset'])) {
    form_set_error('paragraphs_nav_scrollit_topoffset', t('Top Offset must be Numeric'));
  }
}