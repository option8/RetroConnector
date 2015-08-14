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

try:
    import png				# PNG image library
except:
    print("\n\n" + sys.argv[0] + " requires the Python PNG module\n\n   Download from https://pypi.python.org/pypi/pypng \n   Or type in shell: pip install pypng\n\n")
    sys.exit(1)				# exit on exception - no library installed

try:
    INPUTFILE = sys.argv[1]			# what DSK file to parse
    DSK = open(INPUTFILE, "rb")		# open the DSK file for reading
except:
    print("\n\nUsage: python "+ sys.argv[0] +" [filename]\n\n [filename] should be a .DSK file of 143kb.\n")
    sys.exit(1)				# exit on exception - no file chosen

TEMPFILENAME = "DiskImageTEMP.png"
PNG = open(TEMPFILENAME, "wb")	# open a PNG for writing

					# The point: Make a PNG image from the data on a floppy disk image.
					# 35 tracks, each with 16 sectors of 256 bytes each, for a total of 143,360 bytes
					# so 35 lines of 4096 px.

					# new, empty arrays
BYTES  = []
PIXELS = []

try:
    byte = DSK.read(1) 			# read a byte
    while byte != "":			# while the file still has bytes in it
        byte = DSK.read(1)	
        if len(byte) > 0: 		# the last byte, for whatever reason, is length 0. Bah.
            BYTES.append(ord(byte))	# append the number representing the byte (0-255) to the BYTES array
except:
    print("\n\nOops. Is " + INPUTFILE + " a DSK file of 143kb?\n\n")
    sys.exit(1)					# exit on exception - file is empty, etc


print("\n Starting.\n")

for TRACK in range(0,35,1):		# for each of the 35 tracks
    LINE=[]						# start a new line of pixels
    for SECTOR in range(0,4096,1):	# write the bytes for the sectors in that track to the line array
        offset = (SECTOR * TRACK) + SECTOR
        LINE.append(BYTES[(SECTOR * TRACK) + SECTOR])

    print(" Track: " + str(TRACK))
    PIXELS.append(LINE)		# add the array of pixels to the array of arrays

print("\n Done.\n")
 
					# write to the PNG file 
w = png.Writer(4096,35, greyscale=True, bitdepth=8) 
w.write(PNG, PIXELS)			# each number in the array becomes a pixel in the image. each array becomes a line.

print(" Writing to disk. Chunka-chunk-cka. Whirr...\n")


DSK.close()				# done with these files. close them.
PNG.close()


OUTPUTFILE = os.path.join(INPUTFILE + ".png")
					# set a destination file same as DSK, but with PNG extension

os.system('convert '+TEMPFILENAME+' -matte -virtual-pixel transparent -resize 1024x1024! -rotate 90 -distort Polar "512 110 512,512 -180,180" "'+OUTPUTFILE+'"')
					# Using ImageMagick, convert the 4096x35px image to a square, rotate, then rotate around an axis.
					# NOTE: Windows command line needs to have Polar coordinates enquoted with " not '
					# NOTE: Windows has a native command called convert which does something very different 
					#       from ImageMagick's convert command, so obviously this will fail if IM is not installed

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
