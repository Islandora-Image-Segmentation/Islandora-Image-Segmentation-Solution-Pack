<?php
/**
 * @file Form and handler to generate segments on other content models
 */

/**
 * Creates a field set containing settings for adding image segmentation to
 * the ingestion of other content models.
 *
 * @param $form_state
 *  The state of the form that the field set is added to.
 *
 * @return array
 *  The drupal fieldset definition
 */
function image_segmentation_add_generate_segments_form(&$form_state) {
    return [
        '#type' => 'fieldset',
        '#weight' => 0,
        '#title' => t('Image Segmentation Settings'),
        'generate_extracted_segments' => [
            '#type' => 'checkbox',
            '#default' => 1,
            '#title' => t('Generate extracted segments'),
            '#description' => t('Determine whether the article being ingested is supposed to get derivatives generated right away or not'),
        ],
    ];
}

/**
 * Handles ingest forms with image segmentation settings added.
 *
 * @param $form
 *  The form being handled.
 * @param $form_state
 *  The state of the form being handled.
 */
function image_segmentation_add_generate_extracted_segments_handler($form, &$form_state) {
    module_load_include('inc', 'image_segmentation', 'includes/utilities');
    $objects = islandora_ingest_form_get_objects($form_state);
    $generate_segments = $form_state['values']['generate_extracted_segments'];
    
    if(isset($objects) && !empty($objects) && $objects != null && $generate_segments == 1){
        $total_for_segmentation = count($objects);
        $operations = array();
        $page_number = 0;

        foreach($objects as $pageInfo){
            $issue_pid = $pageInfo->id;
            drupal_set_message($issue_pid);

            $split_issue_pid = explode(":", $issue_pid);
            $formatted_issue_pid = $split_issue_pid[0] . "\:" . 
                $split_issue_pid[1];

            //Get associated pages from solr
            $url = "http://localhost:8080/solr/collection1/select?q=*:*&wt=json";
            $data = file_get_contents($url
                . "&fq=RELS_EXT_hasModel_uri_s:info\:fedora\/islandora\:newspaperPageCModel"
                . "&fq=RELS_EXT_isPageOf_uri_s:info\:fedora\/".$formatted_issue_pid 
                . "&start=0");

            if (isset($data)) {
                $results = json_decode($data, TRUE);

                forEach($results['response']['docs'] as $doc){
                    $page_number++;
                    if(image_segmentation_can_segment($doc["PID"])){
                        $operations[] = array('image_segmentation_batch_extract_segments_from_page',
                            array($issue_pid, $total_for_segmentation, $page_number));
                    }
                }
            }
        }

        if(!empty($operations)){
            $batch = array(
                'operations' => $operations,
                'init_message' => t('Segmentation is starting'),
                'error_message' => t('Batch segmentation encountered an error'),
                'progress_message' => t('Batch segmentation process complete'),
                'finished' => 'image_segmentation_batch_extract_segments_from_page_completed',
            );

            batch_set($batch);

            background_batch_process_batch("");
        }
    }
}
