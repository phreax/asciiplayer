#include <stdio.h>
#include <string.h>
#include "screen.h"

// asciiart character map
const unsigned char charmap[] = " .:,;+ijtfLGDKW#";
const int clen = 17;

const int buf_size = TERM_W*TERM_H+1;
unsigned char sc_buf[TERM_W*TERM_H+1];

// copy one line
void convert2ascii(uint8_t *dst, uint8_t *src, int size) {
  int i;
  float c;
  uint8_t *_dst,*_src;
  _dst = dst; _src = src; // save pointer
  for(i=0;i<size;i++,_src+=3,_dst++) {
     // to rgb2gray
     c = 0.2126*_src[0]+0.7152*_src[1]+0.0722*_src[2];
     // stretch [0-255] -> [0-16]
     c = (clen-1)*c/255.0;
     *_dst = charmap[(int)c];
  }
}

void ascii_art(AVFrame *pFrame) {
  int y;
  uint8_t *lineptr;
  memset(sc_buf,32,buf_size);
  for(y=0; y<TERM_H; y++) {
    lineptr = pFrame->data[0]+y*pFrame->linesize[0];
    convert2ascii(sc_buf+TERM_W*y,lineptr,TERM_W);
  }
  display_buffer();

}

void display_buffer() {
  int i;
  unsigned char c;
  printf("\x1b[H");
    for(i=1;i<buf_size;i++) {
        c = i%TERM_W == 0 ? 10 : sc_buf[i];
        putchar(c);
    }
}
