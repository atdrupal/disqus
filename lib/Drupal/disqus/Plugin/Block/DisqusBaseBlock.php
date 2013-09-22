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
}
