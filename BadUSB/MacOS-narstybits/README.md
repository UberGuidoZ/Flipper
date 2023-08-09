 <!DOCTYPE html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="BadUSB scripts exclusively designed for Mac OS & the Flipper Zero device.">
   <meta name="author" content="NarstyBits flipperzero badusb payloads">
  <!-- Open Graph tags -->
  <meta property="og:title" content="MacOS-DuckyScripts">
  <meta property="og:description" content="BadUSB scripts exclusively designed for Mac OS & the Flipper Zero device.">
  <meta property="og:image" content="https://i.imgur.com/vKdvKVp.png">
  <meta property="og:url" content="https://github.com/narstybits/MacOS-DuckyScripts">
</head>
    
[![Image Description](https://i.imgur.com/vKdvKVp.png)](https://github.com/narstybits/MacOS-DuckyScripts)
<div align="left">
  



</div>





<h1 
 align="center"><u>This repository is EXCLUSIVE to MacOs BADUSB Scripts <img src="https://media.giphy.com/media/3oKIPic2BnoVZkRla8/giphy.gif" alt="Giphy Image" style="margin-left: 10px; width: 50px; height: auto;" align="center">
 
   <img alt="Coding" width="1473" height="18" src="https://media.giphy.com/media/9JxkPTP3alOykb8PmQ/giphy.gif">
</div>



  <div>
 <img src="https://media.giphy.com/media/tR8stUDuzhgAHrr2i8/giphy-downsized-large.gif" alt="Giphy Image" style="width: 300px; height: auto;">
</div>

<div align="center">

## 🔹 They are Plug & Play 🔹

</div>

## Acknowledgements

A big shoutout and huge thanks to these amazing contributors:
- [47LeCoste](https://github.com/grugnoymeme)
- [ClaraCrazy](https://github.com/ClaraCrazy)
- [0iphor13](https://github.com/0iphor13)
- [Hak5](https://github.com/hak5)
- [I-Am-Jakoby](https://github.com/I-Am-Jakoby)
- [UberGuidoZ](https://github.com/UberGuidoZ)

## Xremote (Cross Remote)

Check out the Xremote project by [leedave](https://github.com/leedave) to chain Infrared and SubGHz together! 📱

- [XRemote (BETA)](https://github.com/leedave/Leeds-Flipper-Zero-Fap-Files/tree/main/Xtreme)

## Evil Portal 

Check out the 😈 🇪 🇻 🇮 🇱 🇵 🇴 🇷 🇹 🇦 🇱 😈 project by [bigbrodude6119](https://github.com/bigbrodude6119) An evil captive portal Wi-Fi access point using the Flipper Zero and Wi-Fi dev board! 

 For Educational Purposes Only! (This project should teach you why its important to NEVER use an Unauthorized Accesspoint)

- [Evil Portal (BETA)](https://github.com/bigbrodude6119/flipper-zero-evil-portal)

## MacOs Flipper Zero Collection

- [![MacOs Flipper Zero Collection](https://img.shields.io/badge/-MacOs%20Flipper%20Zero%20Collection-brightgreen)](https://github.com/narstybits/MacOS-DuckyScripts)
- [![NEWEST FLIPPER Zero SCRIPT](https://img.shields.io/badge/-NEWEST%20FLIPPER%20Zero%20SCRIPT-blue)](https://github.com/narstybits/MacOS-DuckyScripts/blob/main/Goodusb/Git%20Repository%20Updater.txt)

## How 2 Flipper Guide

If you're a visual learner like me, check out this detailed tutorial by [wrenchathome](https://github.com/wrenchathome):

- [How 2 Flipper Guide](https://github.com/wrenchathome/flipperfiles/blob/main/_Guides/How2Flipper.pdf)

<div align="center">
  <h2>🔹Step by Step Directions to Get You Up and Running🔹</h2>
  
</div>

<div style="border: 2px solid #000000; padding: 10px;">
  <h3 style="font-size: 32px; margin: 0;">🔹 First, Always Read the Remarks</h3>
  <img alt="Coding" width="320" height="5" src="https://media.giphy.com/media/9JxkPTP3alOykb8PmQ/giphy.gif">
  <p>When working with the scripts, make sure to carefully read the REM (remark) lines. REMs explain the purpose of each line or provide instructions to the user. It's crucial to always read and understand the REMs!</p>
  
  <h4>🔹The VendorID and ProductID Combination</h4>
  <img alt="Coding" width="320" height="5" src="https://media.giphy.com/media/9JxkPTP3alOykb8PmQ/giphy.gif">
  <pre><code>'ID 05ac:021e Apple:Keyboard'</code></pre>
  <p>This code helps the operating system identify the specific device (in this case, the Flipper Zero) and load the appropriate drivers or configurations, such as for an Apple keyboard.</p>
  
  <h4>🔹The 'ignorespace' Option</h4>
  <img alt="Coding" width="320" height="5" src="https://media.giphy.com/media/9JxkPTP3alOykb8PmQ/giphy.gif">
  <pre><code>'echo -e "export HISTCONTROL=ignorespace\nunset HISTFILE" >> ~/.bashrc && source ~/.bashrc && exec bash'</code></pre>
  <p>This code enables the 'ignorespace' option for the command history, preventing commands with a leading space from being stored in the shell history. Special thanks to <a href='https://github.com/FalsePhilosopher'>FalsePhilosopher</a>
 for helping create this string!</p>
  
  <h4>🔹Removing Specific Commands from History</h4>
  <img alt="Coding" width="320" height="5" src="https://media.giphy.com/media/9JxkPTP3alOykb8PmQ/giphy.gif">
  <pre><code>history -d $(history | tail -n 2 | head -n 1 | awk '{ print $1 }')</code></pre>
  <p>This code removes the specific command from the history, ensuring minimal traces are left behind and helping maintain discretion and privacy.</p>
</div>

# 🔹 Step by Step Directions 

1️⃣ **Download the Files**: Start by downloading the necessary files from the repository to your desktop.

2️⃣ **Prepare Flipper**: Plug your Flipper into your computer and navigate to the SD card.

3️⃣ **Transfer Files**: Open the File Manager and locate the BADUSB folder on the SD card. Drag and drop the downloaded files into the BADUSB folder.

4️⃣ **Finalize**: Close the Flipper desktop app and ensure the files have been successfully transferred.

5️⃣ **Ready to Go!**: Your Flipper is now ready to deploy the BADUSB DuckyScripts and unleash its power! Enjoy the experience and have fun exploring the possibilities! 🚀



  <h1 
 align="center"><u>🔹 Support Narstybits 🔹
 </div>



  <div>
   
   
<img alt="Coding" width="1483" height="15" src="https://media.giphy.com/media/9JxkPTP3alOykb8PmQ/giphy.gif">
<h5 align="center"><em>If my work has brought a smile to your face or sparked joy in your heart, I wouldn't say no to a little digital love.</em></h5>

   <p></p>
    <p></p>
   
 <p align="center">
  <a href="https://www.blockonomics.co/pay-url/5106312c7ce343bb">
    <img src="https://www.opennode.com/blog/wp-content/uploads/2020/04/donate-button-small-1.png" alt="Donate with Bitcoin" width="290">
<div align="center">
  <img alt="Coding" width="1483" height="15" src="https://media.giphy.com/media/9JxkPTP3alOykb8PmQ/giphy.gif"></div>


</p>
<pre
          ⚠️
         ⚠️⚠️⚠️
        ⚠️⚠️⚠️⚠️⚠️
      ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️
</pre>

<div 
<h3 align="center"><u><a href="https://github.com/narstybits/MacOS-DuckyScripts/blob/main/Warning%20%20Readme.md">⚠️ IMPORTANT DISCLAIMER ⚠️</a></u></h1>
<h5>
<p align="center">❗️ Please exercise caution and responsibility when using the scripts and tools in the "executions," "obscurity," "pranks," "GOODUSB," and "Recon" folders. These scripts are intended for educational and ethical purposes such as penetration testing, security testing, and network testing. However, it is important to note that unauthorized or malicious use of these tools can be illegal and may lead to severe consequences. </h3>

<p align="center">
❗️ It is your responsibility to ensure that you have appropriate authorization, permissions, and legal rights to perform any security testing activities on computer systems or networks. Always respect the law, adhere to ethical guidelines, and obtain proper consent before conducting any tests or assessments.
</p>

<p align="center">
❗️ Remember, it is crucial to prioritize legal and ethical practices, protect user privacy, and use these tools responsibly to improve security and protect against real-world threats.
</p></div>

