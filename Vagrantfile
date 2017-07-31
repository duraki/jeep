# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
    
  # Using Centos 7 
  config.vm.box = "centos/7"

  # Share current directory and sync the data
  config.vm.synced_folder "./", "/opt/jeep/"

  config.vm.provider "virtualbox" do |vb|
      vb.memory = "1024"
  end

  # Configure development environment 
  config.vm.provision :shell, path: "./data/tools"

end
