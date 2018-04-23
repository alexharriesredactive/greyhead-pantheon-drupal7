<!--.l-footer-->
<footer class="l-footer panel row" role="contentinfo">
  <div class="footer large-8 columns">
    <?php if (!empty($page['footer'])): ?>
  <?php print render($page['footer']); ?>
<?php endif; ?>
</div>

<div class="copyright large-4 columns">
  <?php if ($site_name) : ?>
    &copy; <?php print date('Y') . ' ' . check_plain($site_name) ?> |
  <?php endif; ?>
  <?php print l(t('Sign in'), 'user') ?> |
  <?php print l(t('Site by Greyhead'), 'http://greyhead.co.uk') ?>
</div>
</footer>
<!--/.footer-->
