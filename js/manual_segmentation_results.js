const COOKIE_EXPIRY_IN_MINUTES = 10;

(function ($) {
  window.onload = function () {

    if(localStorage.getItem("manual_seg_checkbox_states") !== null){
      const checkbox_states = JSON.parse(localStorage.getItem("manual_seg_checkbox_states"));
      const checkboxes = document.getElementsByClassName("manual_segmentation_checkboxes");

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
  }
}(jQuery));

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

//Pagination
function handle_page_change(page, query){
  const current_date = new Date();
  
  current_date.setMinutes(current_date.getMinutes() + COOKIE_EXPIRY_IN_MINUTES);

  document.cookie = "manual_segmentation_page=" + page + "; expires=" + current_date.toUTCString();
  document.cookie = "manual_segmentation_query=" + JSON.stringify(query) + "; expires=" + current_date.toUTCString();
  window.location.href = window.location.href;
}
