<?php

/**
 * Common template file for the fences module.
 */

$field_label_wrapper_tag = 'h3';
$field_contents_wrapper_tag = 'div';

if ($element['#label_display'] == 'inline') {
  $field_label_wrapper_tag = 'span';
  $field_contents_wrapper_tag = 'span';
}

?>
<div class="field-wrapper<?php if ($element['#label_display'] == 'inline'): ?> field-wrapper-label-inline<?php endif; ?>">

<!--  If the label is inline, display it as a span inside the element's tag -->
  <?php if ($element['#label_display'] == 'inline'): ?>
      <<?php print $field_contents_tag ?> class="<?php print $classes; ?>"<?php print $attributes; ?>>
      <<?php print $field_label_wrapper_tag ?> class="field-label"<?php print $title_attributes; ?>><?php print $label; ?>:</<?php print $field_label_wrapper_tag ?>>

      <<?php print $field_contents_wrapper_tag ?> class="field-content">
        <?php foreach ($items as $delta => $item): ?>
          <span class="field-contents-item field-contents-item-<?php print $delta ?>">
            <?php print render($item); ?>
          </span>
        <?php endforeach; ?>
      </<?php print $field_contents_wrapper_tag ?>>
    </<?php print $field_contents_tag ?>>

  <?php else: ?>
  <!-- Otherwise, the label should be a block-level item outside of the element's tag -->

    <?php if ($element['#label_display'] == 'above'): ?>
      <<?php print $field_label_wrapper_tag ?> class="field-label"<?php print $title_attributes; ?>><?php print $label; ?>:</<?php print $field_label_wrapper_tag ?>>
    <?php endif; ?>

    <<?php print $field_contents_wrapper_tag ?> class="field-content">
      <?php foreach ($items as $delta => $item): ?>
        <<?php print $field_contents_tag ?> class="<?php print $classes; ?>"<?php print $attributes; ?>>
          <?php print render($item); ?>
        </<?php print $field_contents_tag ?>>
      <?php endforeach; ?>
    </<?php print $field_contents_wrapper_tag ?>>
  <?php endif; ?>

</div>
