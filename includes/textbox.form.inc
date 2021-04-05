
<?php
/**
 * @file
 * textbox.form.inc
 */
use NewspaperNavigaton\SegmentationClient;
/**
 * textbox form.
 * @return array
 *   Returns admin form.
 */


function mymodule_custom_search_form($form_state) {
$form = array();

$form['keyword'] = array(
'#type' => 'textfield',
'#title' => t('Enter A Valid Image ID'),
'#required' => TRUE,
);

$form['submit'] = array(
'#type' => 'submit',
'#value' => t('Submit'),
);

$form['#submit'] = array('mymodule_custom_search_form_submit');

return $form; //
}

function mymodule_custom_search_form_submit($form, &$form_state) {
    if($form_state['values']['keyword'] == 'Cadre') {
        $redirect_url = 'islandora/object/newspapers%3A2';
        $data = 'newspapers%3A2';
        $url = 'http://33.33.33.10:8080/adore-djatoka';
        //$client = new \NewspaperNavigator\SegmentationClient(); not working
        //$client->segmentUrl('islandora/object/newspapers%3A2'); not working
        $response = drupal_http_request($url, array('method' => 'POST', 'headers' => array('Content-Type' => 'application/x-www-form-urlencoded; charset=utf-8')));
        if ($response->code == "200") {
            $result = $response->$data;
        }
        //$client = new \NewspaperNavigator\SegmentationClient();
        //$client->segmentUrl("islandora/object/newspapers%3A2");
    }
    else if($form_state['values']['keyword'] == 'Examiner'){
        $redirect_url = 'islandora/object/newspapers%3A3';
    }
    else if($form_state['values']['keyword'] == 'Gaurdian'){
        $redirect_url = 'islandora/object/newspapers%3A4';
    }
    else if($form_state['values']['keyword'] == 'Impartial'){
        $redirect_url = 'islandora/object/newspapers%3A1';
    }
    else if($form_state['values']['keyword'] == 'Island naturalist'){
        $redirect_url = 'islandora/object/newspapers%3A5';
    }
    else{
        drupal_set_message(t('Image Could Not Be Found!'),'error');
    }

    $form_state['redirect'] = $redirect_url;

}



?>

