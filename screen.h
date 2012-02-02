#ifndef SCREEN_H
#define SCREEN_H

#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>


#define TERM_W 128
#define TERM_H 32

// copy one line
void convert2ascii(uint8_t *dst, uint8_t *src, int size);
void display_buffer();
void ascii_art(AVFrame *pFrame);
void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);

#endif
