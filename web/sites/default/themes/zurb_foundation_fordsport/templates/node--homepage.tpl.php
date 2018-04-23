<?php

/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see     template_preprocess()
 * @see     template_preprocess_node()
 * @see     template_process()
 *
 * @ingroup themeable
 */

/*
So, how do we do this homepage then?

- Welcome and about us section
  Preprocess node:
  - Get the "Welcome" heading as "Welcome to [site name]" - $hp_welcome_site_name
  - Get the "Welcome" subheading as "[slogan]" - $hp_welcome_slogan
  - Get background image(s), randomly choose one, and set it as the BG for
    #homepage-[nid]-welcome
- Events section
  Preprocess node:
  - Get background image(s), randomly choose one, and set it as the BG for
    #homepage-[nid]-events
- Photo galleries section
- Contact us form

 */
?>

<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <!-- Welcome text and club logo -->
  <div id="homepage-<?php print $node->nid ?>-welcome" class="page-panel page-panel-with-bg-image hp-welcome">
    <div class="page-panel-inner">
      <div class="hp-welcome-logo"></div>
      <div class="page-panel-inner-inner">
        <div class="hp-welcome-site-name-and-slogan">
          <h1 class="hp-welcome-site-name"><?php print $hp_welcome_site_name ?></h1>

          <h3 class="hp-welcome-slogan"><?php print $hp_welcome_slogan ?></h3>

          <div class="hp-welcome-text">
            <?php print drupal_render($content['field_hp_welcome_text'][0]) ?>
          </div>

          <div class="hp-welcome-ctas">
            <?php print drupal_render($content['field_hp_welcome_ctas']) ?>
          </div>
        </div>
      </div>
    </div>
  </div>
  <!-- /Welcome text and club logo -->

  <!-- Events listings -->
  <div id="homepage-<?php print $node->nid ?>-events" class="page-panel page-panel-with-bg-image page-panel-fixed-background hp-events">
    <div class="page-panel-inner">
      <div class="page-panel-inner-inner">
        <h2><?php print l(t('We attend car events across the UK...'), 'events') ?></h2>

        <div class="row">
          <!--        <div class="columns large-1">&nbsp;</div>-->
          <div class="columns large-6">
            <h3><?php print t('Events coming up') ?></h3>
            <?php print $events_coming_up ?>
          </div>
          <!--        <div class="columns large-2">&nbsp;</div>-->
          <div class="columns large-6">
            <h3><?php print t('Recent events') ?></h3>
            <?php print $recent_events ?>
          </div>
          <!--        <div class="columns large-1">&nbsp;</div>-->
        </div>
        <div class="row">
          <div class="columns large-6">
            <div class="more-link">
              <a class="small button radius secondary" href="/events"><?php print t('More events coming up') ?></a>
            </div>
          </div>
          <div class="columns large-6">
            <div class="more-link">
              <a class="small button radius secondary" href="/events/recent"><?php print t('More recent events') ?></a>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
  <!-- /Events listings -->

  <!-- Photo galleries -->
  <div id="homepage-<?php print $node->nid ?>-galleries" class="page-panel page-panel-no-bg-image hp-galleries">
    <div class="page-panel-inner">
      <div class="page-panel-inner-inner">
        <h2><?php print l(t('Photo galleries'), 'galleries') ?></h2>
        <?php print $photo_galleries ?>
        <div class="more-link">
          <?php print t('You can') ?> <a class="small button radius secondary" href="/galleries"><?php print t('view more photo galleries this way &raquo;') ?></a>
        </div>
      </div>
    </div>
  </div>
  <!-- /Photo galleries -->

  <!-- Register -->
  <div id="homepage-<?php print $node->nid ?>-register" class="page-panel page-panel-fixed-background hp-register">
    <div class="page-panel-inner">
      <div class="page-panel-inner-inner">
        <?php print render($content['field_hp_register_node']) ?>
      </div>
    </div>
  </div>
  <!-- /Register -->

  <!-- Contact us -->
  <div id="homepage-<?php print $node->nid ?>-contact" class="page-panel page-panel-no-bg-image hp-contact">
    <div class="page-panel-inner">
      <div class="page-panel-inner-inner">
        <?php print render($content['field_hp_contact_form']) ?>
      </div>
    </div>
  </div>
  <!-- /Contact us -->


</div>
