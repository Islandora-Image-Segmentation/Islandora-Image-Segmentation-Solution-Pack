<?php
/**
 * @file Form and handler to generate segments on other content models
 */

/**
 * Creates a field set containing settings for adding image segmentation to
 * the ingestion of other content models.
 *
 * @param $form_state
 *  The state of the form that the field set is added to.
 *
 * @return array
 *  The drupal fieldset definition
 */
function image_segmentation_add_generate_segments_form(&$form_state) {
    return [
        '#type' => 'fieldset',
        '#weight' => 0,
        '#title' => t('Image Segmentation Settings'),
        'generate_extracted_segments' => [
            '#type' => 'checkbox',
            '#default' => 1,
            '#title' => t('Generate extracted segments'),
            '#description' => t('Determine whether the article being ingested is supposed to get derivatives generated right away or not'),
        ],
    ];
}

/**
 * Handles newspaper issue ingest form segmentation.
 *
 * @param $form
 *  The form being handled.
 * @param $form_state
 *  The state of the form being handled.
 */
function image_segmentation_add_generate_extracted_segments_issue_handler($form, &$form_state) {
    $objects = islandora_ingest_form_get_objects($form_state);
    
    $generate_segments = $form_state['values']['generate_extracted_segments'];

    $operations = array();

    $operations[] = array('image_segmentation_batch_extract_segments_from_issue',
        array($objects, $generate_segments));

    $batch = array(
        'operations' => $operations,
        'init_message' => t('Issue is being processed for segmentation'),
        'error_message' => t('Batch segmentation encountered an error'),
        'progress_message' => t('Batch segmentation process complete'),
        'finished' => 'image_segmentation_batch_extract_segments_from_page_completed',
        'progressive' => TRUE
    );

    batch_set($batch);

    background_batch_process_batch("");
}

/**
 * Handles newspaper page ingest form segmentation.
 *
 * @param $form
 * @param $form_state
 */

function image_segmentation_add_generate_extracted_segments_page_handler($form, &$form_state) {
    $generate_segments = $form_state['values']['generate_extracted_segments'];

    $object = islandora_ingest_form_get_object($form_state);
    
    if(isset($object) && $generate_segments == 1){
        $total_for_segmentation = 1;
        $operations = array();

        $operations[] = array(
            'image_segmentation_batch_extract_segments_from_page_with_delay',
            array($object, $total_for_segmentation)
        );

        drupal_set_message(t("Segmentation started for @count page(s)", array('@count' => count($operations))));

        $batch = array(
            'operations' => $operations,
            'init_message' => t('Segmentation is starting'),
            'error_message' => t('Batch segmentation encountered an error'),
            'progress_message' => t('Batch segmentation process complete'),
            'finished' => 'image_segmentation_batch_extract_segments_from_page_completed',
        );

        batch_set($batch);

        background_batch_process_batch("");
    }
}
