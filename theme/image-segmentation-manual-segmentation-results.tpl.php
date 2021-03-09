<div class="manual-article-segmentation-container">
  <?php
  /**
   * @file
   * This is the template file to show manual segmentation query results.
   */

  module_load_include('inc', 'image_segmentation', 'includes/utilities');
  ?>

  <?php
  try {
    if (isset($_GET["page"])) {
      $page = (int) $_GET["page"];
    }
    else {
      $page = 1;
    }

    if (isset($query)) {
      $solr_query = $query;
    }
    elseif (isset($_GET["query"])) {
      $solr_query = $_GET["query"];
    }

    $num_of_pages = 0;

    if (isset($solr_query)) {
      echo "<h4>Results: Page " . $page . "</h4><br>";

      //Get SOLR results

      $start = $page * 10 - 10;
      $url = "http://localhost:8080/solr/collection1/select?q=" . $solr_query . "&wt=json";
      $data = file_get_contents($url . "&fq=RELS_EXT_hasModel_uri_s:info\:fedora\/islandora\:newspaperPageCModel" . "&start=" . $start);
      $results = json_decode($data, TRUE);

      $num_of_pages = ceil($results["response"]["numFound"] / 10);
      $docs = $results['response']['docs'];

      //print results as HTML

      foreach ($docs as $value) {
        //Get object segments

        $abstract_object = islandora_object_load($value['PID']);
        $segments = image_segmentation_get_segments($abstract_object);

        //Check if object is segmented

        $is_segmented = empty($segments) ? "No" : "Yes";

        print "<p><strong>PID:</strong> {$value['PID']}</p>";
        print "<p><strong>Title:</strong> {$value['dc.title'][0]}</p>";
        print "<p><strong>MIMETYPE:</strong> {$value['fedora_datastream_version_OBJ_MIMETYPE_mt'][0]}</p>";
        print "<p><strong>Segmented:</strong> {$is_segmented}</p>";
        print "<hr>";
      }
    }
  } catch (Exception $e) {
    echo $e;
  }
  ?>
    <br>
    <div class="manual-article-segmentation-pagination">
      <?php
      for ($i = 1; $i <= $num_of_pages; $i++) {
        echo '<a href = "manual_article_segmentation?query=' . $solr_query . '&page=' . $i . '">' . $i . '&nbsp;&nbsp; </a>';
      }
      ?>
    </div>
    <br>
</div>