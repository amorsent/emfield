<?php
// $Id$

define('EMVIDEO_SEVENLOAD_MAIN_URL', 'http://www.sevenload.com/');

/**0
 * hook emvideo_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 */
function emvideo_sevenload_info() {
  $name = t('Sevenload');
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), ''),
  );
  return array(
    'provider' => 'sevenload',
    'name' => $name,
    'url' => EMVIDEO_SEVENLOAD_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !provider.', array('!provider' => l($name, EMVIDEO_SEVENLOAD_MAIN_URL, array('target' => '_blank')))),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['youtube'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_sevenload_settings() {
  $form = array();
  return $form;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $embed
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_sevenload_extract($embed = '') {
  // src="http://de.sevenload.com/pl/zuh5KMt"
  // http://de.sevenload.com/videos/zuh5KMt/Die-Zeit-rennt
  // hint: de.sevenload.com can also be en.sevenload.com and probably other languages later
  return array(
    '@sevenload\.com/pl/([^"]*)"@i',
    '@sevenload\.com/videos/(.*)/(.*)@i',
  );
}

/**
 * hook emvideo_PROVIDER_video_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_code
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_sevenload_video_link($video_code) {
  return 'http://www.sevenload.com/videos/'. $video_code;
}

function emvideo_sevenload_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return '';
}

/**
 * the embedded flash displaying the Sevenload video
 */
function theme_emvideo_sevenload_flash($embed, $width, $height) {
  if ($embed) {
    $output .= "<object height=\"$height\" width=\"$width\"><param name=\"FlashVars\" value=\"slxml=en.sevenload.com\" /><param name=\"movie\" value=\"http://en.sevenload.com/pl/$embed/options/swf\" /><embed src=\"http://de.sevenload.com/pl/$embed/options/swf\" type=\"application/x-shockwave-flash\" width=\"$width\" height=\"$height\" FlashVars=\"slxml=en.sevenload.com\"></embed></object>";
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_sevenload_video($embed, $width, $height, $field, $item) {
  $output = theme('emvideo_sevenload_flash', $embed, $width, $height);
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_sevenload_preview($embed, $width, $height, $field, $item) {
  $output = theme('emvideo_sevenload_flash', $embed, $width, $height);
  return $output;
}
