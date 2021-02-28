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

  return $form;
}
?>
