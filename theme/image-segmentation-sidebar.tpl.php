**
* @file
* This is the template file for the corpus sidebar.
*/

drupal_add_css("$path/css/sidebar.css");
<div id="sidebar_container">
  <div id="sidebar_container_inner">

    <!--Filter Sidebar Section-->
    <div class="sidebar_section">
      <h2>Filter</h2>
      <p>Filter by category:</p>
      <select name="extract_image_category" id="extract_image_category">
        <option value="illustration">Illustration</option>
        <option value="photograph">Photograph</option>
        <option value="comics/cartoon">Comic Cartoon</option>
        <option value="editorial_cartoon">Editorial Cartoon</option>
        <option value="map">Map</option>
        <option value="headline">Headline</option>
        <option value="ad">Ad</option>
      </select>
      <br />
      <br />
      <button id="sidebar_filter_apply_button">Apply</button>
    </div>

    <!--Segments Sidebar Section-->
    <div class="sidebar_section">
      <h2>Segments</h2>
      <div id="sidebar_segment_gallery">
        <img src="https://via.placeholder.com/300" />
        <br />
        <button>View Similar Segments</button>
      </div>
    </div>
  </div>
</div>