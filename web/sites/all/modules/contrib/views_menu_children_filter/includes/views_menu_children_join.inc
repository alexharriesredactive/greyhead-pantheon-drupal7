<?php

class views_menu_children_join extends \views_join {
  public $prefixes = array();

  function build_join($select_query, $table, $view_query) {
    $condition = "";
    $values = array();
    for($i = 0; $i < count($this->prefixes); $i++) {
      $condition .= "CONCAT(:prefix_$i, node.nid) = menu_links.link_path";
      if($i < (count($this->prefixes) -1)) {
        $condition .= " OR ";
      }
      $values[":prefix_$i"] = $this->prefixes[$i];
    }

    $select_query->addJoin("INNER", "menu_links", "menu_links", $condition, $values);
  }
}