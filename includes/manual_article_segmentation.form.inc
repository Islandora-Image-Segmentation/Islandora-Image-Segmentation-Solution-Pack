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
 * @param $form_state
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
        '#query' => isset($form_state['values']['query']) ? $form_state['values']['query'] : NULL,
    ];

    if (isset($form['form_output']["#query"]) || isset($_GET["page"])) {
        $form['confirm'] = [
            '#type' => 'submit',
            '#value' => t('Confirm for segmentation'),
            '#submit' => ['confirm_for_segmentation'],
            '#attributes' => array("onclick" => "jQuery(this).click(function(event){event.preventDefault(); alert(\"yes\");});")
        ];
    }

    return $form;
}

/**
 * Calls batch segmentation api
 */

function confirm_for_segmentation() {
    batch_set(batch_segmentation());
    background_batch_process_batch();
}

/**
 * Creates segmentation request
 *
 * @return array of operations
 */

function batch_segmentation() {
    module_load_include('inc', 'image_segmentation', 'includes/utilities');
    
    $query_results = json_decode($_COOKIE["updated_checkboxes"], TRUE);
    $operations = array();

    foreach($query_results as $pageInfo){
        if($pageInfo["checked"] == TRUE){
            $page = $abstract_object = islandora_object_load($pageInfo["pid"]);
            if(image_segmentation_can_segment($page)){
                $operations[] = array('extractSegmentsFromPage', array($page));
            }
        }
    }

    drupal_set_message(t("Segmentation started for @count pages", array('@count' => count($operations))));

    return  array(
        'operations' => $operations,
        'finished' => 'batch_segmentation_completed',
    );
}

function extractSegmentsFromPage($page, &$context){
    module_load_include('inc', 'image_segmentation', 'includes/utilities');

    $context['message'] = t('Processing page "@title"', array('@title' => $page["dc.title"]));

    image_segmentation_extract_segments($page, TRUE);
}

/**
 * Displays batch process success or error upon completion
 */

function batch_segmentation_completed($success, $results, $operations) {
    if ($success) {
        drupal_set_message(t('@count pages  processed.', array('@count' => count($results))));
    } else {
        // An error occurred.
        // $operations contains the operations that remained unprocessed.
        $error_operation = reset($operations);
        drupal_set_message(
            t(
                'An error occurred while processing @operation with arguments : @args', 
                array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))
            )
        );
    }
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