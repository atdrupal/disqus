<?php

/**
 * Implements hook_block_info().
 */
function disqus_block_info() {
  $blocks['disqus_comments'] = array(
    'info' => t('Disqus: Comments'),
    'cache' => DRUPAL_CACHE_CUSTOM,
  );
  return $blocks;
}

/**
 * Implements hook_block_configure().
 */
function disqus_block_configure($delta = '') {
  $form = array();
  $form['disqus'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disqus settings'),
  );
  if ($delta == 'disqus_comments') {
    $form['disqus']['#description'] = t('This block will be used to display the comments from Disqus when comments are applied to the given page. Visit the <a href="@disqussettings">Disqus settings</a> to configure when this is visible.', array('@disqussettings' => url('admin/config/services/disqus')));
  }
  $form['disqus'][$delta . '_items'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to show'),
    '#options' => array(1 => 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20),
    '#default_value' => variable_get($delta .'_items', 5),
    '#access' => ($delta != 'disqus_comments'),
  );
  $form['disqus'][$delta . '_showavatars'] = array(
    '#type' => 'select',
    '#title' => t('Show avatars'),
    '#options' => array(FALSE => t('No'), TRUE => t('Yes')),
    '#default_value' => variable_get($delta .'_showavatars', TRUE),
    '#access' => ($delta == 'disqus_recent_comments') || ($delta == 'disqus_top_commenters'),
  );
  $form['disqus'][$delta . '_avatarsize'] = array(
    '#type' => 'select',
    '#title' => t('Avatar size'),
    '#options' => array(
      24 => t('X-Small (24px)'),
      32 => t('Small (32px)'),
      48 => t('Medium (48px)'),
      92 => t('Large (92px)'),
      128 => t('X-Large (128px)'),
    ),
    '#default_value' => variable_get($delta .'_avatarsize', 32),
    '#access' => ($delta == 'disqus_recent_comments') || ($delta == 'disqus_top_commenters'),
  );
  $form['disqus'][$delta . '_colortheme'] = array(
    '#type' => 'select',
    '#title' => t('Color Theme'),
    '#options' => array(
      'blue' => t('Blue'),
      'grey' => t('Grey'),
      'green' => t('Green'),
      'red' => t('Red'),
      'orange' => t('Orange'),
    ),
    '#default_value' => variable_get($delta .'_colortheme', 'blue'),
    '#access' => $delta == 'disqus_combination_widget',
  );
  $form['disqus'][$delta . '_defaulttabview'] = array(
    '#type' => 'select',
    '#title' => t('Default Tab View'),
    '#options' => array(
      'people' => t('People'),
      'recent' => t('Recent'),
      'popular' => t('Popular'),
    ),
    '#default_value' => variable_get($delta .'_defaulttabview', 'people'),
    '#access' => $delta == 'disqus_combination_widget',
  );
  $form['disqus'][$delta . '_excerpt_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment Except Length'),
    '#default_value' => variable_get($delta .'_excerpt_length', '200'),
    '#access' => ($delta == 'disqus_recent_comments') || ($delta == 'disqus_combination_widget'),
    '#size' => 4,
  );
  $form['disqus'][$delta . '_hide_mods'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide moderators in ranking'),
    '#default_value' => variable_get($delta .'_hide_mods', FALSE),
    '#access' => ($delta == 'disqus_top_commenters') || ($delta == 'disqus_combination_widget'),
  );
  return $form;
}

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

/**
 * Implements hook_block_view().
 */
