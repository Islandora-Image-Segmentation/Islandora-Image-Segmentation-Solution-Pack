<?php
/**
 * @file
 * The file for Image segmentation solution pack. Includes preprocess functions
 */

/**
 * Prepares variables for image_segmentation_segment templates
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_image_segmentation_segment(array &$variables)
{
    $object = $variables['object'];
    if (isset($object['OBJ'])) {
        $obj_url = url("islandora/object/{$object->id}/datastream/OBJ/view");
        $params = array(
            'title' => $object->label,
            'path' => $obj_url,
        );
        $variables['segment'] = theme('image', $params);
    }
}