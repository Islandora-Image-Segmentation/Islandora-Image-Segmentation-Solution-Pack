<?php

/**
 * Solr query form.
 * @return array
 *   Returns Solr query form.
 */

function manual_article_segmentation_form($form, &$form_state) {
  $form['query'] = array(
    '#title' => t('Query'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#descritpion' => t('Select which articles get manually segmented'),
  );

  $form['submit'] = array(
    '#title' => t('Submit'),
    '#type' => 'submit',
  );

  return $form;
}

function manual_article_segmentation_form_submit($form, &$form_state) {
  drupal_set_message(t('The form has been submitted.'));
}