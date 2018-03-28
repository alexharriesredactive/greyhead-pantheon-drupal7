<?php

/**
 * Alter the prefix values when used to join onto the link_path column in the menu_links table.
 * @param array $prefixes The path prefix that is used to join onto table menu_links.
 * @param string $menu_name The menu name to filter on.
 * @param \views_plugin_query $query The query before the join is applied.
 */
function hook_views_menu_children_filter_join_alter(&$prefixes, &$menu_name, \views_plugin_query &$query) {
  // The default is "node/".
}
