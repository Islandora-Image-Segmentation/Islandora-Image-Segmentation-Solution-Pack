<?php
  /**
  * @file
  * admin.form.inc
  */


  /**
  * Admin form.
  * @return array
  *   Returns admin form.
  */
  function islandora_api_admin_settings_form() {
    $form = array(
      "api_host" => array(
        '#type' => 'textfield',
        '#title' => t('API Host'),
        '#default_value' => '',
        '#description' => t('Host name for API'),
        '#size' => 50,
      ),
      "api_port" => array(
        '#type' => 'textfield',
        '#title' => t('API Port'),
        '#default_value' => '',
        '#description' => t('Port number for API'),
        '#size' => 50,
      ),
      "api_key" => array(
        '#type' => 'textfield',
        '#title' => t('API Key'),
        '#default_value' => '',
        '#description' => t('Key for API'),
        '#size' => 50,
      ),
      "confidence_threshold" => array(
        '#type' => 'textfield',
        '#title' => t('API Confidence Threshold'),
        '#default_value' => '',
        '#description' => t('Confidence Threshold for API ML results'),
        '#size' => 50,
      ),
      "solr_article_query" => array(
        '#type' => 'textfield',
        '#title' => t('Solar Article Query'),
        '#default_value' => '',
        '#description' => t('Query string for selecting articles for processing'),
        '#size' => 50,
      ),
    );

    return system_settings_form($form);
  }
?>