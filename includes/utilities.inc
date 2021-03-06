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

/**
 * Checks if the given object can be segmented.
 *
 * @param \AbstractObject $object
 *   The object to check.
 *
 * @return bool
 *   TRUE if the datastream can be derived, FALSE otherwise.
 */
function image_segmentation_can_segment(AbstractObject $object) {
  $to_derive = [
    'islandora:newspaperPageCModel',
  ];
  return array_intersect($to_derive, $object->models) && isset($object['OBJ']);
}


/**
 * Extracts segments from a newspaper page
 *
 * @param \AbstractObject $object
 *  The page object that the segments will be extracted from.
 *
 * @return bool
 *   Returns TRUE on success and FALSE on failure.
 */
function image_segmentation_extract_segments(AbstractObject $object) {
  $repository = islandora_get_tuque_connection()->repository;

  $segment = $repository->constructObject("{$object->id}-01");
  $segment->label = "{$object->id}-01";
  $segment->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $object->id);
  $segment->relationships->add(FEDORA_RELS_EXT_URI, 'isSegmentOf', $object->id);
  $segment->models = ['islandora:segmentedImageCModel'];

  $datastream = $segment->constructDatastream('OBJ', 'M');
  $datastream->label = 'OBJ';
  $datastream->mimetype = $object['OBJ']->mimetype;
  $datastream->content = $object['OBJ']->content;
  $segment->ingestDatastream($datastream);

  $ocr = $segment->constructDatastream('OCR', 'M');
  $ocr->label = 'OCR text';
  $ocr->mimetype = 'text/plain';
  $ocr->content = $object['OCR']->content;
  $segment->ingestDatastream($ocr);

  $repository->ingestObject($segment);
  return TRUE && $segment;
}
