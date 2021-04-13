<?php

function segment_list_callback() {
  [$form, $form_state, $form_id, $form_build_id, $commands] = ajax_get_form();
  drupal_process_form($form['#form_id'], $form, $form_state);
  $type = $form_state['values']['type_filter'];
  $page_id = $form_state['values']['page_id'];

  $repository = islandora_get_tuque_connection()->repository;
  $page = $repository->getObject($page_id);

  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  $segments = image_segmentation_get_segments($page, $type);
  $segments_previews = [
    '#type' => 'container',
    '#attributes' => [
      'id' => 'segment_list_gallery',
    ],
    [
      'segments' => image_segmentation_segment_previews($segments),
    ],
  ];

  $commands = [
    ajax_command_replace('#segment_list_gallery', render($segments_previews)),
  ];
  ajax_deliver(['#type' => 'ajax', '#commands' => $commands]);
}


function segment_list_array(AbstractObject $object) {
  $filter = [
    '#type' => 'container',
    '#attributes' => [
      'class' => ['segment_filter'],
    ],
    [
      drupal_get_form('image_segmentation_segment_filter_form', $object),
    ]
  ];

  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  $segments = image_segmentation_get_segments($object);

  $segment_section = [
    '#type' => 'container',
    '#attributes' => [
      'class' => ['segment_list_section'],
    ],
    [
      '#type' => 'container',
      '#attributes' => [
        'id' => 'segment_list_gallery',
      ],
      [
        'segments' => image_segmentation_segment_previews($segments),
      ],
    ],
  ];

  return [
    'filter' => $filter,
    'segments' => $segment_section,
  ];
}

function image_segmentation_segment_previews($segments) {
  $previews = [];
  foreach ($segments as $segment) {
    $previews[] = image_segmentation_segment_preview($segment);
  }
  return $previews;
}

function image_segmentation_segment_preview(AbstractObject $segment) {

  return [
    '#type' => 'container',
    '#attributes' => [
      'class' => ['segment_preview'],
    ],
    [
      '#type' => 'html_tag',
      '#tag' => 'a',
      '#attributes' => [
        'href' => url("islandora/object/{$segment->id}/"),
      ],
      '#pre_render' => ['image_segmentation_render_value_array'],
      '#value_array' => [
        'head' => [
          '#type' => 'html_tag',
          '#tag' => 'h2',
          '#value' => image_segmentation_get_type($segment),
        ],
        'image' => [
          '#theme' => 'image',
          '#path' => url("islandora/object/{$segment->id}/datastream/TN/view"),
          '#title' => $segment->label,
        ],
      ],
    ],
  ];
}

function image_segmentation_segment_filter_form($form, $form_state, $page) {
  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  $types = image_segmentation_get_content_types();

  $type_options['all'] = 'All';
  foreach ($types as $type) {
    $num = count(image_segmentation_get_segments($page, $type));
    $type_options[$type] = "$type ($num)";
  }
  return [
    'type_filter' => [
      '#type' => 'select',
      '#title' => t('Filter by type'),
      '#options' => $type_options,
    ],
    'apply' => [
      '#type' => 'button',
      '#value' => 'Apply',
      '#ajax' => [
        'path' => 'image_segmentation/segments',
      ],
    ],
    'page_id' => [
      '#type' => 'value',
      '#value' => $page->id,
    ],
  ];
}

function image_segmentation_render_value_array($element) {
  if (!isset($element['#value']) && isset($element['#value_array'])) {
    $element['#value'] = render($element['#value_array']);
  }
  return $element;
}
