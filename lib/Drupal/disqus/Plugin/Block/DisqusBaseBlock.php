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

  /**
   * Overrides \Drupal\block\BlockBase::blockForm().
   */
  public function blockForm($form, &$form_state) {
    return $this->_blockForm($form, $form_state, $this->id);
  }

  /**
   * Overrides \Drupal\block\BlockBase::blockSubmit().
   */
  public function blockSubmit($form, &$form_state) {
    foreach ($form_state['values']['disqus'] as $k => $v) {
      $this->configuration[$k] = $v;
    }
  }

  /**
   * Helper for blockForm() method.
   */
  public function _blockForm($form, &$form_state, $delta) {
    $form['disqus'] = array(
      '#type' => 'fieldset',
      '#title' => t('Disqus settings'),
      '#tree' => TRUE,
    );

    if ($delta == 'disqus_comments') {
      $form['disqus']['#description'] = t('This block will be used to display the comments from Disqus when comments are applied to the given page. Visit the <a href="@disqussettings">Disqus settings</a> to configure when this is visible.', array('@disqussettings' => url('admin/config/services/disqus')));
    }

    $form['disqus'][$delta . '_items'] = array(
      '#type' => 'select',
      '#title' => t('Number of items to show'),
      '#options' => array(1 => 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20),
      '#default_value' => $this->configuration($delta .'_items', 5),
      '#access' => ($delta != 'disqus_comments'),
    );
    $form['disqus'][$delta . '_showavatars'] = array(
      '#type' => 'select',
      '#title' => t('Show avatars'),
      '#options' => array(FALSE => t('No'), TRUE => t('Yes')),
      '#default_value' => $this->configuration($delta .'_showavatars', TRUE),
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
      '#default_value' => $this->configuration($delta .'_avatarsize', 32),
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
      '#default_value' => $this->configuration($delta .'_colortheme', 'blue'),
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
      '#default_value' => $this->configuration($delta .'_defaulttabview', 'people'),
      '#access' => $delta == 'disqus_combination_widget',
    );
    $form['disqus'][$delta . '_excerpt_length'] = array(
      '#type' => 'textfield',
      '#title' => t('Comment Except Length'),
      '#default_value' => $this->configuration($delta .'_excerpt_length', '200'),
      '#access' => ($delta == 'disqus_recent_comments') || ($delta == 'disqus_combination_widget'),
      '#size' => 4,
    );
    $form['disqus'][$delta . '_hide_mods'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide moderators in ranking'),
      '#default_value' => $this->configuration($delta .'_hide_mods', FALSE),
      '#access' => ($delta == 'disqus_top_commenters') || ($delta == 'disqus_combination_widget'),
    );
    return $form;
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