function disqus_block_view($delta = '') {
  $options = array(
    'num_items' => variable_get($delta . '_items', 5),
    'avatars' => variable_get($delta . '_showavatars', TRUE) ? array('avatar_size' => variable_get($delta . '_avatarsize', 32)) : array('hide_avatars=1'),
    'color' => variable_get($delta . '_colortheme', 'blue'),
    'default_tab' => variable_get($delta . '_defaulttabview', 'people'),
    'excerpt_length' => variable_get($delta . '_excerpt_length', '200'),
    'hide_mods' => variable_get($delta . '_hide_mods', FALSE) ? '1' : '0',
    'domain' => variable_get('disqus_domain', ''),
  );
  if (!empty($options['domain'])) {
    $subject = '';
    $content = '';
    switch ($delta) {
      case 'disqus_recent_comments':
        $subject = t('Recent Comments');
        $content = _disqus_block_content('recent_comments_widget', $options);
        break;
      case 'disqus_popular_threads':
        $subject = t('Popular Threads');
        $content = _disqus_block_content('popular_threads_widget', $options);
        break;
      case 'disqus_top_commenters':
        $subject = t('Top Commenters');
        $content = _disqus_block_content('top_commenters_widget', $options);
        break;
      case 'disqus_combination_widget':
        $subject = t('Comments');
        $content = _disqus_block_content('combination_widget', $options);
        break;
      case 'disqus_comments':
        if (variable_get('disqus_location', 'content_area') == 'block' && user_access('view disqus comments')) {
          if ($object = menu_get_object()) {
            // For nodes, display if the Disqus object is enabled.
            if (isset($object->disqus) && $object->disqus['status']) {
              $content = array(
                'disqus' => array(
                  '#type' => 'disqus',
                  '#disqus' => $object->disqus,
                ),
                '#cache' => array(
                  'bin' => 'cache_block',
                  'expire' => CACHE_TEMPORARY,
                  'keys' => array(
                    'disqus',
                    'disqus_comments',
                    'node',
                    (int) $object->nid,
                  ),
                ),
              );
            }
          }
          else if ($object = menu_get_object('user')) {
            if (isset($object->disqus)) {
              $content = array(
                'disqus' => array(
                  '#type' => 'disqus',
                  '#disqus' => $object->disqus,
                ),
                '#cache' => array(
                  'bin' => 'cache_block',
                  'expire' => CACHE_TEMPORARY,
                  'keys' => array(
                    'disqus',
                    'disqus_comments',
                    'user',
                    (int) $object->uid,
                  ),
                ),
              );
            }
          }
        }
        break;
    }

    return array('subject' => $subject, 'content' => $content);
  }
}

/**
 * Helper function for disqus widget blocks content.
 *
 * @param $function
 *   Name of the function (widget) that needs to be returned. Same as widget
 *   API call name (w/o .json suffix).
 * @param $options
 *   Options array (query variables, domain, ...).
 * @return
 *   Render array that can be directly used for block content.
 */
function _disqus_block_content($function, $options) {
  $configuration = array(
    'recent_comments_widget' => array(
      'id' => 'dsq-recentcomments',
      'query_items' => array('num_items', 'excerpt_length', 'avatars'),
    ),
    'popular_threads_widget' => array(
      'id' => 'dsq-popthreads',
      'query_items' => array('num_items'),
    ),
    'top_commenters_widget' => array(
      'id' => 'dsq-topcommenters',
      'query_items' => array('num_items', 'hide_mods', 'avatars'),
    ),
    'combination_widget' => array(
      'id' => 'dsq-combinationwidget',
      'query_items' => array('num_items', 'hide_mods', 'excerpt_length', 'color', 'default_tab'),
    ),
  );

  if (empty($configuration[$function])) {
    return FALSE;
  }

  $query = array();
  foreach ($configuration[$function]['query_items'] as $query_item) {
    if ($query_item == 'avatars') {
      $query += $options[$query_item];
    }
    else {
      $query[$query_item] = $options[$query_item];
    }
  }

  return array(
    'widget' => array(
      '#theme' => 'html_tag',
      '#tag' => 'script',
      '#value' => '',
      '#attributes' => array(
        'type' => 'text/javascript',
        'src' => url(
          "//disqus.com/forums/${options['domain']}/$function.js",
          array(
            'external' => TRUE,
            'query' => $query,
          )
        ),
      ),
    ),
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'id' => $configuration[$function]['id'],
      'class' => array('dsq-widget'),
    ),
  );
}
