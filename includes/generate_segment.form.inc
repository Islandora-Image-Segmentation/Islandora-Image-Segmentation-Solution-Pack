<?php
/**
 * Extracts segments from the object.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The page object to be updated.
 *
 * @return array
 *   The Drupal form definition.
 */
function image_segmentation_generate_segment_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  $can_segment = image_segmentation_can_segment($object);
  return [
    'description' => [
      '#type' => 'item',
      '#description' => t('You must have the <b>Image Segmentation API</b> confugured to extract image segments.'),
    ],
    'force' => [
      '#type' => 'checkbox',
      '#title' => t('Force regeneration'),

    ],
    'submit' => [
      '#disabled' => !$can_segment,
      '#type' => 'submit',
      '#value' => t('Generate Segments'),
    ],
    'pid' => [
      '#type' => 'hidden',
      '#value' => $object->id,
    ],
  ];
}

/**
 * Submit handler for the generate segments form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function image_segmentation_generate_segment_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  $object = islandora_object_load($form_state['values']['pid']);
  if (image_segmentation_extract_segments($object, $form_state['values']['force'])) {
    drupal_set_message(t('Successfully extracted segments.'));
  }
  else {
    drupal_set_message(t('Failed to extracted segments.'), 'error');
  }
}

/**
 * Extracts segments from the object.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The page object to be updated.
 *
 * @return array
 *   The Drupal form definition.
 */
function image_segmentation_generate_segment_issues_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  return [
    'description' => [
      '#type' => 'item',
      '#description' => t('You must have the <b>Image Segmentation API</b> confugured to extract image segments.'),
    ],
    'force' => [
      '#type' => 'checkbox',
      '#title' => t('Force regeneration'),

    ],
    'submit' => [
      '#type' => 'submit',
      '#value' => t('Generate Segments'),
    ],
    'pid' => [
      '#type' => 'hidden',
      '#value' => $object->id,
    ],
  ];
}

/**
 * Submit handler for the generate segments form for issues.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function image_segmentation_generate_segment_issues_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  drupal_set_message("Creating background tasks for pages ...");
  $pages = get_pages_from_issue($form_state['values']['pid']);

  forEach($pages['response']['docs'] as $page){
    $total_for_segmentation = 1;
    $operations = array();

    $object = islandora_object_load($page['PID']);

    $operations[] = array(
      'image_segmentation_batch_extract_segments_from_page',
      array($object, $total_for_segmentation)
    );

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