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
    ?>
    
    <table>
        <div class="manual_segmentation_options">
            <label>Select all:&nbsp;&nbsp;</label>
            <input id="manual_segmentation_select_all" type="checkbox" onclick="select_all_checkboxes()"/>
        </div>
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
                
                if(empty($segments) == TRUE){
                    print "<td><strong>Choose for segmentation:</strong> <input onclick=\"updated_checkboxes()\" 
checked type=\"checkbox\" class=\"manual_segmentation_checkboxes\" value=\"{$value['PID']}\" name=\"{$value['PID']}\"></td>";
                }else{
                    print "<td><strong>Choose for segmentation:</strong> <input onclick=\"updated_checkboxes()\" 
type=\"checkbox\" class=\"manual_segmentation_checkboxes\" value=\"{$value['PID']}\" name=\"{$value['PID']}\"></td>";
                }
                
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
</div>