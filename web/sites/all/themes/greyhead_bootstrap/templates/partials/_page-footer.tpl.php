<?php
/**
 * @file: page footer partial.
 */

$secondary_nav = render($secondary_nav);
$secondary_navigation = render($page['secondary_navigation']);
?>

<?php if ($just_above_footer = render($page['justabovefooter'])): ?>
  <div class="just-above-footer-wrapper">
    <footer class="just-above-footer container">
      <?php print $just_above_footer ?>
    </footer>
  </div>
<?php endif ?>

<div class="footer-wrapper">
  <footer class="footer">

    <!-- Primary navigation, if available. -->
    <nav role="navigation">
      <?php if (isset($primary_nav_rendered) && !empty($primary_nav_rendered)): ?>
        <div class="row">
          <div class="container-fluid">
            <div class="col-md-3 col-xs-1"></div>
            <div class="col-md-6 col-xs-10">
              <?php print $primary_nav_rendered; ?>
            </div>
            <div class="col-md-3 col-xs-1"></div>
          </div>
        </div>
      <?php endif; ?>

      <?php if (isset($header_mainmenu) && !empty($header_mainmenu)): ?>
        <div class="row">
          <div class="container-fluid">
            <div class="col-md-3 col-xs-1"></div>
            <div class="col-md-6 col-xs-10">
              <?php print $header_mainmenu; ?>
            </div>
            <div class="col-md-3 col-xs-1"></div>
          </div>
        </div>
      <?php endif; ?>
    </nav>

    <!-- Secondary menu, if populated -->
    <?php if (!empty($secondary_nav) || !empty($secondary_navigation)): ?>
      <div class="row">
        <div class="container">
          <div class="col-xs-12">
            <?php print $secondary_nav ?>
            <?php print $secondary_navigation ?>
          </div>
        </div>
      </div>
    <?php endif; ?>

    <div class="row">
      <div class="container">
        <div class="col-xs-12">
          <?php print render($page['footer']) ?>

          <?php if (isset($logo) && !empty($logo)): ?>
            <a class="logo navbar-btn pull-right" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
              <img src="<?php print $logo; ?>" class="logo" alt="<?php print t('Home'); ?>"/>
            </a>
          <?php endif ?>
        </div>
      </div>
    </div>
  </footer>
</div>
