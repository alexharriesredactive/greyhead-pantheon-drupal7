<?php if ($above_navigation = render($page['above_navigation'])): ?>
  <header id="above-navigation" role="banner" class="above-navigation">
    <!--    <div class="container">-->
    <div class="row">
      <?php print $above_navigation; ?>
    </div>
    <!--    </div>-->
  </header>
<?php endif; ?>

<header id="navbar" role="banner" class="<?php print $navbar_classes; ?>">
  <div class="navbar-header">

    <!-- Header row 1 - site logo and primary menu -->
    <div class="navbar-row-1 clearfix">
      <div class="row">
        <div class="container-may-be-fluid">
          <div class="navbar-header">
            <div class="site-info">
              <div class="site-name-link-and-slogan">
                <?php if (!empty($site_name)): ?>
                  <h1 class="site-name-link">
                    <a class="name navbar-brand" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
                  </h1>
                <?php endif; ?>

                <?php if (!empty($site_slogan)): ?>
                  <p class="lead site-slogan">
                    <a href="<?php print $front_page; ?>"><?php print $site_slogan; ?></a>
                  </p>
                <?php endif; ?>
              </div>
            </div>

            <?php if (!empty($primary_nav) || !empty($page['navigation'])): ?>
              <!-- We create an empty span to hide/show the main menu. If JS is
                   enabled, we will then hide the main menu until the toggle is
                   clicked. No JS? No problem - we just won't hide it. -->
              <span id="toggle-header-menu"
                class="eleganticon toggler only-visible-with-js"
                data-state="closed"
                data-closed-class="icon_menu"
                data-open-class="icon_close_alt2"
                data-target="header-menu-collapsible"><span class="accessibility-label"><?php print t('Menu') ?></span></span>

              <div id="header-menu-collapsible" class="showhideable-target header-menu-container only-visible-without-js">
                <nav role="navigation">
                  <?php if ($primary_nav_rendered = render($primary_nav)): ?>
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

                  <?php if ($header_mainmenu = render($page['header_mainmenu'])): ?>
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
              </div>
            <?php endif; ?>

            <?php
            // If search is allowed, display a hideable/showable search form.
            if ($header_search_rendered = render($header_search)): ?>
              <!-- We create an empty span to hide/show the main menu. If JS is
                   enabled, we will then hide the main menu until the toggle is
                   clicked. No JS? No problem - we just won't hide it. -->
              <span id="toggle-header-search"
                class="eleganticon toggler only-visible-with-js toggler-close-top-right"
                data-state="closed"
                data-closed-class="icon_search"
                data-open-class="icon_close_alt2"
                data-target="header-search-collapsible"><span class="accessibility-label"><?php print t('Search') ?></span></span>

              <div id="header-search-collapsible" class="showhideable-target header-search-container only-visible-without-js">
                <div class="row">
                  <div class="container-fluid">
                    <div class="col-md-3 col-xs-1"></div>
                    <div class="col-md-6 col-xs-10">
                      <?php print $header_search_rendered ?>
                    </div>
                    <div class="col-md-3 col-xs-1"></div>
                  </div>
                </div>
              </div>

              <?php
              // Do we have content below the search block?
              if ($header_search = render($page['header_search'])): ?>
                <div class="row">
                  <div class="container-fluid">
                    <div class="col-md-3 col-xs-1"></div>
                    <div class="col-md-6 col-xs-10">
                      <?php print $header_search; ?>
                    </div>
                    <div class="col-md-3 col-xs-1"></div>
                  </div>
                </div>
              <?php endif; ?>
            <?php endif ?>
          </div>
        </div>
      </div>
    </div>
  </div>
</header>
