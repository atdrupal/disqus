<?php

namespace Drupal\disqus\Plugin\Block;

use Drupal\block\BlockBase;
use Drupal\Component\Annotation\Plugin;
use Drupal\Core\Annotation\Translation;

/**
 *
 * @Plugin(
 *   id = "disqus_top_commenters",
 *   admin_label = @Translation("Disqus: Top Commenters"),
 *   module = "disqus"
 * )
 */
class DisqusTopCommenters extends BlockBase {
  /**
   * Overrides \Drupal\block\BlockBase::settings().
   */
  public function settings() {
    return array(
      'cache' => DRUPAL_CACHE_GLOBAL,
    );
  }
}
