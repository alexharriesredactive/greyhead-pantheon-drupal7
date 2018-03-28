/**
 * @file: greyhead-encrypted-field.js
 */

var Drupal = Drupal || {};

(function ($, Drupal) {
  "use strict";

  /**
   * Hide the content of a decrypted .contenteditable until you hover over it.
   */
  Drupal.behaviors.greyheadEncryptedFieldHideContents = {
    attach: function (context, settings) {
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
    }
  };
})
(jQuery, Drupal);
