<?php
  $form = array(
    "api_host" => array(
      '#type' => 'textfield',
      '#title' => 'API Host',
      '#default_value' => '',
      '#description' => 'Host name for API',
      '#size' => 50,
    ),
    "api_port" => array(
      '#type' => 'textfield',
      '#title' => 'API Port',
      '#default_value' => '',
      '#description' => 'Port number for API',
      '#size' => 50,
    ),
    "api_key" => array(
      '#type' => 'textfield',
      '#title' => 'API Key',
      '#default_value' => '',
      '#description' => 'Key for API',
      '#size' => 50,
    ),
    "confidence_threshold" => array(
      '#type' => 'textfield',
      '#title' => 'API Confidence Threshold',
      '#default_value' => '',
      '#description' => 'Confidence Threshold for API ML results',
      '#size' => 50,
    ),
  );

  //return system_settings_form($form);
?>