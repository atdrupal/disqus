// $Id$

/**
 * Drupal ShareThis behaviors.
 */
Drupal.behaviors.disqus = function(context) {
  if (Drupal.settings.disqus) {
    jQuery('#disqus_thread:not(.disqus-processed)', context).addClass('disqus-processed').disqus(Drupal.settings.disqus);
  }
  if (Drupal.settings.disqusCommentDomain) {
    jQuery.disqusLinks(Drupal.settings.disqusCommentDomain);
  }
};
