<?php
/**
 * @file
 * add_checkbox_to_newspaper.form.inc
 *
 * @param $form
 * @param $form_state
 * @param $form_id
 *
 * @return mixed
 */

function islandora_ingest_form_alter (&$form, &$form_state, $form_id) {

  module_load_include('inc', 'islandora', 'includes/ingest.form');

  if (isset($form['type']) && $form['type']['#value'] . '_node_settings' == $form_id) {
    $form['generate_derivatives_immediately'] = array(
      '#type' => 'checkbox',
      '#title' => t('Generate derivatives immediately'),
      '#description' => t('Determine whether the article being ingested is supposed to get derivatives generated right away or not'),
    );
  }

  return $form;
  }
