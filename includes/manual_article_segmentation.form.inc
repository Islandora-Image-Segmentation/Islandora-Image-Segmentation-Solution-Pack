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
  if(isset($_GET["page"])){
    $page = $_GET["page"];
  }
  else {
    $page = 1;
  }

  $start = $page*10-10;
  $query = $form_state["values"]["query"];
  $url = "http://localhost:8080/solr/collection1/select?q=".$query."&wt=php";
  $file = file_get_contents($url."&start=".$start);
  //$numOfPages = ceil($file["response"]["numFound"]/10);

  drupal_set_message(t('The form has been submitted.'));
}