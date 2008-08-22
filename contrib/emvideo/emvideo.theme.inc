<?php
// $Id$

/**
 * @file
 * This defines the various theme functions for Embedded Video Field (emvideo).
 */

/**
 * Formatter for emvideo_video_embed.
 * This will return the 'embed code', typically used to embed media in an
 * external site or blog.
 */
function theme_emvideo_video_embed($field, $item, $formatter, $node, $options = array()) {
  /*
    Note you can use this in node.tpl.php, substituting the proper field type:
    $field_type = 'field_video';
    $system_types = _content_type_info();
    $field = $system_types['fields'][$field_type];
    $field['widget'] = $system_types['content types'][$node->type]['fields'][$field_type]['widget'];
    print theme('emvideo_video_embed', $field, $node->{$field_type}[0], 'emvideo_embed', $node);

    or you can set $field to NULL and just use the options array
   */
  if ($item['value'] && $item['provider']) {
    $output = drupal_get_form('emvideo_embed_form', $field, $item, $formatter, $node, $options);
  }

  return $output;
}

/**
 * This will return a provided thumbnail image for a video.
 *
 * @param $field
 *   This is the field providing settings for the video thumbnail.
 * @param $item
 *   This is the data returned by the field. It requires at the least to be an array with 'value' and 'provider'.
 *   $item['value'] will be the video code, and $item['provider'] will be the provider, such as youtube.
 * @param $formatter
 *   This is the formatter for the view. This will nearly always be video_thumbnail.
 * @param $node
 *   This is the node object containing the field.
 * @param $no_link
 *   Optional. If FALSE, then we provide a link to the node.
 *   (In retrospect, this should have been $link, defaulting to TRUE.
 *   TODO: fix? problem though is that this goes deeper up the tree.)
 * @param $options
 *   Optional array. This is to pass optional overrides. currently:
 *     $options['width'] and $options['height'], if provided, will override any field settings for the thumbnail w/h.
 *     $options['link_url'], if provided, will cause the thumbnail link to go to another URL other than node/nid. $no_link must be FALSE.
 *     $options['link_title'], if provided, will set the title of the link when no image is provided. otherwise, it defaults to 'See video'.
 *     $options['image_title'], if provided, will set the title attribute of the href link, defaulting to $options['link_title'].
 *     $options['image_alt'], if provided, will set the alt attribute of the href link, defaulting to $options['link_title'].
 *     $options['thumbnail_url'], if provided, will completely override the thumbnail image entirely.
 */
function theme_emvideo_video_thumbnail($field, $item, $formatter, $node, $no_link = FALSE, $options = array()) {
  if ($item['value'] && $item['provider']) {
    // If we've set $options['thumbnail_url'], then we'll just use that.
    // Otherwise, if we have emthumb installed, then give it a chance to override our thumbnail
    $thumbnail_url = $options['thumbnail_url'] ? $options['thumbnail_url'] : module_invoke('emthumb', 'thumbnail_url', $item);

    // if we don't have a custom thumbnail, then see if the provider gives us a thumbnail
    $thumbnail_url = $thumbnail_url ? $thumbnail_url : module_invoke('emfield', 'include_invoke', 'emvideo', $item['provider'], 'thumbnail', $field, $item, $formatter, $node, $width, $height, $options);

    // If we still don't have a thumbnail, then apply a default thumbnail, if it exists.
    if (!$thumbnail_url) {
      $default_thumbnail_url = $field['widget']['thumbnail_default_path'] ? $field['widget']['thumbnail_default_path'] : variable_get('emvideo_default_thumbnail_path', NULL);

      if ($default_thumbnail_url) {
        $thumbnail_url = base_path() . $default_thumbnail_url;
      }
    }
  }
  else {
    // Seems to be an unknown video. Apply a default thumbnail, if it exists.
    if (!$thumbnail_url) {
      $default_thumbnail_url = $field['widget']['thumbnail_default_path'] ? $field['widget']['thumbnail_default_path'] : variable_get('emvideo_default_thumbnail_path', NULL);
      if ($default_thumbnail_url) {
        $thumbnail_url = base_path() . $default_thumbnail_url;
      }
    }
  }

  $link_url = isset($options['link_url']) ? $options['link_url'] : 'node/'. $node->nid;
  $link_title = isset($options['link_title']) ? $options['link_title'] : t('See video');
  $image_title = isset($options['image_title']) ? $options['image_title'] : $link_title;
  $image_alt = isset($options['image_alt']) ? $options['image_alt'] : $link_title;

  if ($thumbnail_url) {
    $width = isset($options['width']) ? $options['width'] : NULL;
    $width = isset($width) ? $width : ($field['widget']['thumbnail_width'] ? $field['widget']['thumbnail_width'] : variable_get('emvideo_default_thumbnail_width', EMVIDEO_DEFAULT_THUMBNAIL_WIDTH));
    $height = isset($options['height']) ? $options['height'] : NULL;
    $height = isset($height) ? $height : ($field['widget']['thumbnail_height'] ? $field['widget']['thumbnail_height'] : variable_get('emvideo_default_thumbnail_height', EMVIDEO_DEFAULT_THUMBNAIL_HEIGHT));
    if ($no_link) { //thickbox requires the thumbnail returned without the link
      $output = '<img src="'. $thumbnail_url .'" width="'. $width  .'" height="'. $height  .'" alt="'. $image_alt .'" title="'. $image_title .'" />';
    }
    else {
      $output = l('<img src="'. $thumbnail_url .'" width="'. $width  .'" height="'. $height  .'" alt="'. $image_alt .'" title="'. $image_title .'" />', $link_url, array('html'=> true));
    }
  }
  else {
     // if all else fails, then just print a 'see video' link.
    if ($no_link) {
      $output = ''; //thickbox won't work without a thumbnail
    }
    else {
      $output = l($link_title, $link_url);
    }
  }

  return $output;
}

