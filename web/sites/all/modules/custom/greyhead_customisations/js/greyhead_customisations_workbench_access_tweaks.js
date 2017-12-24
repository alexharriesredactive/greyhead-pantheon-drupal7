/**
 * @file: greyhead_customisations_workbench_access_tweaks.js
 */

var Drupal = Drupal || {};

(function ($, Drupal) {
  "use strict";

  /**
   * If the user is logged in and we're on a node edit page, add the Javascript
   * which tries to automagically select the right menu parent when the
   * Workbench Access module's section is selected, and vice-versa.
   */
  console.log('Monkey');

  Drupal.behaviors.greyheadCorrectBodyMarginForAdminMenu = {
    attach: function (context, settings) {
      // Detect if we've loaded a page which has no workbench section and is
      // already in the menu; if so, try to match the section to the menu.
      if ($('select[name="workbench_access"]').val() == '') {
        greyhead_copy_menu_parent_to_workbench_section('select[name="menu[parent]"]', 'select[name="workbench_access"]');
      }

      function greyhead_copy_menu_parent_to_workbench_section(element_to_copy_from_selector, element_to_copy_to_selector) {
        console.log('element_to_copy_from_selector', element_to_copy_from_selector);
        console.log('element_to_copy_to_selector', element_to_copy_to_selector);
        console.log($(element_to_copy_from_selector).find(":selected").val());

        var selected_menu_parent = $(element_to_copy_from_selector).find(":selected").val();

        if (typeof selected_menu_parent !== 'undefined') {
          // Get the mlid by splitting the value at ":".
          var mlid_parts = selected_menu_parent.split(':');

          // Get the mlid which is the second array value.
          // If the mlid is 0, it's a menu parent - use the menu name.
          if (mlid_parts[1] > 0) {
            var mlid = mlid_parts[1];
          }
          else {
            var mlid = mlid_parts[0];
          }

          // Set the Workbench Access drop-down to the right mlid, if it exists.
          var workbench_access_menu_option_selector = $(element_to_copy_to_selector + ' option[value="' + mlid + '"]');
          console.log('workbench_access_menu_option_selector:', workbench_access_menu_option_selector);

          if (workbench_access_menu_option_selector.length > 0) {
            console.log('Found it: length ', workbench_access_menu_option_selector);
            console.log('Copying to:', element_to_copy_to_selector);

            $(element_to_copy_to_selector).val(mlid);
            console.log(element_to_copy_to_selector + ' val: ', $(element_to_copy_to_selector).val());
          }
          else {
            console.log('Not found. Sadface.');
            // :not([disabled])
          }
        }
      }

      function greyhead_copy_workbench_section_to_menu_parent(element_to_copy_from_selector, element_to_copy_to_selector) {
        console.log('element_to_copy_from_selector', element_to_copy_from_selector);
        console.log('element_to_copy_to_selector', element_to_copy_to_selector);
        console.log($(element_to_copy_from_selector).find(":selected").val());

        var selected_menu_parent = $(element_to_copy_from_selector).find(":selected").val();
        console.log('selected_menu_parent:', selected_menu_parent);

        // Get the corresponding main menu option, if it exists.
        // If the selected item is numeric, we've been given a child item of a
        // top-level menu; otherwise, it's a menu parent item.
        if ($.isNumeric(selected_menu_parent)) {
          var mlid = selected_menu_parent;
          var selector = 'option[value$="' + selected_menu_parent + '"]';
        }
        else {
          var selector = 'option[value="' + selected_menu_parent + ':0"]';
        }

        var main_menu_option = $(element_to_copy_to_selector + ' ' + selector).attr('value');
        
        if (typeof main_menu_option !== 'undefined') {
          console.log('main_menu_option:', main_menu_option);

          if (main_menu_option.length > 0) {
            // Set the Menu Parent drop-down to the right mlid, if it exists.
            console.log('Set the Menu Parent drop-down to the right mlid, if it exists.');

            var main_menu_option_selector = $(element_to_copy_to_selector + ' option[value="' + main_menu_option + '"]');
            console.log('main_menu_option_selector:', main_menu_option_selector);

            if (main_menu_option_selector.length > 0) {
              console.log('Found it: length ', main_menu_option_selector);

              $(element_to_copy_to_selector).val(main_menu_option);
            }
            else {
              console.log('Not found. Sadface.');
            }
          }
        }
      }

      $('select[name="workbench_access"]').on('change', function () {
        greyhead_copy_workbench_section_to_menu_parent('select[name="workbench_access"]', 'select[name="menu[parent]"]');
      });

      $('select[name="menu[parent]"]').on('change', function () {
        greyhead_copy_menu_parent_to_workbench_section('select[name="menu[parent]"]', 'select[name="workbench_access"]');
      });
    }
  };
})(jQuery, Drupal);
