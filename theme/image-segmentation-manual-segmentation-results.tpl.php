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

    drupal_add_css(drupal_get_path('module', 'image_segmentation') . '/css/image_segmentation_segment_list.css');
    module_load_include('inc', 'image_segmentation', 'includes/utilities');
    ?>
    
    <table>
        <thead>
            <tr>
                <td>PID</td>
                <td>Title</td>
                <td>Mimetype</td>
                <td>Is Segmented?</td>
                <td>Action</td>
            </tr>
        </thead>
        <tbody>
        <?php
        if (!empty($_COOKIE["manual_segmentation_page"])) {
            $page = (int) $_COOKIE["manual_segmentation_page"];
        }else {
            $page = 1;
        }

        if (isset($object)) {
            $solr_query = $object;
            $solr_results = image_segmentation_get_newspaper_pages_from_solr($page, $object);
        }elseif (!empty($_COOKIE['manual_segmentation_query'])){
            $solr_query = json_decode($_COOKIE['manual_segmentation_query']);
            $solr_results = image_segmentation_get_newspaper_pages_from_solr($page, $solr_query);
        }

        if(isset($solr_results["docs"])){
            $docs = $solr_results["docs"];
            $num_of_pages = $solr_results["num_of_pages"];
        }else{
            $num_of_pages = 0;
        }

        if (isset($docs)) {
            foreach ($docs as $value) {
                //Get object segments

                $abstract_object = islandora_object_load($value['PID']);
                $segments = image_segmentation_get_segments($abstract_object);

                //Check if object is segmented
                $is_segmented = empty($segments) ? "No" : "Yes";
                $row_color = empty($segments) ? "#EC7063" : "#ABEBC6";

                print "<tr style='background-color: $row_color'>";
                print "<td><strong>PID:</strong> {$value['PID']}</td>";
                print "<td><strong>Title:</strong> {$value['dc.title'][0]}</td>";
                print "<td><strong>MIMETYPE:</strong> {$value['fedora_datastream_version_OBJ_MIMETYPE_mt'][0]}</td>";
                print "<td><strong>Segmented:</strong> {$is_segmented}</td>";
                print "<td><strong>Choose for segmentation:</strong> <input onclick=\"updated_checkboxes()\" checked type=\"checkbox\" class=\"manual_segmentation_checkboxes\" value=\"{$value['PID']}\" name=\"{$value['PID']}\"></td>";
                print "</tr>";
            }
        }
        ?>
        </tbody>
    </table>
    <br>
    <div id="manual-article-segmentation-pagination">
        <?php
        if(isset($solr_query)){
            print "<ul>";
            for ($i = 1; $i <= $num_of_pages; $i++) {
                if($i != $page){
                    print('<li><a onclick="handle_page_change('.$i.',\''.$solr_query.'\')">'.$i.'</a></li>');
                }else{
                    print('<li class="manual_segmentation_pagination_active"><a onclick="handle_page_change('.$i.',\''.$solr_query.'\')">'.$i.'</a></li>');
                }
            }
            print "</ul>";
        }
        ?>
    </div>
    <br>
    <script>
      const COOKIE_EXPIRY_IN_MINUTES = 30;

      if(localStorage.getItem("manual_seg_checkbox_states") !== null){
        const checkbox_states = JSON.parse(localStorage.getItem("manual_seg_checkbox_states"));
        const checkboxes = document.getElementsByClassName("manual_segmentation_checkboxes");
        console.log(checkbox_states);

        for (let i = 0; i < checkboxes.length; i++) {
          if(checkboxes[i].value in checkbox_states){
            checkboxes[i].checked = checkbox_states[checkboxes[i].value];
          }else {
            checkboxes[i].checked = true;
          }
        }
        
        updated_checkboxes();
      }else{
        updated_checkboxes();
      }
      
      //Pagination
      function handle_page_change(page, query){
        const current_date = new Date();
        current_date.setMinutes(current_date.getMinutes() + COOKIE_EXPIRY_IN_MINUTES);

        document.cookie = "manual_segmentation_page=" + page + "; expires=" + current_date.toUTCString();
        document.cookie = "manual_segmentation_query=" + JSON.stringify(query) + "; expires=" + current_date.toUTCString();
        window.location.href = window.location.href;
      }

      //POST updated checkbox information back to manual segmentation page
      function updated_checkboxes() {
        const checkboxes = document.getElementsByClassName("manual_segmentation_checkboxes");
        let updated_checkboxes = [];
        let updated_checkboxes_map = {};

        for (let i = 0; i < checkboxes.length; i++) {
          updated_checkboxes.push({
            pid: checkboxes[i].value,
            checked: checkboxes[i].checked
          });

          updated_checkboxes_map[checkboxes[i].value] = checkboxes[i].checked;
        }
        
        if(localStorage.getItem("manual_seg_checkbox_states") !== null){
          updated_checkboxes_map = {...JSON.parse(localStorage.getItem("manual_seg_checkbox_states")), ...updated_checkboxes_map};
        }

        localStorage.setItem("manual_seg_checkbox_states", JSON.stringify(updated_checkboxes_map));

        const current_date = new Date();
        current_date.setMinutes(current_date.getMinutes() + COOKIE_EXPIRY_IN_MINUTES);

        document.cookie = "manual_segmentation_updated_checkboxes=" + JSON.stringify(updated_checkboxes) + "; expires=" + current_date.toUTCString();
      }
    </script>
</div>