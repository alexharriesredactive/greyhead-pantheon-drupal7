<?php
/**
 * @file
 * Template partial which renders out any sharing links and submitted-by text.
 */
?>

<div class="social-media-links-and-submitted">
  <?php if (isset($add_to_any_links)): ?>
    <div class="social-media-links">
      <?php print $add_to_any_links ?>
    </div>
  <?php endif ?>

  <?php if ($submitted_by = render($content['submitted_by'])): ?>
    <div class="submitted submitted-by">
      <?php print $submitted_by; ?>
    </div>
  <?php endif ?>

  <?php if ($display_submitted): ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>
</div>

