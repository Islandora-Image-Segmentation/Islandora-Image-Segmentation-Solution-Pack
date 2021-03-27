# -*- mode: ruby -*-
# vi: set ft=ruby :

$name = "Islandora 7.x-1.x Image Segmentation Development VM"
$cpus   = ENV.fetch("ISLANDORA_VAGRANT_CPUS", "2")
$memory = ENV.fetch("ISLANDORA_VAGRANT_MEMORY", "3000")
$hostname = ENV.fetch("ISLANDORA_VAGRANT_HOSTNAME", "islandora")
$forward = ENV.fetch("ISLANDORA_VAGRANT_FORWARD", "TRUE")
$multiple_vms  = ENV.fetch("ISLANDORA_VAGRANT_MULTIPLE_ISLANDORAS", "FALSE")
$newspaper_navigator_host  = ENV.fetch("NEWSPAPER_NAVIGATOR_HOST", "10.0.2.2")
$newspaper_navigator_port  = ENV.fetch("NEWSPAPER_NAVIGATOR_PORT", "8008")

Vagrant.configure("2") do |config|

  config.vm.provider "virtualbox" do |v|
    v.name = $name
    v.customize ["modifyvm", :id, "--memory", $memory]
    v.customize ["modifyvm", :id, "--cpus", $cpus]
  end

  config.vm.provider "hyperv" do |h|
    h.vmname = $name
    h.cpus = $cpus
    h.memory = $memory
  end

  config.vm.box = "spaceduck/IslandoraNewsDev"

  # This checks and updates VirtualBox Guest Additions.
   if Vagrant.has_plugin?("vagrant-vbguest")
    config.vbguest.auto_update = true
    config.vbguest.no_remote = false
   end

  unless  $forward.eql? "FALSE"
    config.vm.network :forwarded_port, guest: 8080, host: 8080, id: 'Tomcat', auto_correct: true
    config.vm.network :forwarded_port, guest: 3306, host: 3306, id: 'MySQL', auto_correct: true
    config.vm.network :forwarded_port, guest: 8000, host: 8000, id: 'Apache', auto_correct: true
    config.vm.network :forwarded_port, guest: 22, host: 2222, id: "ssh", auto_correct: true
  end

  config.vm.synced_folder ".", "/var/www/drupal/sites/all/modules/Islandora-Image-Segementation-Solution-Pack"

  # Moves Islandora 7.x to ipaddress instead of localhost.
  unless  $multiple_vms.eql? "FALSE"
     unless Vagrant.has_plugin?("vagrant-hostsupdater")
       raise 'vagrant-hostsupdater is not installed!'
     end
     config.vm.network :private_network, ip: "33.33.33.10"

   end

  config.vm.provision :shell, inline: "drush --root=/var/www/drupal/ en -u 1 -y composer_manager || echo '##### PLEASE IGNORE THE ERRORS #####' ", :privileged => false
  config.vm.provision :shell, inline: "drush --root=/var/www/drupal/ en -u 1 -y image_segmentation", :privileged => false
  config.vm.provision :shell, inline: "drush --root=/var/www/drupal/ en -u 1 -y background_batch", :privileged => false
  config.vm.provision :shell, inline: "drush --root=/var/www/drupal/ vset api_host $1", :args => $newspaper_navigator_host, :privileged => false
  config.vm.provision :shell, inline: "drush --root=/var/www/drupal/ vset api_port $1", :args => $newspaper_navigator_port, :privileged => false
  config.vm.provision :shell, inline: "drush --root=/var/www/drupal/ dl drush_extras", :privileged => false
  config.vm.provision :shell, inline: "drush --root=/var/www/drupal/ block-configure  --module image_segmentation --delta image_segment_list --region sidebar_first", :privileged => false

  unless  $multiple_vms.eql? "FALSE"
    # Fires last to modify one last change.
    config.vm.provision "this",
    type: "shell",
    preserve_order: true,
    inline: "cd /var/www/drupal && /usr/bin/drush vset islandora_paged_content_djatoka_url 'http://33.33.33.10:8080/adore-djatoka'"
  end
end
