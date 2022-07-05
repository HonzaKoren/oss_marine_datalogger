#!/bin/bash

# Default values
my_locale=en_US.UTF-8

# Check locales and add en_US to default set
locales=$(grep -v -e "^#" -e "^$" -e "en_GB.UTF-8" /etc/locale.gen | wc -l)

# Set ff only strategy
git config --global pull.ff true

# If there is only a default locale set, add en_US.UTF-8
if [ $locales -eq 0 ]
then
    echo "$my_locale UTF-8" | sudo tee -a /etc/locale.gen
fi

# Re-generate locales
sudo locale-gen

# Update system settings
for var in LANG LANGUAGE LC_ALL LC_ADDRESS LC_NAME LC_MONETARY LC_PAPER LC_IDENTIFICATION LC_TELEPHONE LC_MEASUREMENT LC_TIME LC_ALL LC_COLLATE LC_NUMERIC
do
    sudo update-locale $var=$my_locale
done
source /etc/default/locale

# Just to be sure, make a full update
APT="sudo $(which apt)"

$APT update
$APT upgrade -y
$APT full-upgrade -y
$APT autoremove -y

# Necessary and usefull tools for running full stack deploy Ansible playbook

$APT install -y ansible aptitude python3-pip

cd ansible
ansible-playbook prepare-host.yaml

# All done, reboot to apply changes
sudo reboot
