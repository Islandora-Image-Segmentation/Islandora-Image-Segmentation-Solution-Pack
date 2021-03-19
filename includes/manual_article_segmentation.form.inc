<?php

/**
 * Solr query form.
 *
 * @param $form
 * @param $form_state
 *
 * @return array
 *   Returns Solr query form.
 */

module_load_include('inc', 'image_segmentation', 'includes/utilities');

function manual_article_segmentation_form(array $form, &$form_state) {
  $form['query'] = array(
    '#title' => t('Query'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#descritpion' => t('Select which articles get manually segmented'),
  );

  $form['submit'] = array(
    '#title' => t('Submit'),
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['form_output'] = array
  (
    '#theme' => 'image_segmentation_manual_segmentation_results',
    '#query' => isset($form_state['values']['query']) ? $form_state['values']['query'] : NULL,
  );

  if(isset($_COOKIE["updated_checkboxes"])){
    $form['confirm'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm for segmentation'),
      '#submit' => array('confirm_for_segmentation'),
    );
  }

  return $form;
}

function confirm_for_segmentation () {
  drupal_set_message($_COOKIE["updated_checkboxes"]);
}

function manual_article_segmentation_form_submit($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'image_segmentation') .'/css/manual-segmentation-results.css');
  $form_state['rebuild'] = TRUE;
}