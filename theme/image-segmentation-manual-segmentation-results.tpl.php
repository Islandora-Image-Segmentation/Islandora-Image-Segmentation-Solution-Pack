<div>
  <?php
  /**
   * @file
   * This is the template
   *   file
   *   to
   *   show
   *   manual
   *   segmentation
   *   query
   *   results.
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

    if (isset($_GET["updated_checkboxes"])) {
      $updated_checkboxes = $_GET["updated_checkboxes"];
    }

    $num_of_pages = 0;

    if (isset($solr_query)) {
      echo "<h4>Results: Page " . $page . "</h4><br>";

      $solr_query = str_replace(" ", "", $solr_query);

      //Get SOLR results

      $start = $page * 10 - 10;
      $url = "http://localhost:8080/solr/collection1/select?q=" . $solr_query . "&wt=json";
      $data = file_get_contents($url . "&fq=RELS_EXT_hasModel_uri_s:info\:fedora\/islandora\:newspaperPageCModel" . "&start=" . $start);

      if (isset($data)) {
        $results = json_decode($data, TRUE);

        $num_of_pages = ceil($results["response"]["numFound"] / 10);
        $docs = $results['response']['docs'];

        if (isset($docs)) {
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
            print "<p><strong>Choose for segmentation:</strong> <input onclick=\"updated_checkboxes()\" checked type=\"checkbox\" class=\"manual_segmentation_checkboxes\" value=\"{$value['PID']}\" name=\"{$value['PID']}\"></p>";
            print "<hr>";
          }
        }
      }
    }
    
  } catch (Exception $e) {
    drupal_set_message("No results found", "error");
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
    <script>
      //POST updated checkbox information back to manual segmentation page
      function updated_checkboxes() {
        const checkboxes = document.getElementsByClassName("manual_segmentation_checkboxes");
        let updated_checkboxes = [];

        for(let i = 0; i < checkboxes.length; i++){
          updated_checkboxes.push({pid: checkboxes[i].value, checked: checkboxes[i].checked});
        }

        document.cookie = "updated_checkboxes=" + JSON.stringify(updated_checkboxes);
      }

      updated_checkboxes();
    </script>
</div>