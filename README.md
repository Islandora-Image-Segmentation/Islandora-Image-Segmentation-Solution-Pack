# Islandora-Image-Segmentation-Solution-Pack

## Introduction
An Islandora solution pack that enables an image segmentation functionality, where visual content such as cartoons and
advertisements can be extracted from hosted images. This solution pack requires the use of an external service, a segmentation API that is found [here](https://github.com/Islandora-Image-Segmentation/Newspaper-Navigator-API).

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Newspaper Solution Pack](https://github.com/Islandora/islandora_solution_pack_newspaper)
* [Background Batch](https://www.drupal.org/sandbox/gielfeldt/1130434)
* [Background Process](https://www.drupal.org/project/background_process)
* [Composer Manager](https://www.drupal.org/project/composer_manager)

Furthermore, you will need to have the image segmentation API from [here](https://github.com/Islandora-Image-Segmentation/Newspaper-Navigator-API) running somewhere and accessible.

## Installation
Install as you might a regular Islandora solution pack. See [this](https://www.drupal.org/docs/7/extend/installing-modules) for more information.

## Configuration 
A configuration page can be found at Administration » Islandora » Solution Pack Configuration » Image Segmentation Module. 

The configuration page allows you to set the following parameters:

1. API Host: The hostname where your API is running. For example, `localhost` or `37.53.12.12`.
2. API Port: The port that your API is running on. For example, `8008`.
3. API Key: The API key that your API is using. Can be left blank if the API was not launched with a key.
4. Confidence Threshold: A number between 0 and 1. All returned segments below this threshold will not be ingested.

## Per-page Segmentation
You can manually segment individual pages. 
Navigate to a newspaper page >> Manage >> Page >> Generate Segments.
From here, you can submit that page for segmentation or force the segments to regenerate.

## SOLR Query Segmentation
Instead of segmenting individual pages, you can submit a batch of pages that match a SOLR query for segmentation.
Navigate to Islandora >> Manual Article Segmentation >> Enter a SOLR query >> Check/uncheck all of the pages that you want to submit >> Confirm.
This will submit all of the pages for segmentation, and the processing will happen in the background. 
To check the status of your background jobs, navigate to http://YOUR_ISLANDORA:YOUR_PORT/admin/config/system/batch/overview.


## Test Drive
To try the solution pack out, you can use a Vagrant virtual machine which has an Islandora instance with this solution pack already installed. Follow these instructions to launch Islandora  

1. Start the virtual machine 
    - **VirtualBox**: 
        1. Run `vagrant up`
    - **Hyper-V**:
        1. Run `vagrant up --provider hyperv` (will require administrator priveleges for HyperV)
        2. Login with your system user credentials when prompted to allow shared drives.
2. Navigate to http://localhost:8000
3. Login with username: `admin`, and password: `islandora`
4. Go to **Modules** in the top bar, enable **Islandora Image Segmentation Solution Pack**, and save your configuration.
5. All done! You now have an Islandora instance running with this solution pack installed. 

