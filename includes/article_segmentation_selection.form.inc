<?php
/**
 * Solr query form.
 * @return array
 *   Returns Solr query form.
 */

function solr_query_form($form, &$form_state) {
  $form['query'] = array(
    '#title' => t('Query'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#descritpion' => t('Select which articles get manually segmented'),
  );

  $form['submit'] = array(
    '#title' => t('Submit'),
    '#type' => 'submit',
  );

  return $form;
}

function solr_query_form_submit($form, &$form_state) {
  drupal_set_message(t('The form has been submitted.'));
}
