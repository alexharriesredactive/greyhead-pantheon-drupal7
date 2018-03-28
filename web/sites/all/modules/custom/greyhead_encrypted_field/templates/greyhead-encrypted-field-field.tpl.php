<?php
/**
 * @file
 * greyhead-encrypted-field-field.tpl.php
 *
 * Renders an encrypted field for display.
 */

/**
 * $element contains:
 *
 * '#'. GEF_FIELD_LABEL_FAPI_KEY (#gef_field_title)
 * '#'. GEF_FIELD_VALUE_FAPI_KEY (#gef_field_value)
 */

/**
 * @file    field.tpl.php
 * Default template implementation to display the value of a field.
 *
 * This file is not used and is here as a starting point for customization only.
 * @see     theme_field()
 *
 * Available variables:
 * - $items: An array of field values. Use render() to output them.
 * - $label: The item label.
 * - $label_hidden: Whether the label display is set to 'hidden'.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - field: The current template type, i.e., "theming hook".
 *   - field-name-[field_name]: The current field name. For example, if the
 *     field name is "field_description" it would result in
 *     "field-name-field-description".
 *   - field-type-[field_type]: The current field type. For example, if the
 *     field type is "text" it would result in "field-type-text".
 *   - field-label-[label_display]: The current label position. For example, if
 *     the label position is "above" it would result in "field-label-above".
 *
 * Other variables:
 * - $element['#object']: The entity to which the field is attached.
 * - $element['#view_mode']: View mode, e.g. 'full', 'teaser'...
 * - $element['#field_name']: The field name.
 * - $element['#field_type']: The field type.
 * - $element['#field_language']: The field language.
 * - $element['#field_translatable']: Whether the field is translatable or not.
 * - $element['#label_display']: Position of label display, inline, above, or
 *   hidden.
 * - $field_name_css: The css-compatible field name.
 * - $field_type_css: The css-compatible field type.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see     template_preprocess_field()
 * @see     theme_field()
 *
 * @ingroup themeable
 */
?>

<div class="encrypted-field field-item <?php print $classes; ?>"<?php print $attributes; ?> id="<?php print $element['#id'] ?>">
  <div class="encrypted-field-label <?php
  if ($element[GEF_FIELD_LABEL_FAPI_KEY . '_decrypted']): ?>decrypted<?php
  else: ?>encrypted<?php
  endif ?>"<?php print $title_attributes ?>>

    <?php print $element[GEF_FIELD_LABEL_FAPI_KEY] ?>:&nbsp;
  </div>

  <div class="encrypted-field-value <?php
  if ($element[GEF_FIELD_VALUE_FAPI_KEY . '_decrypted']): ?>decrypted<?php
  else: ?>encrypted<?php
  endif ?>"<?php print $content_attributes ?>>
    <?php if ($element[GEF_FIELD_VALUE_FAPI_KEY . '_decrypted']): ?>
      <pre contenteditable class="contenteditable"><?php print $element[GEF_FIELD_VALUE_FAPI_KEY] ?></pre>
    <?php else: ?>
    <div class="not-a-textarea"><?php print $element[GEF_FIELD_VALUE_FAPI_KEY] ?></div>
    <?php endif ?>
  </div>
</div>
