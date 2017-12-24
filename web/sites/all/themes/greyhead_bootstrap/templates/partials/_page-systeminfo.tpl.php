<?php
/**
 * @file: system information partial.
 */
?>

<?php if ($systeminfo = render($page['systeminfo'])): ?>
  <div class="system-info-wrapper">
    <footer class="system-info container">
      <?php print $systeminfo; ?>
    </footer>
  </div>
<?php endif ?>
