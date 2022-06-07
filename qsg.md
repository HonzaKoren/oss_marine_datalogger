# Quick Start Guide

## Preparation and tools you'll need
1. SSH client
    1. Linux has SSH client usually as a part of default installation, no pain
    1. Macos has some variety of options, [try this guide](https://www.makeuseof.com/best-ssh-clients-mac/)
    1. Windows has an excellent free SSH terminal emulation called [Putty](https://www.ssh.com/academy/ssh/putty/windows)

## Marine Logger setup
1. Take your Raspberry and make an initial setup as is [very well documented](external_links.md)
1. Connect to your Raspberry as pi user and:
    1. Download latest stable copy of Marine Logger code
    1. Run setup
    1. Connect to Marine Logger web UI (automatically generated admin password is on your SSH terminal screen)
    
### Download latest stable copy of Marine Logger code
'''bash
cd ~
git clone https://github.com/HonzaKoren/oss_marine_datalogger.git
'''

### Run setup
'''bash
cd ~/oss_marine_datalogger
./install.sh
'''

### Connect to Marine Logger web UI
Open https://<your_datalogger_IP_address_used_for_SSH_already>/
Default superuser of Marine Datalogger is <em>admin</em>, initial password for that user was generated and shown in the previous step.
If you have lost for any reason the secure password, simply re-run previous step, the installation will be performed again and password re-generated. All your settings made till then will be lost as well. Consider this step a "factory reset".
