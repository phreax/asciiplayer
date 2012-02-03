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
yourself. For Ubuntu you can find a good guide here:

[build] [build ffmpeg]

Be sure you build ffmpeg with `--enable-shared` as otherwise you will
not be able to link to the ffmpeg libraries.
Also will add `/usr/local/lib` to your library path, for e.g.

    export LD_LIBRARY_PATH=/usr/local/lib

or by adding `/usr/local/lib` to your `/etc/ld.so.conf` and run
`ldconfig`

Build and play
--------------

    make
    ./asciiplayer <video>

[build]: http://ubuntuforums.org/showthread.php?t=786095 "build ffmpeg"
