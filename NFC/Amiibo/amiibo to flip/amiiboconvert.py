"""
amiiboconvert.py
3/12/2022
Modified Amiibo Flipper Conversion Code

Original Code by Friendartiste
Modified by Lamp
Modified again by VapidAnt
Modified and commented by bjschafer

Execute with python amiiboconvert -h to see options
"""
import argparse
import logging
import os
import pathlib
from typing import Tuple


def write_output(name: str, assemble: str, out_dir: str):
    """
    Handles writing the converted file
    :param name: The base filename - e.g. for Foo.bin, Foo
    :param assemble: The converted flipper-compatible contents
    :param out_dir: The directory to place Foo.nfc in
    """
    with open(os.path.join(out_dir, f"{name}.nfc"), "wt") as f:
        f.write(assemble)


def convert(contents: bytes) -> Tuple[str, int]:
    """
    Convert from bytes into the Page-based format expected by flipper

    Each "Page" is 4 bytes hex, notated like:
        Page 0: DE AD BE EF

    To process, we grab one byte at a time, turn it into a hex string, and store it in `page`.
    When page is "full" (has 4 bytes in it), we flush it to the buffer.

    When all's said and done, buffer contains text ready for writing to the end of a .nfc file.

    Also tracks and returns running page number, since that's also needed.
    :param contents: byte array we're reading, from a .bin file
    :return: The full string of Pages, suitable for writing to a file
    """
    buffer = []
    page_count = 0

    page = []
    for i in range(len(contents) - 1):
        byte = contents[i : i + 1].hex()
        page.append(byte)

        if len(page) == 4:
            buffer.append(f"Page {page_count}: {' '.join(page).upper()}")
            page = []
            page_count += 1

    # we may have an unfilled page. This needs to be filled out and appended
    logging.debug(f"We have an unfilled final page: {page} with length {len(page)}")
    if len(page) > 0:
        # pad with zeroes
        for i in range(len(page) - 1, 3):
            page.append("00")
        buffer.append(f"Page {page_count}: {' '.join(page).upper()}")
        page_count += 1
    return "\n".join(buffer), page_count


def get_uid(contents: bytes) -> str:
    """
    the UID appears to be made up of the first 3 bytes, a byte is skipped, and then the next 4 bytes
    :param contents: The bytes object we're operating on
    :return: something like `23 20 41 6D 69 69 62 6F`
    """
    page = []
    for i in range(3):
        byte = contents[i : i + 1].hex()
        page.append(byte)
    for i in range(4, 8):
        byte = contents[i : i + 1].hex()
        page.append(byte)

    return " ".join(page).upper()


def assemble_code(contents: {hex}) -> str:
    """
    Convert from .bin files to Flipper text-like .nfc files

    :param contents: File contents upon which .hex() can be called
    :return: A string to be written to a file
    """
    conversion, page_count = convert(contents)

    return f"""Filetype: Flipper NFC device
Version: 2
# Nfc device type can be UID, Mifare Ultralight, Bank card
Device type: NTAG215
# UID, ATQA and SAK are common for all formats
UID: {get_uid(contents)}
ATQA: 44 00
SAK: 00
# Mifare Ultralight specific data
Signature: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Mifare version: 00 04 04 02 01 00 11 03
Counter 0: 0
Tearing 0: 00
Counter 1: 0
Tearing 1: 00
Counter 2: 0
Tearing 2: 00
Pages total: {page_count}
{conversion}
"""


def convert_file(input_path: str, output_path: str):
    """
    Handles reading, converting, and writing a single file
    :param input_path: The full path to the .bin file
    :param output_path: The base directory to output to
    """
    input_extension = os.path.splitext(input_path)[1]
    if input_extension == ".bin":
        logging.info(f"Writing: {input_path}")
        with open(input_path, "rb") as file:
            contents = file.read()
            name = os.path.split(input_path)[1]
            write_output(name.split(".bin")[0], assemble_code(contents), output_path)

    elif input_extension == ".nfc":
        logging.warning(f"Seems like {input_path} may already be Flipper-compatible!")
    else:
        logging.info(f"{input_path} doesn't seem like a relevant file, skipping")


def process(path: str, output_path: str):
    """
    Process an input file, or walk through an input directory and process every matching .bin file therein
    :param path: Path to a single file or a directory containing one or more .bin files
    :param output_path: The base directory to output to
    """
    if os.path.isfile(path):
        convert_file(path, output_path)
    else:
        for filename in os.listdir(path):
            new_path = os.path.join(path, filename)
            logging.debug(f"Current file: {filename}; Current path: {new_path}")

            if os.path.isfile(path):
                convert_file(path, output_path)
            else:
                logging.debug(f"Recursing into: {new_path}")
                process(new_path, output_path)


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-i",
        "--input-path",
        required=True,
        type=pathlib.Path,
        help="Single file or directory tree to convert",
    )
    parser.add_argument(
        "-o",
        "--output-path",
        required=False,
        type=pathlib.Path,
        help="Directory to store output in. Will be created if it doesn't exist. If not specified, the output will be "
        "stored in the same location as the original, with a '.nfc' extension.",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="count",
        default=0,
        help="Show extra info: pass -v to see what's going on, pass -vv to get useful debug info",
    )
    args = parser.parse_args()
    if args.verbose >= 2:
        # set debug
        logging.basicConfig(level=logging.DEBUG)
    elif args.verbose >= 1:
        # set info
        logging.basicConfig(level=logging.INFO)
    logging.debug(f"Parsed args into {args}")
    return args


def main():
    args = get_args()

    # single file mode
    if os.path.isfile(args.input_path):
        if not args.output_path:
            args.output_path = os.path.split(args.input_path)[0]
    # recursive directory mode
    elif os.path.isdir(args.input_path):
        if not args.output_path:
            logging.exception(
                ValueError(
                    f"{args.input_path} is a directory, but no output path given."
                )
            )
    elif not os.path.exists(args.input_path):
        logging.exception(
            FileNotFoundError(f"{args.input_path} doesn't actually exist")
        )

    logging.debug(f"Going to create output directory {args.output_path}")
    os.makedirs(args.output_path, exist_ok=True)

    logging.debug(f"input: {args.input_path}, output: {args.output_path}")
    process(args.input_path, args.output_path)


if __name__ == "__main__":
    main()
    print("----Good Execution----")
