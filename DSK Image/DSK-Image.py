#	KansasFest 2015
#	HackFest Entry: Disk Images Images
#	By Charles Mangin
#	@RetroConnector

#	requires ImageMagick: http://www.imagemagick.org/
#	 and python PNG module: https://pypi.python.org/pypi/pypng 


import os,sys				# filesystem functions
import subprocess


try:
	import png # PNG image library
except:
	print("This script requires the python PNG module: https://pypi.python.org/pypi/pypng (or: pip install pypng)\n\n")
	sys.exit(0)


try:
    INPUTFILE = sys.argv[1]             # what DSK file to parse
    DSK = open(INPUTFILE, "rb")		# open the DSK file for reading
except:
    print("\n\nUsage: python [filename]\n\n [filename] should be a DSK file of 143kb.\n\n")
    sys.exit(0)				# exit on exception - no file chosen
finally:
	print("Checking " + INPUTFILE + "...\n")
	
# to do: check for 140k 5.25'' disks vs 400k/800k 3.5''	and adjust accordingly
	
if (os.path.getsize(INPUTFILE)) != 143360:		# check file size. for 5.25'', it needs to be 143k
	print("\n\nOops. Is " + INPUTFILE + " a DSK file of 143kb?\n\n")
	sys.exit(0)				# exit on exception - file is empty, etc
		
					# The point: Make a PNG image from the data on a floppy disk image.
					# 5.15'' disks have 35 tracks, each with 16 sectors of 256 bytes each, for a total of 143,360 bytes
					# so 35 lines of 4096 px.
TRACKS = 35
SECTORS = 16
BYTESPERSECTOR = 256

PNG = open("DiskImageTEMP.png", "wb")	# open a PNG for writing


					# new, empty arrays
BYTES = []
PIXELS = []

try:
    byte = DSK.read(1) 			# read a byte
    while byte !="":			# while the file still has bytes in it
        byte = DSK.read(1)	
	if len(byte) > 0: 		# the last byte, for whatever reason, is length 0. Bah.
	    BYTES.append(ord(byte))	# append the number representing the byte (0-255) to the BYTES array
	
except:
    print("\n\nOops. Is " + INPUTFILE + " a DSK file of 143kb?\n\n")
    sys.exit(0)				# exit on exception - file is empty, etc
finally:

    sys.stdout.write("\r Starting.\n\n")


    for TRACK in range(0,TRACKS,1):		# for each of the 35 tracks
	LINE=[]				# start a new line of pixels
	for SECTOR in range(0,SECTORS*BYTESPERSECTOR,1):	# write the bytes for the sectors in that track to the line array
	    offset = (SECTOR * TRACK) + SECTOR
	    LINE.append(BYTES[(SECTOR * TRACK) + SECTOR])

	sys.stdout.write("\r Track: " + str(TRACK))
	sys.stdout.flush()
	PIXELS.append(LINE)		# add the array of pixels to the array of arrays

    sys.stdout.write("\n\n\r Done.\n\n")
    sys.stdout.flush()
 
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
	subprocess.call(['convert', 'DiskImageTEMP.png', '-matte', '-virtual-pixel', 'transparent', '-resize', '1024x1024!', '-rotate', '90', '-distort', 'Polar', '512 110 512,512 -180,180', OUTPUTFILE])
					# convert the 4096x35px image to a square, rotate, then rotate around an axis.
except OSError:
    print("\n\nOops. This script requires ImageMagick: http://www.imagemagick.org/")
    sys.exit(0)				# exit on exception - needs imagemagick installed
	
subprocess.call(['open', OUTPUTFILE])		# opens the resulting image in the default image viewer (Preview.app)

