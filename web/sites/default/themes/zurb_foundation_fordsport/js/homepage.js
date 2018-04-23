// Adjust the height of the homepage's panels according to the window size,
// from http://stackoverflow.com/a/1187011 with thx :)
//jQuery.event.add(window, "load", resizeFrame);
//jQuery.event.add(window, "resize", resizeFrame);
//
//function resizeFrame() {
//  var h = $(window).height();
////  var w = $(window).width();
//  $('.page-panel').css('height', h * 0.8);
//  console.log('Win! :)');
//}

jQuery(document).ready(function() {
  resizePanels();
});

// for the window resize
jQuery(window).resize(function() {
  resizePanels();
});

function resizePanels() {
  var windowheight = jQuery(window).height();
  var panelHeight = windowheight * 0.8;
  jQuery(".page-panel").css('min-height', panelHeight);

  var welcomePanelHeight = jQuery('.page-panel.hp-welcome .page-panel-inner').height();
  var welcomePanelMargin = (panelHeight - welcomePanelHeight) / 2;

  if (welcomePanelMargin > 0) {
    jQuery('.page-panel.hp-welcome .page-panel-inner').css('margin-top', welcomePanelMargin);
    jQuery('.page-panel.hp-welcome .page-panel-inner').css('margin-bottom', welcomePanelMargin);
  }

  var eventsPanelHeight = jQuery('.page-panel.hp-events .page-panel-inner').height();
  var eventsPanelMargin = (panelHeight - eventsPanelHeight) / 2;

  if (eventsPanelMargin > 0) {
    jQuery('.page-panel.hp-events .page-panel-inner').css('margin-top', eventsPanelMargin);
    jQuery('.page-panel.hp-events .page-panel-inner').css('margin-bottom', eventsPanelMargin);
  }

  var galleriesPanelHeight = jQuery('.page-panel.hp-galleries .page-panel-inner').height();
  var galleriesPanelMargin = (panelHeight - galleriesPanelHeight) / 2;

  if (galleriesPanelMargin > 0) {
    jQuery('.page-panel.hp-galleries .page-panel-inner').css('margin-top', galleriesPanelMargin);
    jQuery('.page-panel.hp-galleries .page-panel-inner').css('margin-bottom', galleriesPanelMargin);
  }

  var contactPanelHeight = jQuery('.page-panel.hp-contact .page-panel-inner').height();
  var contactPanelMargin = (panelHeight - contactPanelHeight) / 2;

  if (contactPanelMargin > 0) {
    jQuery('.page-panel.hp-register .page-panel-inner').css('margin-top', contactPanelMargin);
    jQuery('.page-panel.hp-register .page-panel-inner').css('margin-bottom', contactPanelMargin);
  }

  var registerPanelHeight = jQuery('.page-panel.hp-register .page-panel-inner').height();
  var registerPanelMargin = (panelHeight - registerPanelHeight) / 2;

  if (registerPanelMargin > 0) {
    jQuery('.page-panel.hp-register .page-panel-inner').css('margin-top', registerPanelMargin);
    jQuery('.page-panel.hp-register .page-panel-inner').css('margin-bottom', registerPanelMargin);
  }
}
