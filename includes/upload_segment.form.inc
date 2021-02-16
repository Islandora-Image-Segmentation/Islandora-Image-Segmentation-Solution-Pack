<?php

/**
 * @file
 * Handles the uploading of extracted segments as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the segmented image file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function image_segmentation_upload_segment_form(array $form, array &$form_state){
    $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
    $extensions = array('tiff tif jp2 jpg jpeg');
    $form = array();
    $form['file'] = array(
        '#title' => t('Segmented image file'),
        '#type' => 'managed_file',
        '#required' => TRUE,
        '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
        '#default_value' => isset($form_state['values']['file']) ? $form_state['values']['file'] : NULL,
        '#upload_location' => 'temporary://',
        '#upload_validators' => array(
            'file_validate_extensions' => $extensions,
        ),
    );

    $form['ocr'] = array(
        '#title' => t('Image text'),
        '#type' => 'textarea',
        '#required' => FALSE,
        '#description' => 'The text in the segment',
        '#default_valie' => isset($form_state['values']['ocr']) ? $form_state['values']['ocr'] : Null,
    );

    return $form;
}

/**
 * Adds the uploaded file into the ingestable objects 'OBJ' datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function image_segmentation_upload_segment_form_submit(array $form, array &$form_state){
    $object = $form_state['islandora']['objects'][0];
    // Create OBJ datastream
    if (empty($object['OBJ'])) {
        $ds = $object->constructDatastream('OBJ', 'M');
        $object->ingestDatastream($ds);
    }
    else {
        $ds = $object['OBJ'];
    }
    $obj_file = file_load($form_state['values']['file']);
    $obj_path = drupal_realpath($obj_file->uri);
    $ds->setContentFromFile($obj_path, FALSE);
    $ds->label = $obj_file->filename;
    $ds->mimetype = $obj_file->filemime;
    // Create OCR datastream
    if (isset($form_state['values']['ocr']) && empty($object['OCR'])){
        $ds = $object->constructDatastream('OCR', 'M');
        $ds->label = 'OCR text';
        $ds->mimetype = 'text/plain';
        $ds->setContentFromString($form_state['values']['ocr']);
        $object->ingestDatastream($ds);
    }

}