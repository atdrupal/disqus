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
  $form['disqus_userapikey'] = array(
    '#type' => 'textfield',
    '#title' => t('User API Key'),
    '#description' => t('The API key of the administrator account on Disqus. You can get yours <a href="@key">here</a>.', array('@key' => 'http://disqus.com/api/get_my_key/')),
    '#default_value' => variable_get('disqus_userapikey', ''),
  );
  $form['visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );
  $form['visibility']['disqus_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#description' => t('Apply comments to only the following node types.'),
    '#default_value' => variable_get('disqus_nodetypes', array()),
    '#options' => node_get_types('names'),
  );
  $form['visibility']['disqus_location'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#description' => t('Display the Disqus comments in the given location. When "Block" is selected, the comments will appear in the <a href="@disquscomments">Disqus Comments block</a>.', array('@disquscomments' => url('admin/build/block'))),
    '#default_value' => variable_get('disqus_location', 'content_area'),
    '#options' => array(
      'content_area' => t('Content Area'),
      'block' => t('Block'),
    ),
  );
  $form['disqus_behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['disqus_behavior']['disqus_developer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Testing'),
    '#description' => t('When enabled, uses the <a href="http://disqus.com/help/#faq-14">disqus_developer</a> flag to tell Disqus that you are in a testing environment. Threads will not display on the public community page with this set.'),
    '#default_value' => variable_get('disqus_developer', FALSE),
  );
  return system_settings_form($form);
}
