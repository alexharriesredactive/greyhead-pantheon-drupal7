<?php
/**
 * @file
 * Adestra Default Rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function adestra_default_rules_configuration() {
  $items = array();

  $items['rules_adestra_on_xmlrpc_error_email_site_mail'] = entity_import('rules_config', '{ "rules_adestra_on_xmlrpc_error_email_site_mail" : {
        "LABEL" : "On Adestra XMLRPC error email site mail",
        "PLUGIN" : "reaction rule",
        "TAGS" : [ "adestra" ],
        "REQUIRES" : [ "rules", "adestra" ],
        "ON" : [ "adestra_xmlrpc_error" ],
        "DO" : [
          { "mail" : {
              "to" : [ "site:mail" ],
              "subject" : "Adestra - Error returned from XMLRPC",
              "message" : [ "xmlrpc-error-msg" ],
              "from" : [ "site:mail" ]
            }
          }
        ]
      }
    }'
  );

  return $items;
}
