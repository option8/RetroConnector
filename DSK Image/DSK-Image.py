#! /usr/bin/env python

"""
	KansasFest 2015
	HackFest Entry: Disk Images Images
	By Charles Mangin
	@RetroConnector

	requires ImageMagick: http://www.imagemagick.org/
	 and python PNG module: https://pypi.python.org/pypi/pypng
"""


import os,sys				# filesystem functions
import subprocess


try:
	import png # PNG image library
except:
    print("\n\n" + sys.argv[0] + " requires the Python PNG module\n\n   Download from https://pypi.python.org/pypi/pypng \n   Or type in shell: pip install pypng\n\n")
    sys.exit(1)				# exit on exception - no library installed

try:
    INPUTFILE = sys.argv[1]			# what DSK file to parse
    DSK = open(INPUTFILE, "rb")		# open the DSK file for reading
except:
    print("\n\nUsage: python "+ sys.argv[0] +" [filename]\n\n [filename] should be a .DSK file of 143kb.\n")
    sys.exit(1)				# exit on exception - no file chosen


print("Checking " + INPUTFILE + "...\n")

# to do: check for 140k 5.25'' disks vs 400k/800k 3.5''	and adjust accordingly

if (os.path.getsize(INPUTFILE)) != 143360:		# check file size. for 5.25'', it needs to be 143k
	print("\n\nOops. Is " + INPUTFILE + " a DSK file of 143kb?\n\n")
	sys.exit(1)				# exit on exception - file is empty, etc

					# The point: Make a PNG image from the data on a floppy disk image.
					# 5.15'' disks have 35 tracks, each with 16 sectors of 256 bytes each, for a total of 143,360 bytes
					# so 35 lines of 4096 px.
TRACKS = 35
SECTORS = 16
BYTESPERSECTOR = 256

TEMPFILENAME = "DiskImageTEMP.png"
PNG = open("DiskImageTEMP.png", "wb")	# open a PNG for writing


					# new, empty arrays
BYTES  = []
PIXELS = []

try:
    byte = DSK.read(1) 			# read a byte
<<<<<<< HEAD
    while byte != "":			# while the file still has bytes in it
        byte = DSK.read(1)
        if len(byte) > 0: 		# the last byte, for whatever reason, is length 0. Bah.
            BYTES.append(ord(byte))	# append the number representing the byte (0-255) to the BYTES array
=======
    while byte !="":			# while the file still has bytes in it
        BYTES.append(ord(byte))	# append the number representing the byte (0-255) to the BYTES array
        byte = DSK.read(1)
	
>>>>>>> master
except:
    print("\n\nOops. Is " + INPUTFILE + " a DSK file of 143kb?\n\n")
    sys.exit(1)					# exit on exception - file is empty, etc


print("\n Starting.\n")

<<<<<<< HEAD
for TRACK in range(0,TRACKS,1):		# for each of the 35 tracks
	LINE=[]				# start a new line of pixels
	for SECTOR in range(0,SECTORS*BYTESPERSECTOR,1):	# write the bytes for the sectors in that track to the line array
		offset = (SECTOR * TRACK) + SECTOR
		LINE.append(BYTES[(SECTOR * TRACK) + SECTOR])
=======
    for TRACK in range(TRACKS):		# for each of the 35 tracks
	LINE=[]				# start a new line of pixels
	for SECTOR in range(SECTORS*BYTESPERSECTOR):	# write the bytes for the sectors in that track to the line array
	    LINE.append(BYTES[(SECTORS*BYTESPERSECTOR * TRACK) + SECTOR])
>>>>>>> master

	print(" Track: " + str(TRACK))
	PIXELS.append(LINE)		# add the array of pixels to the array of arrays

print("\n Done.\n")

					# write to the PNG file
w = png.Writer(SECTORS*BYTESPERSECTOR,TRACKS, greyscale=True, bitdepth=8)
w.write(PNG, PIXELS)			# each number in the array becomes a pixel in the image. each array becomes a line.

sys.stdout.write("\n\n\r Writing bytes to disk. Chunka-chunka-chunk. Whirr.\n\n")
sys.stdout.flush()

DSK.close()				# done with these files. close them.
PNG.close()


OUTPUTFILE = os.path.join(INPUTFILE + ".png")
					# set a destination file same as DSK, but with PNG extension



try:
	subprocess.call(['convert', 'DiskImageTEMP.png', '-scale', '100%x300%', '-resize', '3072x!', '(', '-size', '3072x115', 'pattern:horizontal3', '-negate', '-alpha', 'copy', '-fx', '#000', ')', '-composite', '-virtual-pixel', 'HorizontalTile', '-flip', '+distort', 'Polar', '1024 220', '-resize', '50%x50%', OUTPUTFILE])

					# convert the 4096x35px image to a square, rotate, then rotate around an axis.
except OSError:
    print("\n\nOops. This script requires ImageMagick: http://www.imagemagick.org/")
    sys.exit(1)				# exit on exception - needs imagemagick installed



if 'win32' in sys.platform:
	# Because the Windows "start" commandline command cannot take an enquoted file or pathname,
	# which is necessary if the path has spaces, we have to obtain the "short" version of
	# the file/path in the 8.3 format. There is no Python library to do this for us.
	# Said another way: Windows is broken in that enquoting a file argument after their start
	# command causes it to open a blank terminal. Boo!
	# But we do this *after* the ImageMagick convert process above, because that will take an
	# enquoted file just fine.
    from ctypes import windll, create_unicode_buffer, sizeof
    buf = create_unicode_buffer( 512 )
    if windll.kernel32.GetShortPathNameW( unicode(OUTPUTFILE), buf, sizeof(buf) ):
        OUTPUTFILE = buf.value
else:
	# Otherwise we enquote the output file becuase it may have spaces
    OUTPUTFILE = '"' + OUTPUTFILE + '"'

platform_commands = {
					'darwin' : 'open', 			# opens the resulting image in the default Mac image viewer (Preview.app)
					'linux'  : 'xdg-open', 		# opens the resulting image in the default Linux image viewer (mime-determined); Python 2: 'linux2', Python 3: 'linux'
					'win32'  : 'start'			# opens the resulting image in the default Windows image viewer (Windows Photo Viewer, or...?)
					}

# Open the .PNG in the default image viewer
for platform,command in platform_commands.items():
    if platform in sys.platform:
        os.system(command + ' ' + OUTPUTFILE)
        break
else:
    print(" Your file is ready to view: " + OUTPUTFILE)
