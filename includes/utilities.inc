<?php

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