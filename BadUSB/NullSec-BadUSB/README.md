# NullSec BadUSB Collection

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20Mac-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Payloads-25+-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Author-bad--antics-orange?style=for-the-badge" />
</p>

A comprehensive collection of BadUSB payloads for the Flipper Zero, designed for penetration testing and security research.

## 📁 Payload Categories

### 🔍 Reconnaissance
| Payload | Target | Description |
|---------|--------|-------------|
| `01_SystemRecon.txt` | Windows | Enumerate system info, users, processes, network |
| `08_NetworkScan.txt` | Windows | Discover network hosts and services |
| `21_LinuxRecon.txt` | Linux | Linux system reconnaissance |
| `23_MacOSRecon.txt` | macOS | macOS system enumeration |

### 🔑 Credential Extraction
| Payload | Target | Description |
|---------|--------|-------------|
| `02_WiFiStealer.txt` | Windows | Extract saved WiFi passwords |
| `05_CredDump.txt` | Windows | Dump cached credentials |
| `06_BrowserData.txt` | Windows | Extract browser history, cookies, passwords |
| `09_SAMDump.txt` | Windows | Dump SAM database (requires admin) |

### 🐚 Remote Access
| Payload | Target | Description |
|---------|--------|-------------|
| `03_ReverseShell.txt` | Windows | PowerShell reverse shell |
| `22_LinuxReverseShell.txt` | Linux | Bash reverse shell |
| `24_DownloadExecute.txt` | Windows | Download and execute remote payload |

### 🛡️ Defense Evasion
| Payload | Target | Description |
|---------|--------|-------------|
| `04_DisableDefender.txt` | Windows | Disable Windows Defender |
| `10_Persistence.txt` | Windows | Registry run key persistence |
| `25_CreateAdmin.txt` | Windows | Create hidden admin account |

### �� Data Exfiltration
| Payload | Target | Description |
|---------|--------|-------------|
| `07_Keylogger.txt` | Windows | Install PowerShell keylogger |
| `12_WebcamSnap.txt` | Windows | Capture webcam image |
| `13_ClipboardStealer.txt` | Windows | Monitor clipboard contents |
| `14_ScreenCapture.txt` | Windows | Take screenshot |
| `15_USBExfil.txt` | Windows | Copy documents to USB |

### 😈 Pranks (Harmless)
| Payload | Target | Description |
|---------|--------|-------------|
| `11_FakeUpdate.txt` | Windows | Fake Windows update screen |
| `16_RickRoll.txt` | Windows | Classic RickRoll |
| `17_WallpaperPrank.txt` | Windows | Change wallpaper |
| `18_VoicePrank.txt` | Windows | Text-to-speech prank |
| `19_DisableMouse.txt` | Windows | Temporarily disable mouse |
| `20_InvertScreen.txt` | Windows | Invert display colors |

## ⚙️ Usage

1. Copy desired payload to your Flipper Zero's `badusb` folder
2. Navigate to BadUSB on your Flipper
3. Select the payload
4. Connect to target device
5. Execute

## ⚠️ Disclaimer

These payloads are provided for **educational and authorized security testing purposes only**. 
- Only use on systems you own or have explicit permission to test
- Never use for malicious purposes
- The author is not responsible for misuse

## 🔗 Links

- **Full Suite**: [nullsec-flipper-suite](https://github.com/bad-antics/nullsec-flipper-suite)
- **Author**: [bad-antics](https://github.com/bad-antics)
- **Discord**: discord.gg/killers

## 📜 License

MIT License - Use responsibly!
