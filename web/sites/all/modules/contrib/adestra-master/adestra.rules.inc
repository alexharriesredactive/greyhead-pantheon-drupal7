<?php
/**
 * @file
 * Adestra Rules Implementations.
 */


/**
 * Implements hook_rules_event_info().
 */
function adestra_rules_event_info() {
  $items = array();

  $items['adestra_xmlrpc_error'] = array(
    'label' => t('XMLRPC Error'),
    'group' => t('Adestra'),
    'variables' => array(
      'xmlrpc_errno' => array(
        'type' => 'text',
        'label' => t('The last XML-RPC client error number'),
      ),
      'xmlrpc_error_msg' => array(
        'type' => 'text',
        'label' => t('The last XML-RPC client error message'),
      ),
    ),
  );

  return $items;
}