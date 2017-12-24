/**
 * @file: greyhead_customisations.js
 *
 * Little JS nips and tucks go in here.
 */

var Drupal = Drupal || {};

(function($, Drupal){
  "use strict";

  /**
   * If we're on a node edit page and the URL slug field is present, copy the
   * menu title into the URL slug field converted to lowercase alphanumeric-
   * only.
   */
  Drupal.behaviors.greyheadAutoPopulateURLSlugField = {
    attach: function (context, settings) {
      // Are we on a node edit/add page?
      var nodeFormSelector = 'form.node-form';
      var $nodeForm = $(nodeFormSelector);
      if ($nodeForm.length) {
        // Do we have a URL slug field?
        var $urlSlugField = $(nodeFormSelector + ' #edit-field-parapg-url-slug-und-0-value');
        if ($urlSlugField.length) {
          // // Yes, we are Go. Add a watcher to add a data-manuallyEdited=yes
          // // attribute if the field is changed.
          // $urlSlugField.data('manuallyEdited', 'no').blur(function() {
          //   $(this).data('manuallyEdited', 'yes');
          // });

          // Get the node title field.
          var $nodeTitleField = $(nodeFormSelector + ' #edit-title');

          // When focus leaves the title or slug fields, check if the slug
          // is empty and copy the title in if it is.
          $nodeTitleField.blur(function() {
            greyheadCopyNodeTitleToURLSlugField($(this).val(), $urlSlugField);
          });

          $urlSlugField.blur(function() {
            greyheadCopyNodeTitleToURLSlugField($nodeTitleField.val(), $urlSlugField);
          });
        }
      }

      /**
       * Copy the node title to the menu title if the menu title field isn't
       * already populated.
       *
       * @param nodeTitle
       * @param menuTitleField
       */
      function greyheadCopyNodeTitleToURLSlugField(nodeTitle, $urlSlugField) {
        if ($urlSlugField.val().length == 0) {
          var nodeTitleConverted = nodeTitle.replace(/\W/g, '-').toLowerCase();
          $urlSlugField.val(nodeTitleConverted);
        }
      }
    }
  };

  /**
   * If we're on an admin page, get the height of the admin toolbar and set the
   * body's top margin accordingly.
   */
  Drupal.behaviors.greyheadCorrectBodyMarginForAdminMenu = {
    attach: function(context, settings) {
      if ($('body', context).hasClass('admin-menu')) {
        var height = $('#admin-menu').height();

        if (!(height === null) && (height > 0)) {
          console.log('Setting body top-margin to ' + height + 'px.');

          $('body', context).attr('style', 'margin-top: ' + $('#admin-menu').height() + 'px !important');
        }
      }
    }
  };

  /**
   * Convert DisplaySuite Javascript links into Real Links (TM).
   */
  Drupal.behaviors.convertDisplaySuiteJSLinksToReal = {
    attach: function(context, settings) {
      this.convertDisplaySuiteJSLinksToReal(context, settings);
    },

    convertDisplaySuiteJSLinksToReal: function(context, settings) {
      // Do we have any DS pseudo-links?
      if ($('[about][onClick]').length) {
        $('[about][onClick]').each(function() {
          // Change "about" to "href".
          $(this).attr('href', $(this).attr('about'));

          // Remove onClick.
          $(this).removeAttr('onClick');

          // Convert element to an <a />. This has to happen last.
          $(this).changeElementType('a');
        });
      }
    }
  };

  /**
   * This function stolen from https://stackoverflow.com/a/8584217 with love.
   *
   * @param newType
   */
  $.fn.changeElementType = function(newType) {
    var attrs = {};

    $.each(this[0].attributes, function(idx, attr) {
      attrs[attr.nodeName] = attr.nodeValue;
    });

    this.replaceWith(function() {
      return $("<" + newType + "/>", attrs).append($(this).contents());
    });
  };
})(jQuery, Drupal);
