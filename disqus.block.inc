<?php

/**
 * Implements hook_block_save().
 */
function disqus_block_save($delta = '', $edit = array()) {
  // The Disqus comments block doesn't have any configuration.
  if ($delta != 'disqus_comments') {
    variable_set($delta . '_items', $edit[$delta . '_items']);
    // Recent comments and top commenters have avatars.
    if (($delta == 'disqus_recent_comments') || ($delta == 'disqus_top_commenters')) {
      variable_set($delta . '_showavatars', $edit[$delta . '_showavatars']);
      variable_set($delta . '_avatarsize', $edit[$delta . '_avatarsize']);
    }

    // The excerpt length is only available for recent comments and combination.
    if (($delta == 'disqus_recent_comments') || ($delta == 'disqus_combination_widget')) {
      variable_set($delta . '_excerpt_length', $edit[$delta . '_excerpt_length']);
    }

    // Combination widget has the color theme and the default tab view.
    if ($delta == 'disqus_combination_widget') {
      variable_set($delta . '_colortheme', $edit[$delta . '_colortheme']);
      variable_set($delta . '_defaulttabview', $edit[$delta . '_defaulttabview']);
    }

    // Hide moderators appears in top commenters and combination widget.
    if (($delta == 'disqus_top_commenters') || ($delta == 'disqus_combination_widget')) {
      variable_set($delta . '_hide_mods', $edit[$delta . '_hide_mods']);
    }
  }
}
