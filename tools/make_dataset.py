import sys
import struct
from os import path
from optparse import OptionParser
from termcolor import colored

# Usage:
# python make_dataset --source=image_dataset.csv --output=image_dataset.bin

def log(severity: str, message: str):
	color: str = "blue"
	if "error" in severity:
		color = "red"
	elif "warn" in severity:
		color = "orange"
	elif "debug" in severity:
		color = "green"
	print(colored(severity, color), message)

def debug(message: str): log("[debug]:", message)
def error(message: str): log("[error]:", message)
def info(message: str):  log("[info ]:", message)
def warn(message: str):  log("[warn ]:", message)

def parse_options():
	parser = OptionParser()
	parser.add_option("-s", "--source", metavar="file", help="Source file")
	parser.add_option("-o", "--output", metavar="file", help="Output file")

	options, args = parser.parse_args()

	if not options.source:
		error("Set the source option!")
		return None, None

	source: str = options.source
	if not options.output:
		file_name, file_extension = path.splitext(source)
		options.output = str(file_name) + '.bin'

	output: str = options.output

	debug(f"Source: {source}")
	debug(f"Output: {output}")

	return source, output

def get_separator(source: str) -> str:
	_, extension = path.splitext(source)
	if "csv" in extension:
		return ","
	if "txt" in extension:
		return " "
	return " "

def read_source(source: str) -> list[float]:
	if not path.exists(source):
		return []

	separator = get_separator(source)
	with open(source, 'r') as file:
		result = []
		for line in file:
			numbers = [float(x) for x in line.strip().split(separator)]
			result.append(numbers)
		return result

def float_to_bin(f):
	# Pack the floating-point number as a binary string
	return struct.pack('d', f)

def convert_to_binary(values):
	binary_values = []
	for line in values:
		for value in line:
			binary_values.append(float_to_bin(value))
	return binary_values


def main() -> None:
	source, output = parse_options()
	if source is None: return

	data = read_source(source)
	with open(output, 'wb') as file:
		binary_values = convert_to_binary(data)
		for line in binary_values:
			file.write(line)


if __name__ == '__main__':
	main()
