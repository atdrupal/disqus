<?php

namespace Drupal\disqus\Plugin\Block;

use Drupal\block\BlockBase;

abstract class DisqusBaseBlock extends BlockBase {
  /**
   * Overrides \Drupal\block\BlockBase::settings().
   */
  public function settings() {
    return array(
      'cache' => DRUPAL_CACHE_GLOBAL,
    );
  }

  protected function getOptions() {
    return array(
      'num_items' => $this->configuration($delta . '_items', 5),
      'avatars' => $this->configuration($delta . '_showavatars', TRUE) ? array('avatar_size' => $this->configuration($delta . '_avatarsize', 32)) : array('hide_avatars=1'),
      'color' => $this->configuration($delta . '_colortheme', 'blue'),
      'default_tab' => $this->configuration($delta . '_defaulttabview', 'people'),
      'excerpt_length' => $this->configuration($delta . '_excerpt_length', '200'),
      'hide_mods' => $this->configuration($delta . '_hide_mods', FALSE) ? '1' : '0',
      'domain' => $this->configuration('disqus_domain', ''),
    );
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
  function render($function, $options) {
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
}
