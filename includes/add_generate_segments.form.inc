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
    module_load_include('inc', 'image_segmentation', 'includes/utilities');
    $object = islandora_ingest_form_get_object($form_state);

    $results = get_pages_from_issue($object->id);

    forEach($results['response']['docs'] as $page){
        variable_set($page["PID"].":segment_on_ingest", 
            $form_state['values']['generate_extracted_segments']);
    }
}

/**
 * Handles newspaper page ingest form segmentation.
 *
 * @param $form
 *  The form being handled.
 * @param $form_state
 *  The state of the form being handled.
 */

function image_segmentation_add_generate_extracted_segments_page_handler($form, &$form_state) {
    $object = islandora_ingest_form_get_object($form_state);
    variable_set($object->id.":segment_on_ingest",
        $form_state['values']['generate_extracted_segments']);
}
