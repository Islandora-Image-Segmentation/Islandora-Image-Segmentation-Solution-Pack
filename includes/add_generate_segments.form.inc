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
            '#title' => t('Generate extracted segments'),
            '#description' => t('Determine whether the article being ingested is supposed to get derivatives generated right away or not'),
        ],
    ];
}

/**
 * Handles ingest forms with image segmentation settings added.
 *
 * @param $form
 *  The form being handled.
 * @param $form_state
 *  The state of the form being handled.
 */
function image_segmentation_add_generate_extracted_segments_handler($form, &$form_state) {
    drupal_set_message(json_encode($form_state['values']));
    $generate_segments = $form_state['values']['generate_extracted_segments'];
    drupal_set_message(t("Extract Segments? = $generate_segments"));
}
