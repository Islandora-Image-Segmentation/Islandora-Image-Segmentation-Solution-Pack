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
  function image_segmentation_admin_form() {
    $form = array(
      "image_segmentation_api_host" => array(
        '#type' => 'textfield',
        '#title' => t('API Host'),
        '#default_value' => variable_get('image_segmentation_api_host', 'localhost'),
        '#description' => t('Host name for API'),
        '#size' => 50,
      ),
      "image_segmentation_api_port" => array(
        '#type' => 'textfield',
        '#title' => t('API Port'),
        '#default_value' => variable_get('image_segmentation_api_port', '8008'),
        '#description' => t('Port number for API'),
        '#size' => 50,
      ),
      "image_segmentation_api_key" => array(
        '#type' => 'password',
        '#title' => t('API Key'),
        '#default_value' => variable_get('image_segmentation_api_key', ""),
        '#description' => t('Key for API'),
        '#size' => 50,
      ),
      "image_segmentation_confidence_threshold" => array(
        '#type' => 'textfield',
        '#title' => t('API Confidence Threshold'),
        '#default_value' => variable_get('image_segmentation_confidence_threshold', '0.6'),
        '#description' => t('Confidence Threshold for API ML results'),
        '#size' => 50,
      ),
      "image_segmentation_timeout" => array(
        '#type' => 'textfield',
        '#title' => t('API Timeout'),
        '#default_value' => variable_get('image_segmentation_timeout', '60.0'),
        '#description' => t('How long to wait for a response in seconds.'),
        '#size' => 50,
      )
    );

    return system_settings_form($form);
  }
?>
