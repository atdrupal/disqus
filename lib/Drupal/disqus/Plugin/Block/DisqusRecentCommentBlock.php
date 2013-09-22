<?php

namespace Drupal\disqus\Plugin\Block;

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
class DisqusRecentCommentBlock extends DisqusBaseBlock {
  /**
   * {@inheritdoc}
   */
  public function build() {
    return array(
      '#title' => t('Recent Comments'),
      $this->render('recent_comments_widget')
    );
  }
}
