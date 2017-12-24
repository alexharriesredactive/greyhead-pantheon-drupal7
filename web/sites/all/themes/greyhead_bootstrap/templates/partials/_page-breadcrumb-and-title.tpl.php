<?php
/**
 * @file: page breadcrumb and title partial.
 */
?>

<div class="row">
  <div class="container container-may-be-fluid">
    <div class="breadcrumbs"><?php print $breadcrumb ?></div>
  </div>
</div>

<div class="row">
  <div class="container container-may-be-fluid">
    <?php if (!empty($page['highlighted'])): ?>
      <div class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
    <?php endif; ?>
    <a id="main-content"></a>
    <?php print render($title_prefix); ?>
    <?php if (!empty($title)): ?>
      <h1 class="page-header">
          <span class="page-header-outer-inner-trevor">
            <span class="page-header-outer">
              <span class="page-header-inner">
                <span class="not-a-link"><?php print $title; ?></span>
              </span>
            </span>
          </span>
      </h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php print $messages; ?>
    <?php if (!empty($tabs)): ?>
      <?php print render($tabs); ?>
    <?php endif; ?>
    <?php if (!empty($page['help'])): ?>
      <?php print render($page['help']); ?>
    <?php endif; ?>
    <?php if (!empty($action_links)): ?>
      <ul class="action-links"><?php print render($action_links); ?></ul>
    <?php endif; ?>
  </div>
</div>
