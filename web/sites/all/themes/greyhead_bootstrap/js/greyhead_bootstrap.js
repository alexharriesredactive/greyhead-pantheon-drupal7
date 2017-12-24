/**
 * @file: greyhead_bootstrap.js
 *
 * Little JS nips and tucks go in here.
 */

var Drupal = Drupal || {};

(function($, Drupal){
  "use strict";

  /**
   * Make icon font elements clickable.
   */
  Drupal.behaviors.greyheadMakeIconFontElementsClickable = {
    attach: function(context, settings) {
      // Un-hide elements marked as only-visible-with-js.
      $('.only-visible-with-js').each(function() {
        $(this).removeClass('only-visible-with-js');
      });

      // Hide elements marked as only-visible-without-js.
      $('.only-visible-without-js').each(function() {
        $(this).hide();
      });

      // Set classes on togglers and hide toggled elements which are closed.
      $('.toggler').each(function() {
        var startingState = $(this).data('state');
        var closedClass = $(this).data('closed-class');
        var openClass = $(this).data('open-class');
        var targetElement = $(this).data('target');

        // Note that if "state" is "closed", we don't want to toggle the menu
        // open, and vice-versa! :)
        var state = '';

        if (startingState == 'open') {
          state = 'closed';
        }
        else if (startingState == 'closed') {
          state = 'open';
        }

        toggle(state, closedClass, openClass, targetElement, $(this));
      });

      // Add a click handler to togglers.
      $('.toggler').click(function() {
        var state = $(this).data('state');
        var closedClass = $(this).data('closed-class');
        var openClass = $(this).data('open-class');
        var targetElement = $(this).data('target');

        toggle(state, closedClass, openClass, targetElement, $(this));
      });

      /**
       * Toggles an element open or closed and updates its toggler element.
       *
       * @param closedClass
       * @param openClass
       * @param targetElement
       * @param toggler
       */
      function toggle(state, closedClass, openClass, targetElement, toggler) {
        // If state is "open", remove the closedClass and add the openClass to
        // the toggler, and set the targetElement to visible.
        if (state == 'open') {
          toggler.removeClass(openClass + ' toggler-open').addClass(closedClass + ' toggler-closed').data('state', 'closed').css('z-index', 0);
          $('#' + targetElement).slideUp(200).removeClass('showhideable-target-open').addClass('showhideable-target-closed').css('z-index', 0);

          // Also put the site name back to its correct z-index.
          $('h1.site-name-link').removeClass('toggler-open');
        }
        // If state is "closed", remove the openClass and add the closedClass to
        // the toggler, and set the targetElement to hidden.
        else if (state == 'closed') {
          toggler.removeClass(closedClass + ' toggler-closed').addClass(openClass + ' toggler-open').data('state', 'open').css('z-index', 10001);
          $('#' + targetElement).slideDown(200).removeClass('showhideable-target-closed').addClass('showhideable-target-open').css('z-index', 10000);

          // Also put the site name back to its correct z-index.
          $('h1.site-name-link').addClass('toggler-open');
        }
      }

      // <span id="toggle-header-search"
      // class="eleganticon icon_search hidden"
      // data-state="closed"
      // data-closed-class="icon_menu" data-open-class="icon_close_alt2"
      // data-target="header-search-collapsible"></span>

      $(".encrypted-field .contenteditable").each(function () {
        var decryptedText = $(this).text();
        var decryptedTextLength = decryptedText.length + 1;
        var stars = Array(decryptedTextLength).join('*');

        $(this).data('decrypted-text', decryptedText);
        $(this).data('stars', stars);
        $(this).text(stars);

        $(this).hover(function () {
          $(this).text($(this).data('decrypted-text'));
        }, function () {
          $(this).text($(this).data('stars'));
        });
      });



      if ($('body', context).hasClass('admin-menu')) {
        var height = $('#admin-menu').height();

        if (!(height === null) && (height > 0)) {
          console.log('Setting body top-margin to ' + height + 'px.');

          $('body', context).attr('style', 'margin-top: ' + $('#admin-menu').height() + 'px !important');
        }
      }
    }
  };
})(jQuery, Drupal);
