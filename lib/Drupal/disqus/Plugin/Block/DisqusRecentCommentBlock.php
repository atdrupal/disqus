<?php

namespace Drupal\disqus\Plugin\Block;

use Drupal\block\BlockBase;
use Drupal\Component\Annotation\Plugin;
use Drupal\Core\Annotation\Translation;

/**
 *
 * @Plugin(
 *   id = "disqus_recent_comments",
 *   admin_label = @Translation("Disqus: Recent Comments"),
 *   module = "disqus"
 * )
 */
class DisqusRecentCommentBlock extends BlockBase {
  /**
   * Overrides \Drupal\block\BlockBase::settings().
   */
  public function settings() {
    return array(
      'cache' => DRUPAL_CACHE_GLOBAL,
    );
  }
}
