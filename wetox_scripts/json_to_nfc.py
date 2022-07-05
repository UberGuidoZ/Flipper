# Script to convert proxmark JSON output to Flipper Mifare Classic NFC File Format

import json
import argparse


def add_spaces_to_hex(in_str):
  # converts something like "AB00BA" to "AB 00 BA"
  out_str = ""
  for i in range(0, len(in_str), 2):
    out_str += in_str[i:i+2] + " "
  return out_str.strip()

def guess_mifare_size_by_sak(SAK):
  if SAK == "18":
    return "4K"
  elif SAK == "08":
    return "1K"
  else:
    return "idk"

parser = argparse.ArgumentParser(description='Convert proxmark JSON output to Flipper Mifare classic NFC File Format')
parser.add_argument('-i', '--input', help='Input file', required=True)
parser.add_argument('-o', '--output', help='Output file', required=True)
parser.add_argument('-v', '--verbose', help='Verbose output', action='store_true')
parser.add_argument('-k', '--keys-only', help='Only output keys', action='store_true')
args = parser.parse_args()

with open(args.input, 'r') as f:
    data = json.load(f)

if args.keys_only:
  # just output the keys to a file
  with open(args.output, 'w') as f:
    for sector in data['SectorKeys']:
      f.write(data)

else:
  with open(args.output, 'w') as f:
      f.write('Filetype: Flipper NFC device\n')
      f.write('Version: 2\n')
      f.write('# Nfc device type can be UID, Mifare Ultralight, Mifare Classic, Bank card\n')
      f.write('Device type: Mifare Classic\n')
      f.write('# UID, ATQA and SAK are common for all formats\n')
      f.write('UID: '  + add_spaces_to_hex(data['Card']['UID'])  + '\n')
      f.write('ATQA: ' + add_spaces_to_hex(data['Card']['ATQA']) + '\n')
      f.write('SAK: '  + add_spaces_to_hex(data['Card']['SAK'])  + '\n')
      f.write('# Mifare Classic specific data\n')
      if guess_mifare_size_by_sak(data['Card']['SAK']) == "4K":
        f.write('Mifare Classic type: 4K\n')
      elif guess_mifare_size_by_sak(data['Card']['SAK']) == "1K":
        f.write('Mifare Classic type: 1K\n')
      f.write('# Mifare Classic blocks\n')
      for block in range(0, len(data['blocks'])):
        f.write('Block ' + str(block) + ': ' + add_spaces_to_hex(data['blocks'][str(block)]) + '\n')