function theme_emvideo_video_video($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $embed = $item['value'];
    $width = $options['width'] ? $options['width'] : ($field['widget']['video_width'] ? $field['widget']['video_width'] : variable_get('emvideo_default_video_width', EMVIDEO_DEFAULT_VIDEO_WIDTH));
    $height = $options['height'] ? $options['height'] : ($field['widget']['video_height'] ? $field['widget']['video_height'] : variable_get('emvideo_default_video_height', EMVIDEO_DEFAULT_VIDEO_HEIGHT));
    $autoplay = $options['autoplay'] ? $options['autoplay'] : $field['widget']['video_autoplay'];
    $output = module_invoke('emfield', 'include_invoke', 'emvideo', $item['provider'], 'video', $embed, $width, $height, $field, $item, $autoplay, $options);
  }

  return $output;
}

function theme_emvideo_default($field, $item, $formatter, $node, $options = array()) {
  return theme('emvideo_video_video', $field, $item, $formatter, $node, $options);
}

function theme_emvideo_video_preview($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $embed = $item['value'];
    $width = $options['width'] ? $options['width'] : ($field['widget']['preview_width'] ? $field['widget']['preview_width'] : variable_get('emvideo_default_preview_width', EMVIDEO_DEFAULT_PREVIEW_WIDTH));
    $height = $options['height'] ? $options['height'] : ($field['widget']['preview_height'] ? $field['widget']['preview_height'] : variable_get('emvideo_default_preview_height', EMVIDEO_DEFAULT_PREVIEW_HEIGHT));
    $autoplay = $options['autoplay'] ? $options['autoplay'] : $field['widget']['preview_autoplay'];
    $output = module_invoke('emfield', 'include_invoke', 'emvideo', $item['provider'], 'preview', $embed, $width, $height, $field, $item, $autoplay, $options);
  }

  return $output;
}

function theme_emvideo_thickbox($field, $item, $formatter, $node, $options = array()) {
  $thumbnail = theme('emvideo_video_thumbnail', $field, $item, 'video_thumbnail', $node, TRUE, $options);

  $destination = 'emvideo/thickbox/'. $node->nid .'/'. $field['widget']['video_width'] .'/'. $field['widget']['video_height'] .'/'. $field['field_name'];
  $options = array(
    'attributes' => array(
      'title' => $title,
      'class' => 'thickbox',
      'rel' => $field['type_name'],
    ),
    'query' => NULL,
    'fragment' => NULL,
    'absolute' => FALSE,
    'html' => TRUE,
  );
  $output = l($thumbnail, $destination, $options);

  return $output;
}

function theme_emvideo_formatter_video_video($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emvideo', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emvideo_formatter_video_embed($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emvideo', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emvideo_formatter_default($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emvideo', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emvideo_formatter_video_preview($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emvideo', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emvideo_formatter_video_thumbnail($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emvideo', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emvideo_formatter_video_flash($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emvideo', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}
function theme_emvideo_formatter_thickbox($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emvideo', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}