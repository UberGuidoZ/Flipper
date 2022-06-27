#!/usr/bin/python

# flipnoise.py
#
# Converts Renoise song files (.xrns) to Flipper Zero song files (.fmf). Pass
# in the .xrns file as the only argument and flipnoise will unzip it, parse
# the Song.xml file within and generate a .fmf file you can place on your
# device with qFlipper.
#
# flipnoise will read the song name, artist name and BPM from the Renoise file.
# Empty lines are filled in with the last note played. The intention here is to
# primarily make chiptune/keygen-sounding music at a very high BPM (e.g., 500).
#
# Note that Flipper Zero only has a single sound channel, so only make one track
# in your Renoise song.
#
# Juan Irming
# https://amplitudeproblem.com/

import os
from os.path import exists
import shutil
import sys
import uuid
import xml.etree.ElementTree as ET
import zipfile

# ------------------------------------------------------------------------------
# Constants

SONG_FILENAME = "Song.xml"

RENOISE_MAGIC = "RenoiseSong"

APP_NAME = "flipnoise"
APP_VERSION = 1.0

# ------------------------------------------------------------------------------


def invalid_file_exit(filename):
	sys.exit(filename + " is not a valid Renoise song")


# ------------------------------------------------------------------------------


def main():
	if len(sys.argv) < 2:
		sys.exit("Usage: " + APP_NAME + " [filename.xrns]")
	else:
		filename = sys.argv[1]

	if not exists(filename):
		invalid_file_exit(filename)

	tmp_dir = "." + APP_NAME + "_tmp/"

	if exists(tmp_dir):
		sys.exit("Temporary directory " + tmp_dir + " already exists")

	os.mkdir(tmp_dir)

	shutil.copy(filename, tmp_dir)

	with zipfile.ZipFile(tmp_dir + filename, 'r') as zip_handle:
		zip_handle.extractall(tmp_dir)

	try:
		renoise = ET.parse(tmp_dir + SONG_FILENAME)
	except:
		invalid_file_exit(filename)

	print(filename)

	renoise_root = renoise.getroot()

	if not renoise_root.tag == RENOISE_MAGIC:
		invalid_file_exit(filename)

	song_name = renoise_root.find("./GlobalSongData/SongName").text
	print("Song name:", song_name)

	artist = renoise_root.find("./GlobalSongData/Artist").text
	print("Artist:", artist)

	bpm = renoise_root.find("./GlobalSongData/BeatsPerMin").text
	print("BPM:", bpm)

	octave = renoise_root.find("./GlobalSongData/Octave").text
	print("Octave:", octave)

	notes = "Notes: "

	for pattern in renoise_root.findall("./PatternPool/Patterns/Pattern"):
		last_index = 0
		last_note = ""

		for line in pattern.findall("Tracks/PatternTrack/Lines/Line"):
			index = int(line.attrib.get("index"))
			index_delta = index - last_index
			if index_delta == 0:
				index_delta = 1

			note = line.find("NoteColumns/NoteColumn/Note").text
			note = note.replace("-", "")

			for i in range(0, index_delta - 1):
				notes += note + ", "

			notes += note + ", "

			last_index = index
			last_note = note

	notes = notes.rstrip(", ")

	print(notes)

	flipper_filename = artist + "-" + song_name + "." + str(uuid.uuid4()) + ".fmf"
	with open(flipper_filename, "w") as file:
		file.write("Filetype: Flipper Music Format\n\n")
		file.write("Version: 0\n\n")
		file.write("BPM: " + bpm + "\n\n")
		file.write("Duration: 16\n\n")
		file.write("Octave: 5\n\n")
		file.write(notes)

	print("Saved", flipper_filename)

	shutil.rmtree(tmp_dir)


# ------------------------------------------------------------------------------

if __name__ == "__main__":
	main()
