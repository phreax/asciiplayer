ASCII Art player
================

This is a sketched project to demonstrate the FFMpeg API for playing videos.
It just consumes an arbitrary video file and plays it on the console.
I recommend to use `xterm` or `rxvt` as it's rendering is really fast, 
compared to other terminal emulators like the gnome-terminal.

Dependencies
------------

- ffmpeg (latest git version)
    * libavcodec
    * libavformat
    * libswscale

You will probably need to compile the latest version of ffmpeg
yourself. For Ubuntu you can find a good guide [here][build].

Be sure you build ffmpeg with `--enable-shared` as otherwise you will
not be able to link to the ffmpeg libraries.
Also will also have to add `/usr/local/lib` to your library path, for e.g.

    export LD_LIBRARY_PATH=/usr/local/lib

or by adding `/usr/local/lib` to your `/etc/ld.so.conf` and run
`ldconfig`

Build and play
--------------

    make
    ./asciiplayer [-g termsize] <video>

Where `termsize` gives the number of columns and lines of your terminal.
The default size is 128x32 which works great for 16:9 videos. Make sure
you have you terminal scaled to at least that resolution, or even bigger
to see the video info on the bottom.

Links
-----

+ A really well written tutorial on the FFMpeg API can be found [here][api]. Though it's sligtly out of date, it shows in a very detailed fashion, how to write a fully functional video player in under 1000 lines of C code, using FFMpeg and SDL. 

+ The official [FFMpeg Documention][doc]

[build]: http://ubuntuforums.org/showthread.php?t=786095 "build ffmpeg"
[api]: http://dranger.com/ffmpeg/ "dranger"
[doc]: http://ffmpeg.org/documentation.html "doc"

