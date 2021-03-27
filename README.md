# Islandora-Image-Segmentation-Solution-Pack

An Islandora solution pack that enables an image segmentation functionality, where visual content such as cartoons and
advertisements can be extracted from hosted images.

# Usage

1. From the repository's directory start the vagrant machine
    - **VirtualBox**: 
        1. Run `vagrant up`
        1. Navigate to http://localhost:8000
    - **Hyper-V**:
        1. Run powershell as administer
        1. Navigate to project directory
        1. Run `vagrant up --provider hyperv`
        1. Input windows login when prompted to create shared directories
        1. Get the machines ip from the output of the startup command
        1. Navigate to http://ip:8000
1. Login with username: `admin`, and password: `islandora`
1. Go to **Modules** in the top bar.
1. Find **Islandora Image Segmentation Solution Pack** in the list, and click the checkmark
1. At the bottom of the page select **Save confiuration**

# Cancelling a batch process

1. Navigate to your drupal root folder in terminal (normally '/var/www/drupal')
1. Get the id of your batch job found at {site-url}/config/system/batch/overview (ex. http://localhost:8000/admin/config/system/batch/overview)
1. Replace {batch id} with the id obtained in step 2 and run `drush -y -u 1 sql-query "DELETE FROM batch WHERE bid IN ({batch id});"`