/**
 * @file
 * We are mapping values from variables back to ScrollIt array.
 */

Drupal.behaviors.paragraphsNavJsScrollit = {
    attach: function (context, settings) {

        (function ($) {

            function nav_attributes(nav_info, current_id) {
                // Find navigation and paras only for one nav
                var navnum = $("div[class$='" + nav_info.delta + "'] a.ppn-item, " +
                    "div[class*='" + nav_info.delta + "'] a.ppn-item, " +
                    "div[id$='" + nav_info.delta + "'] a.ppn-item, " +
                    "div[id*='" + nav_info.delta + "'] a.ppn-item");
                var paranum = $("." + nav_info.paras_class + ".paragraphs-items .entity-paragraphs-item");


                // Start counting where we left of.
                nav_id = current_id;

                // We always have same number of navs and paras.
                for (nav_id = current_id; nav_id < (navnum.length + current_id); ++nav_id) {
                    $(navnum[(nav_id - current_id)]).attr('data-scroll-nav', nav_id);
                }
                for (nav_id = current_id; nav_id < (paranum.length + current_id); ++nav_id) {
                    $(paranum[(nav_id - current_id)]).attr('data-scroll-index', nav_id);
                }

                // We don't care if nav or paras or both are missing.
                if (navnum.length > paranum.length) {
                    return current_id + navnum.length;
                } else {
                    return current_id + paranum.length;
                }
            }

            var current_id = 0;
            // Attach attributes for each nav at a time not mixing up indexes.
            for (var key in settings.paragraphs_nav_scrollit.paragraphs_nav_scrollit_delta) {
                if (settings.paragraphs_nav_scrollit.paragraphs_nav_scrollit_delta.hasOwnProperty(key)) {
                    var single_nav = settings.paragraphs_nav_scrollit.paragraphs_nav_scrollit_delta[key];
                    current_id = nav_attributes(single_nav, current_id);
                }
            }

            $.scrollIt({
                upKey: settings.paragraphs_nav_scrollit.upKey,
                downKey: settings.paragraphs_nav_scrollit.downKey,
                easing: settings.paragraphs_nav_scrollit.easing,
                scrollTime: settings.paragraphs_nav_scrollit.scrollTime,
                activeClass: settings.paragraphs_nav_scrollit.activeClass,
                onPageChange: settings.paragraphs_nav_scrollit.onPageChange,
                topOffset: settings.paragraphs_nav_scrollit.topOffset
            });
        })(jQuery);
    }
};