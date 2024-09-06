This script will export the Forticlient configuration on a Windows client. This serves purely as a proof of concept.

YOU have to decide what to do with it. As-is, it will export to the desktop of a user account named "user." YOU MUST CHANGE THIS!

Note: Exporting the configuration in a normal setup does not require admin privileges.

This export doesn't include VPN login information, but it does include the preshared key and VPN configuration.

The command that's run to perform the export looks like this, where 'password' is a password that you choose. 
You'll need it when you try to import it.  Change the output path however you want.

```"C:\Program Files\Fortinet\FortiClient\FCConfig" -m vpn -f C:\users\user\Desktop\forticonfig.conf -o export -i 1 -p password```

To import ("Restore") the configuration, run the below in an admin primpt, with the password you originally set:

```"C:\Program Files\Fortinet\FortiClient\FCConfig" -m vpn -f C:\users\user\Desktop\forticonfig.conf -o import -i 1 -p password```

As a final step, the script will clear the history from the run dialog.
