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

module_load_include('inc', 'image_segmentation', 'includes/utilities');

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
        '#required' => TRUE,
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

    if (isset($_COOKIE["updated_checkboxes"])) {
        $form['confirm'] = [
            '#type' => 'submit',
            '#value' => t('Confirm for segmentation'),
            '#submit' => ['confirm_for_segmentation'],
        ];
    }

    return $form;
}

/**
 * Sends currently selected pages to newspaper api via drupal batch api
 */

function confirm_for_segmentation() {
    drupal_set_message($_COOKIE["updated_checkboxes"]);

    batch_set(multi_page_segmentation());
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