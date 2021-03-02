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
    $dsid = '';
    if (isset($object['JP2'])) {
      $dsid = 'JP2';
    }
    elseif (strtolower($object['OBJ']->mimetype) == 'image/jp2') {
      $dsid = 'OBJ';
    }

    $params = [];
    if (isset($object[$dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$dsid])) {
      module_load_include('inc', 'islandora', 'includes/authtokens');
      $token = islandora_get_object_token($object->id, $dsid, 2);
      $params['token'] = $token;
      $params['pid'] = $object->id;
      $params['dsid'] = $dsid;

      module_load_include('inc', 'islandora', 'includes/solution_packs');
      $viewer_callback = islandora_get_viewer_callback('islandora_openseadragon');
      if (function_exists($viewer_callback)) {
        $viewer = $viewer_callback($params, $variables['object']);
        if ($viewer) {
          $variables['segment'] = $viewer;
        }
      }
    }
    // Fallback to image.
    else {
      $dsid = '';
      if (isset($object['JPG'])) {
        $dsid = 'JPG';
      }
      elseif (strtolower($object['OBJ']->mimetype) == 'image/jpeg') {
        $dsid = 'OBJ';
      }
      if
      (isset($object[$dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$dsid])) {
        $params = [
          'title' => $object->label,
          'path' => url("islandora/object/{$object->id}/datastream/{$dsid}/view"),
        ];
        $variables['segment'] = theme('image', $params);
      }
    }
  }
}