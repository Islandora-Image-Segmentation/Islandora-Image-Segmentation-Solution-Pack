<?php
  /**
  * @file
  * add_checkbox_to_newspaper.form.inc
  */

function islandora_newspaper_admin_settings_form_alter (&$form, &$form_state, $form_id) {
    $form['local_derivative_settings'] = array(
      '#type' => 'checkbox',
      '#title' => t('Generate derivatives immediately'),
      '#description' => t('Determine whether the article being ingested is supposed to get derivatives generated right away or not'),
    );

    return $form;
  }
?>