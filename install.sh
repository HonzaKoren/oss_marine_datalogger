#!/bin/bash

# Check locales and add en_US to default set
locales=$(grep -v -e "^#" -e "^$" -e "en_GB.UTF-8" /etc/locale.gen | wc -l)

# If there is only a default locale set, add en_US.UTF-8
if [ $locales -eq 0 ]
then
    echo "en_US.UTF-8 UTF-8" | sudo tee -a /etc/locale.gen
    echo "LANG=en_US.UTF-8" | sudo tee -a /etc/default/locale
fi

# Re-generate locales
sudo locale-gen

# Just to be sure, make a full update
APT="sudo $(which apt)"

$APT update
$APT upgrade -y
$APT full-upgrade -y
$APT autoremove -y

# Necessary and usefull tools for running full stack deploy Ansible playbook

$APT install -y ansible aptitude

cd ansible
ansible-playbook prepare-host.yaml
