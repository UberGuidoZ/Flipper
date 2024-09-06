A collection of BadUSB scripts to assist on Linux

poweroff_linux.txt opens a terminal and runs the 'poweroff' command.

shred_nvme_live_ubuntu_mate.txt is intended to be used in conjuncton with an Ubuntu Mate LiveUSB to boot a system 
and then run sudo shred -vn1 on /dev/nvme0n1  Since it's a LiveUSB, sudo will not ask for a password. Refer to the script for further instructions.
