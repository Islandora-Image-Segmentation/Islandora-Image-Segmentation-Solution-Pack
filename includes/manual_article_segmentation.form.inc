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

  return $form;
}

function manual_article_segmentation_form_submit($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'image_segmentation') .'/css/manual-segmentation-results.css');
  $form_state['rebuild'] = TRUE;
}