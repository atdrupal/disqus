<?php
// $Id$

/**
 * Implementation of hook_views_data().
 */
function disqus_views_data() {
  // Number of Disqus comments made on the given node.
  $data['node']['disqus_comments'] = array(
    'field' => array(
      'title' => t('Disqus Comments'),
      'help' => t('The number of Disqus comments made on the node.'),
      'handler' => 'views_handler_field_node_disqus_comments',
    ),
  );
  return $data;
}

/**
 * Field handler to present the number of Disqus comments on a node.
 */
class views_handler_field_node_disqus_comments extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }

  /**
   * When rendering the field.
   */
  function render($values) {
    // ensure Disqus comments are available on the node user has access to edit this node.
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->type = $values->{$this->aliases['type']};
    $types = variable_get('disqus_nodetypes', array());
    if (!empty($types[$node->type]) && user_access('view disqus comments')) {
  	  $text = !empty($this->options['text']) ? $this->options['text'] : t('Comments');
      $domain = variable_get('disqus_domain', '');
  	  return theme('disqus_comments_num', $domain, "node/$node->nid", $text);
  	}
  }
}
