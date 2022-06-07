#!/bin/bash

# Just to be sure, make a full update
APT="sudo $(which apt-get)"

$APT update
$APT upgrade -y
$APT full-upgrade -y
$APT dist-upgrade -y
$APT autoremove -y

# Necessary and usefull tools for running full stack deploy Ansible playbook

$APT install ansible

cd ansible
ansible-playbook install.yaml
