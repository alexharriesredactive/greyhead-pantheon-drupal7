<?php
/**
 * @file
 * paragraphs-item--customisable-content-listing.tpl.php
 */

// Danger: horrible hacks ahead - the following code really ought to be in a
// preprocess function. @TODO: fix this awfulness :)

// Get the view ID.
$view_name = 'paragraphs_list_of_content';

// Get the view display.
$view_display = $content['field_parapg_sort']['#items'][0]['value'];

// Filter by which content type(s) - the view accepts a + sign to concatenate
// multiple items together.
$content_types = [];

// Set the count.
$count = intval($content['field_parapg_count']['#items'][0]['value']);

/**
 * Arguments.
 */
$arguments = [];

// Get the content types argument to pass into the view.
$arguments['content_types_argument'] = !empty($content['field_parapg_node_type']['#items'][0]['safe_value']) ? trim($content['field_parapg_node_type']['#items'][0]['safe_value']) : NULL;

// Get a tag, if specified.
$arguments['tag'] = !empty($content['field_parapg_contentlist_tags']['#items'][0]['safe_value']) ? trim($content['field_parapg_contentlist_tags']['#items'][0]['safe_value']) : NULL;

// Replace empty arguments with "all".
foreach ($arguments as &$argument) {
  if (empty($argument)) {
    $argument = 'all';
  }
}

// Are we showing a pager? Make sure we don't allow crufty data from old
// fields to cause us strange errors.
$pager_types = ['full', 'mini', 'none'];
$pager_type = $content['field_parapg_pager']['#items'][0]['value'];
$pager_type = in_array($pager_type, $pager_types) ? $pager_type : 'mini';

// Load view name and display.
$view_id = drupal_html_id('contentlist--' . $view_display . '--' . $arguments['content_types_argument']) . '--' . $arguments['tag'];

if (!empty($view_name) && !empty($view_display)) {
  // Check if we have a view.
  if ($view = views_get_view($view_name)) {
    $view->set_display($view_display);

    // Check if user has access.
    if ($view->access($view_display)) {
      // Update the pager options.
      $view->display_handler->set_option('pager', [
        'type' => $pager_type,
        'options' => [
          'items_per_page' => $count,
        ],
      ]);

      // Load argument values and pass the to the view.
      $args = array_values($arguments); //[$content_types_argument, $tag];
      $view->set_arguments($args);
      $view->preview($view_display, $args);

      $output = $view->render();
    }
    else {
      $output = '<!-- Paragraphs Customisable Content listing: user does not have access to this view. -->';
    }
  }
  else {
    $output = '<!-- Paragraphs Customisable Content listing: the View "' . $view_name . '" and display "' . $view_display . '" could not be loaded. -->';
  }
}
else {
  $output = '<!-- Paragraphs Customisable Content listing: either the View "' . $view_name . '" or display "' . $view_display . '" were not set. -->';
}
?>

<div id="<?php print $view_id ?>" class="paragraphs-item paragraphs-item-update-pager-links paragraphs-item--customisable-content-listing">
  <div class="row">
    <div class="container">
      <div class="col-md-12">
        <?php print $output ?>
      </div>
    </div>
  </div>
</div>
