<?php
// $Id$

/**
 * Menu callback; Displays the administration settings for Disqus.
 */
function disqus_admin_settings() {
  $form = array();
  $form['disqus_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#description' => t('The domain you registered Disqus with. If you registered http://example.disqus.com, you would enter "example" here.'),
    '#default_value' => variable_get('disqus_domain', ''),
  );
  $form['disqus_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#description' => t('Apply comments to only the following node types.'),
    '#default_value' => variable_get('disqus_nodetypes', array()),
    '#options' => node_get_types('names'),
  );
  $form['disqus_developer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Testing'),
    '#description' => t('When enabled, uses the <a href="http://disqus.com/help/#faq-14">disqus_developer</a> flag to tell Disqus that you are in a testing environment. Threads will not display on the public community page with this set.'),
    '#default_value' => variable_get('disqus_developer', FALSE),
  );
  return system_settings_form($form);
}
