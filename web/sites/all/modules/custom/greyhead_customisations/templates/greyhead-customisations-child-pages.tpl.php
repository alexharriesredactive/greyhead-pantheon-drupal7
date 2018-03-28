<?php
/**
 * @file
 * Default implementation to print out a child pages listing.
 */
?>

<?php
// We keep the HTML lean here because we add a number of class-heavy
// wrappers in the theme.tpl.php file instead. ?>

<!--<div class="child-pages">-->
<!--  <div class="child-pages-content">-->
    <?php foreach ($child_pages_list as $child_page): ?>
      <?php print theme('greyhead_customisations_child_page', array('child_page' => $child_page)) ?>
    <?php endforeach ?>
<!--  </div>-->
<!--</div>-->
