<?php
/**
 * @file
 * This is the template file for the object page for image segments.
 * Available variables:
 * - $object: The islandora object being displayed
 * - $segment: The image of the segment
 */
?>

<div class="image-segmentation-object islandora">
    <?php if (isset($segment)): ?>
        <div><?php print $segment ?></div>
    <?php endif; ?>
    <?php if (isset($object['OCR'])): ?>
        <div class="ocr content">
            <h2>OCR:</h2>
            <p><?php print $object['OCR']->content ?></p>
        </div>
    <?php endif; ?>
</div>