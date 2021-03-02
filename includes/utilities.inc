<?php

/**
 * Gets all the segments in the given paged content object.
 *
 * @param AbstractObject $object
 *   The newspaper page to fetch the pages from.
 *
 * @return array
 *   All the segments in the given newspaper page. Ordered by sequence
 *   number. Each an array containing info.
 */
function image_segmentation_get_segments(AbstractObject $object) {
  return [$object, $object, $object];
}

/**
 * Gets the type of content in the image object
 *
 * @param AbstractObject $object
 *   The image segment object to get the content type of.
 *
 * @return string
 *    The type of the image segment.
 */
function image_segmentation_get_type(AbstractObject $object) {
  return 'Advertisement';
}

function image_segmentation_get_content_types() {
  return [
    'illustration',
    'photograph',
    'comics/cartoon',
    'editorial_cartoon',
    'map',
    'headline',
    'ad',
  ];
}