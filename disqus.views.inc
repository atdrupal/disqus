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
   * Before rendering the field, add the required JavaScript.
   */
  function pre_render($values) {
  	static $js_added = FALSE;
	if ($js_added === FALSE) {
      $domain = variable_get('disqus_domain', '');
	  if (!empty($domain)) {
        $disqus_js = theme('disqus_comments_js', $domain);
        drupal_add_js($disqus_js, 'inline', 'footer');
      }
	  $js_added = TRUE;
	}
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
    if (!empty($types[$node->type])) {
	  $text = !empty($this->options['text']) ? $this->options['text'] : t('Comments');
	  return l($text, "node/$node->nid", array('fragment' => 'disqus_thread'));
	}
  }
}
