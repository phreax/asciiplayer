/**
 *  This file is part of asciiplayer.
 *
 *  Asciiplayer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>

/* terminal resulution */
#define TERM_W 128
#define TERM_H 32

/**
 * convert one line of length sie from src to dst
 *  - rgb -> ascii
 *  - assumes src to be an buffer of consecutive rgb24 triples
 */
void convert2ascii(uint8_t *dst, uint8_t *src, int size);

/**
 * print out ascii buffer to standard out 
 */
void display_buffer();

/** main function to display an AVFrame 
 * in ascii art.
 * Assumes frame to be resized to terminal size
 */
void ascii_art(AVFrame *frame);

/* init stuff */
void init_screen(int w, int h);
#endif
