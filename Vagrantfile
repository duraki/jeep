# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
    
  # Using Centos 7 
  config.vm.box = "centos/7"

  # Share current directory and sync the data
  config.vm.synced_folder "./", "/opt/jeep/", type: "rsync",
    rsync_exclude: [".git/"]


  # Configure the window for gatling to coalesce writes.
  if Vagrant.has_plugin?("vagrant-gatling-rsync")
    config.gatling.latency = 2.5
    config.gatling.time_format = "%H:%M:%S"
  end

  config.vm.provider "virtualbox" do |vb|
      vb.memory = "1024"
  end

  # Configure development environment 
  config.vm.provision :shell, path: "./data/tools"

  # AUto rsync on startup
  config.gatling.rsync_on_startup = true

end
