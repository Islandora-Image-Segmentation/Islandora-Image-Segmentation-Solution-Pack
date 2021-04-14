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

/**
 * Adds a solr query form to the manual segmentation page
 *
 * @param array $form
 *  The form being created
 * @param $form_state
 *  Current state of the created form
 *
 * @return array
 */

function manual_article_segmentation_form(array $form, &$form_state) {
    $form['query'] = [
        '#title' => t('Query'),
        '#type' => 'textfield',
        '#descritpion' => t('Select which articles get manually segmented'),
    ];

    $form['submit'] = [
        '#title' => t('Submit'),
        '#type' => 'submit',
        '#value' => t('Submit'),
    ];

    $form['form_output'] = [
        '#theme' => 'image_segmentation_manual_segmentation_results',
        '#object' => isset($form_state['values']['query']) ? $form_state['values']['query'] : NULL,
    ];

    if (isset($form['form_output']["#object"]) || isset($_COOKIE["manual_segmentation_page"])) {
        $form['confirm'] = [
            '#type' => 'submit',
            '#value' => t('Confirm for segmentation'),
            '#submit' => ['manual_article_segmentation_confirm_for_segmentation'],
            '#attributes' => array("onclick" => "jQuery(this).click(function(event){event.preventDefault(); alert(\"yes\");});")
        ];
    }

    return $form;
}

/**
 * Calls batch segmentation api
 */

function manual_article_segmentation_confirm_for_segmentation() {
    $batch = manual_article_segmentation_batch_segmentation();
    batch_set($batch);

    background_batch_process_batch("");
}

/**
 * Creates segmentation request and initiates batch process
 *
 * @return array of operations
 */

function manual_article_segmentation_batch_segmentation() {
    module_load_include('inc', 'image_segmentation', 'includes/utilities');
    
    $query_results = json_decode($_COOKIE["manual_segmentation_updated_checkboxes"], TRUE);
    $total_for_segmentation = 0;
    $operations = array();
    $page_number = 0;

    foreach($query_results as $pageInfo){
        if($pageInfo["checked"] == TRUE){
            $total_for_segmentation++;
        }
    }

    foreach($query_results as $pageInfo){
        if($pageInfo["checked"] == TRUE){
            $page_number++;
            $page = islandora_object_load($pageInfo["pid"]);
            if(image_segmentation_can_segment($page)){
                $operations[] = array('image_segmentation_batch_extract_segments_from_page', array($page, $total_for_segmentation, $page_number));
            }
        }
    }

    drupal_set_message(t("Segmentation started for @count page(s)", array('@count' => count($operations))));

    return  array(
        'operations' => $operations,
        'init_message' => t('Segmentation is starting'),
        'error_message' => t('Batch segmentation encountered an error'),
        'progress_message' => t('Batch segmentation process complete'),
        'finished' => 'image_segmentation_batch_extract_segments_from_page_completed',
    );
}

/**
 * Handles solr query form submission by rebuilding form to display results
 *
 * @param $form
 * @param $form_state
 */

function manual_article_segmentation_form_submit($form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'image_segmentation') . '/css/manual-segmentation-results.css');
    $form_state['rebuild'] = TRUE;
}