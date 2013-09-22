<?php

namespace Drupal\disqus\Plugin\Block;

use Drupal\Component\Annotation\Plugin;
use Drupal\Core\Annotation\Translation;

/**
 *
 * @Plugin(
 *   id = "disqus_comments",
 *   admin_label = @Translation("Disqus: Comments"),
 *   module = "disqus"
 * )
 */
class DisqusCommentsBlock extends DisqusBaseBlock {
  /**
   * Overrides DisqusBaseBlock::settings().
   */
  public function settings() {
    return array(
      'cache' => DRUPAL_CACHE_CUSTOM,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    if (variable_get('disqus_location', 'content_area') == 'block' && user_access('view disqus comments')) {
      if ($object = menu_get_object()) {
        return $this->buildForNodeEntity($object);
      }

      if ($object = menu_get_object('user')) {
        return  $this->buildForUserEntity($object);
      }
    }
  }

  /**
   * Build the disqus comment block for node entity.
   */
  protected function buildForNodeEntity($object) {
      // For nodes, display if the Disqus object is enabled.
      if (isset($object->disqus) && $object->disqus['status']) {
        return array(
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

  /**
   * Build the disqus comment block for user entity.
   */
  protected function buildForUserEntity($object) {
    if (isset($object->disqus)) {
      return array(
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
