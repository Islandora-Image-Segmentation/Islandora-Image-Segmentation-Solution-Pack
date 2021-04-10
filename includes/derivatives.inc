<?php

function image_segmentation_create_tn_jp2_derivative(AbstractObject $object, $force = TRUE) {
  return image_segmentation_create_scaled_jp2_derivative($object, 'TN', 2, 'image/jpeg', $force);
}

function image_segmentation_create_jpg_jp2_derivative(AbstractObject $object, $force = TRUE) {
  return image_segmentation_create_scaled_jp2_derivative($object, 'JPG', 100, 'image/jpeg', $force);
}

function image_segmentation_create_jp2_jp2_derivative(AbstractObject $object, $force = TRUE) {
  try {
    image_segmentation_image_add_datastream($object, 'JP2', $object['OBJ']->content, 'image/jp2', "JP2 Datastream");
    return [
      'succes' => TRUE,
      'messages' => [
        [
          'message' => t("Created JP2 derivative . "),
          'type' => 'dsm',
          'severity' => 'status',
        ],
      ],
    ];
  } catch (Execption $e) {
    return [
      'succes' => TRUE,
      'messages' => [
        [
          'message' => t("Failed to create JP2 derivative . "),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ],
      ],
    ];
  }

}

function image_segmentation_create_scaled_jp2_derivative(AbstractObject $object, $dsid, $level, $ds_mimetype = 'image/jpeg', $force = TRUE) {
  try {
    module_load_include('inc', 'image_segmentation', 'includes/utilities');
    $ds_content = image_segmentation_scale_jp2($object['OBJ'], $level);
    image_segmentation_image_add_datastream($object, $dsid, $ds_content, $ds_mimetype, "$dsid Datastream");
    return [
      'succes' => TRUE,
      'messages' => [
        [
          'message' => t("Created $dsid derivative . "),
          'type' => 'dsm',
          'severity' => 'status',
        ],
      ],
    ];
  } catch (Execption $e) {
    return [
      'succes' => TRUE,
      'messages' => [
        [
          'message' => t("Failed to create $dsid derivative . "),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ],
      ],
    ];
  }
}

function image_segmentation_image_add_datastream(AbstractObject $object, $dsid, $content, $mimetype, $label) {
  $exists = isset($object[$dsid]);
  if ($exists) {
    $ds = $object[$dsid];
  }
  else {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $label;
    $ds->mimeType = $mimetype;
  }

  $ds->content = $content;

  if (!$exists) {
    $object->ingestDatastream($ds);
  }
}