<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) {return;}
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/<?php print $variables['lang']; ?>/all.js#xfbml=1";
  fjs.parentNode.insertBefore(js, fjs);
}(document, "script", "facebook-jssdk"));</script>
<div class="fb-comments <?php print $variables['class']; ?>" data-href="<?php print $variables['url']; ?>" data-num-posts="<?php print $variables['amount']; ?>" data-width="<?php print $variables['width']; ?>" data-colorscheme="<?php print $variables['style']; ?>"></div>
