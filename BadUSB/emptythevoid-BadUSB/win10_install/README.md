Ducky scripts for the Flipper to automate the GUI for installing Windows 10 Pro, 22H2.  Emphasis is on automation, not efficiency (longer delays than necessary)

Part 1 handles the intial setup, including removing any existing partitions and letting Windows create a new scheme.

Part 2 Handles the rest of the setup. It is configured to *always* click "I don't have internet", as trying to get Ducky Script to select the correct Wifi SSID from the list is unpredictable. NOTE: you have to choose in the script which version to run - the one where you have no wifi at all, or the one where you have wifi and at least one access point will show up when it scans for access points.  This part also handles creation of the initial local user account. Make sure to fill in the STRINGs that contain the information required: User, Password, and three security answers (the script assumes these aren't important so it always selects the first one from the drop down - adjust as you desire)

Notes about timing: The script implicitly assumes that your computer is relatively fast with an SSD/NVME drive. If the timing is wrong, most commands will accidentally turn on voice assistant. That said, take care when the account is created. If it goes too quickly, it's possible that your password could be added as a user account!

TODO:

-Need to add optional code block for when Ethernet and/or Wifi is available (currently it operates under the assumption that internet isn't available)

-Probably need to set a standard longer delay for part 2, since so many Delay 100 is needed
