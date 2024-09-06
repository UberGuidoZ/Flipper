# Research into manipulating Bitlocker using BadUSB

All scripts require the current user to have administrative permissions on Windows.

bitlocker_disable.txt -- Immediately suspends Bitlocker on C:. Does not remove it

bitlocker_turn_off.txt -- Turns off Bitlocker and begins decryption of C:. When decryption is complete, all key protectors are removed.

bitlocker_encrypt_password.txt:

Uses base64 to dump a registry.pol into Windows to allow tablets (Surface Pro, etc) to have pre-boot authentication enabled. Then perform a gpupdate /force (without this, Bitlocker will fail to enable with just a password when it detects the computer is a tablet, aka "slate")

If Bitlocker is already enabled, all key protectors are removed

Enable Bitlocker on C: and skip hardware test. If Bitlocker wasn't previously enabled, it gets turned on and drive begins encrypting

Remove the default TPM key protector that gets created on new Bitlocker enabled C:

Manually set an attacker-controlled password as the only key protector.  Must be at least 8 characters

Enable the new password protector immediately.

Optional: Reboot after a pre-defined delay or immediately, clear history, close window.'

On next boot, Windows will require the attacker-controlled password to boot, and no recovery key will exist.
