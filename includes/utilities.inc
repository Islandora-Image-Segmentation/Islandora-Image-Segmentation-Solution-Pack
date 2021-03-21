<?php

use NewspaperNavigator\SegmentationClient;
use NewspaperNavigator\ExtractedSegment;

/**
 * Gets all the segments in the given newspaper page.
 *
 * @param AbstractObject $object
 *   The newspaper page to fetch the pages from.
 *
 * @return array
 *   All the segments in the given newspaper page.
 */
function image_segmentation_get_segments(AbstractObject $object) {

  $repository = $object->repository;
  $get_object = function ($id) use (&$repository) {
    return $repository->getObject($id);
  };
  return array_map($get_object, image_segmentation_get_segment_pids($object));
}

/**
 * Gets all the segments in the given newspaper page.
 *
 * @param AbstractObject $object
 *   The newspaper page to fetch the pages from.
 *
 * @return array
 *   An array of the PIDs of the segments in the page.
 */
function image_segmentation_get_segment_pids(AbstractObject $object) {
  $query = <<<EOQ
  PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
  SELECT ?pid
  FROM <#ri>
  WHERE {
    ?pid <islandora-rels-ext:isSegmentOf> <info:fedora/$object->id>
  }
EOQ;

  $results = $object->repository->ri->sparqlQuery($query);
  $map = function ($e) {
    return $e['pid']['value'];
  };
  return array_map($map, $results);
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
  $res = $object->relationships->get(ISLANDORA_RELS_INT_URI, 'hasImageCategory');
  if (!isset($res[0]['predicate']['value'])) {
    return 'Unknown';

  }
  return $res[0]['object']['value'];

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
 * Extracts segments from a newspaper page.
 *
 * @param \AbstractObject $object
 *  The page object that the segments will be extracted from.
 *
 * @return bool
 *   Returns TRUE on success and FALSE on failure.
 */
function image_segmentation_extract_segments(AbstractObject $object, bool $force = FALSE) {
  $repository = islandora_get_tuque_connection()->repository;
  $existing_segments = image_segmentation_get_segment_pids($object);
  if ($force) {
    foreach ($existing_segments as &$segment) {
      $repository->purgeObject($segment);
    }
  }
  elseif (count($existing_segments) > 0) {
    drupal_set_message("Segments already exist.", "error");
    return FALSE;
  }

  $api_host = variable_get("image_segmentation_api_host");
  $api_port = variable_get("image_segmentation_api_port");
  $api_key = variable_get("image_segmentation_api_key", 60.0);
  $api_timeout = variable_get("image_segmentation_timeout", 60.0);
  $client = new SegmentationClient("{$api_host}:{$api_port}/api/", $api_timeout, $api_key);

  $filename = image_segmentation_get_uploaded_file($object['OBJ']);
  $response = $client->segmentFile($filename);
  if ($response->status_code != 0) {
    drupal_set_message("Newspaper Navigator API error:\n{$response->error_message}", "error");
    return FALSE;
  }

  for ($i = 1; $i <= $response->segment_count; $i++) {
    $ingested = image_segmentation_ingest_segment($response->segments[$i - 1], $i, $object, $filename);
    if (!isset($ingested)) {
      drupal_set_message("Failed to ingest segment", "error");
      return FALSE;
    }
    drupal_set_message("Ingested <a href='/islandora/object/{$ingested->id}'>$ingested->id</a>");
  }
  drupal_unlink($filename);

  return TRUE;
}

/**
 * Fetches the upload image file from Fedora, and saves it to a temp location.
 *
 * @param AbstractDatastream $datastream
 *   The object to fetch the uploaded image file from.
 *
 * @return string
 *   The file path to the temp file if successful, FALSE otherwise.
 */
function image_segmentation_get_uploaded_file(AbstractDatastream $datastream) {
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($datastream->mimeType);
  $filename = file_create_filename(str_replace(':', '_', "{$datastream->parent->id}_{$datastream->id}.{$ext}"), 'temporary://');
  $datastream->getContent($filename);
  return $filename;
}


/**
 * Ingests an extracted segment
 *
 * @param \NewspaperNavigator\ExtractedSegment $extractedSegment
 *  THe segment to ingest
 * @param int $seq
 *   The sequence number of the segment
 * @param \AbstractObject $parent
 *   The parent of the extracted segment
 * @param string $file
 *   The file to crop the segment from
 *
 * @return \AbstractObject
 *  The extracted segment
 * @throws \ImagickException
 */
function image_segmentation_ingest_segment(ExtractedSegment &$extractedSegment, int $seq, AbstractObject $parent, string $file) {
    $id = sprintf("%s-%02d", $parent->id, $seq);
    $repository = $parent->repository;
    $segment = $repository->constructObject($id);
    $segment->label = $id;
    $segment->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $parent->id);
    $segment->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSegmentOf', $parent->id);
    $segment->relationships->add(ISLANDORA_RELS_INT_URI, 'hasImageCategory', $extractedSegment->classification);
    $segment->models = ['islandora:segmentedImageCModel'];

    $image = new Imagick($file);
    $cropped = \NewspaperNavigator\cropImage($image, $extractedSegment->bounding_box);
    $mime_detector = new MimeDetect();
    $ext = $mime_detector->getExtension($image->getImageMimeType());
    $cropped_file = file_create_filename("cropped.{$ext}", 'temporary://');
    $cropped->writeImage(drupal_realpath($cropped_file));

    $datastream = $segment->constructDatastream('OBJ', 'M');
    $datastream->label = 'OBJ';
    $datastream->mimetype = $cropped->getImageMimeType();
    $datastream->setContentFromFile($cropped_file);
    $segment->ingestDatastream($datastream);

    $ocr = $segment->constructDatastream('OCR', 'M');
    $ocr->label = 'OCR text';
    $ocr->mimetype = 'text/plain';
    $ocr->setContentFromString($extractedSegment->ocr_text);
    $segment->ingestDatastream($ocr);

    $hocr = $segment->constructDatastream('HOCR', 'M');
    $hocr->label = 'HOCR text';
    $hocr->mimetype = 'text/html';
    $hocr->setContentFromString($extractedSegment->hocr);
    $segment->ingestDatastream($hocr);

    drupal_unlink($cropped_file);
    return $repository->ingestObject($segment);
}