At 2015’s KansasFest [http://kansasfest.org], I had the idea to convert Apple II disk images, of the kind used to run in an emulator, to actual images, of the kind you can view with your eyeballs. I was using the DSK format, which is one of several common image formats.

The resulting images were illuminating and pretty, but flawed. DSK, being abstracted from the original bits, no longer represents actual bytes on disk. Plus, it doesn’t work reliably for copy-protected disks. I wanted to ‘see’ the bits. The closest representation we can get is what’s called EDD, named for the Essential Disk Duplicator. I won’t go into the details, but it’s a hardware method of imaging a floppy that captures bytes before they are interpreted by the Apple II, and is more of a one-to-one to bytes on disk.

So, I recently polished off the original script, dug a little more into imagemagick, and learned about the EDD format itself.

The resulting images are much more detailed and nuanced. A few samples are included in the Images directory.

And here’s all it takes. One line of imagemagick command line arguments:

        filename=EDD-FILENAME;convert -depth 8 -size 16384x141 gray:"$filename" -crop '6554x141!+0+0' -scale 100%x800% -flip -virtual-pixel 'transparent' +distort Polar '1024 256' "$filename".png

I’ll break it down:

        filename=EDD-FILENAME;

your EDD file name goes here.

        convert -depth 8 -size 16384×141 gray:”$filename”

“convert” is the imagemagick command. This tells it to create an 8 bits-per-pixel greyscale image, based on the bytes in the EDD file defined above. The size is based on the EDD format, which stores 2.5 copies of each disk track in a bistream 16384 bytes long. There are 141 quarter-tracks in the EDD file, so that is the vertical resolution.

        -crop ‘6554×141!+0+0’

crops the resulting image down to 6554 bytes wide (1x track instead of 2.5) starting at the top left corner.

        -scale 100%x800% -flip

stretches the vertical resolution by 800% and flips it vertically. This is optional, but if you stop here, the resulting image is easier to “read” the tracks.

The resulting image at this step looks like this.

        -virtual-pixel ‘transparent’ +distort Polar ‘1024 256’

distorts the rectangular image around a circle 1024 pixels in diameter, with a hole 256 pixels in the middle. This roughly represents the floppy disk media. The background, instead of consisting of stretched out pixels from the edge, is transparent.

        “$filename”.png

Finally, write the result to a PNG file with the same filename as your EDD, with PNG extension.

*Caveats:*

The EDD file contains “about” 2.5 rotations of each track. The variability of hardware and each disk means the track length in bytes isn’t precisely 6554 bytes. In some examples, you can see the “seam” where the track length isn’t exactly 1.

At 8 bits-per-pixel, the image is more visually appealing than a true representation of the bits. Fiddling with the values can get you a 1 bit-per-pixel version, as well as higher resolution. The size of the above images is chosen for portability, not any specific function or utility.

The EDD files I’m using are from Project Applesauce by John Keoni Morris. His hardware, unlike the EDD card and its clones, synchronizes the tracks of the resulting EDD, and so are going to have better/different properties to other typical EDD files found online.
