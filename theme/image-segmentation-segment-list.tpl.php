<?php
/**
 *
 * @file
 * This is the template file for the corpus sidebar .
 */
?>

<div id="sidebar_container">

    <!--Filter Sidebar Section-->
    <div class="segment_list_section">
        <h2>Filter</h2>
        <p>Filter by category:</p>
        <select name="extract_image_category" id="extract_image_category">
          <?php foreach ($content_types as &$type) : ?>
              <option value=<?php $type ?>><?php print $type ?></option>
          <?php endforeach ?>
        </select>
        <br/>
        <br/>
        <button id="sidebar_filter_apply_button">Apply</button>
    </div>

    <!--Segments Sidebar Section-->
    <div class="segment_list_section">
        <h2>Segments</h2>
        <div id="segment_list_gallery">
            <?php if (count($segments) > 0): ?>
                <?php foreach ($segments as &$segment) : ?>
                    <div class="segment_preview">
                        <h3><?php print $segment['type'] ?>:</h3>
                        <a href=<?php print $segment['link'] ?>>
                            <?php print theme('image', [
                              'path' => $segment['tn_url'],
                              'title' => $segment['object']->label,

                            ]) ?>
                        </a>
                    </div>
                <?php endforeach ?>
          <?php endif; ?>
          <?php if (count($segments) == 0): ?>
            <p>This page has no segments of the requested type.</p>
          <?php endif; ?>
        </div>
    </div>
</div>
