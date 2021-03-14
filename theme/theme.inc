<?php
/**
 * @file
 * The file for Image segmentation solution pack. Includes preprocess functions
 */

/**
 * Prepares variables for image_segmentation_segment templates
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_image_segmentation_segment(array &$variables) {
  $object = $variables['object'];
  if (isset($object['OBJ'])) {
    $obj_url = url("islandora/object/{$object->id}/datastream/OBJ/view");
    $params = [
      'title' => $object->label,
      'path' => $obj_url,
    ];
    $variables['segment'] = theme('image', $params);
  }
}

/**
 * Prepares variables for image_segmentation_segment templates
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_image_segmentation_segment_list(array &$variables) {
  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  $object = $variables['object'];
  $segments = image_segmentation_get_segments($object);
  $variables['segments'] = [];
  foreach ($segments as $segment) {
    $variables['segments'][] = [
      'object' => $segment,
      'type' => image_segmentation_get_type($object),
      'link' => url("islandora/object/{$object->id}/"),
      'tn_url' => url("islandora/object/{$object->id}/datastream/TN/view"),
    ];
    $variables['content_types'] = image_segmentation_get_content_types();
  }

}

function template_preprocess_image_segmentation_manual_segmentation_results(&$variables) {
  $options = array(
    'group' => JS_THEME,
  );
  
  drupal_add_js(
    drupal_get_path('module', 'image_segmentation').'/js/image_segmentation_manual_segmentation.js', $options);
}