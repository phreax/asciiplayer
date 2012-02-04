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

#include <stdio.h>
#include <string.h>
#include "screen.h"

/* asciiart character map */
//const unsigned char charmap[] = " .:,;+ijtfLGDKW#";
const unsigned char charmap[] = ".:,;+ijtfLGDKW#";
const unsigned char *mods[]={"1;30m","0;37m","1;37m"};
const unsigned char reset[]="\033[0m";
int clen;
int buf_size;
int w,h;
/* screen buffer */
unsigned char *sc_buf;

/* is initilized */
int initialized = 0;

void init_screen(int _w, int _h) {
   w = _w; h = _h;
   buf_size = w*h+1;
   clen = 16*3+1;
   sc_buf = malloc(buf_size*sizeof(unsigned char));

   printf("buf_size = %d\n",buf_size);
   printf("w = %d, h = %d\n",w,h);
   initialized = 1;
}

/* copy one line */
/*void convert2ascii(uint8_t *dst, uint8_t *src, int size) {
  int i;
  float lum;
  uint8_t *_dst,*_src;
  _dst = dst; _src = src; 
  for(i=0;i<size;i++,_src+=3,_dst++) {
    lum = 0.2126*_src[0]+0.7152*_src[1]+0.0722*_src[2];
    lum = (clen-1)*lum/255.0;
    *_dst = charmap[(int)lum];
  }
}
*/
/* copy one line */
void convert2ascii(uint8_t *dst, uint8_t *src, int size) {
  int i;
  float lum;
  uint8_t *_dst,*_src;
  _dst = dst; _src = src; /* save pointer */
  for(i=0;i<size;i++,_src+=3,_dst++) {
    /* to rgb2gray (luminance) */
    lum = 0.2126*_src[0]+0.7152*_src[1]+0.0722*_src[2];
    /* stretch [0-255] -> [0-16] */
    lum = (clen-1)*lum/255.0;
    *_dst = (int)lum;
  }
}

void ascii_art(AVFrame *pFrame) {
  if(!initialized) {
    printf("warning: screen not initilized\n");
    init_screen(TERM_W,TERM_H);
  }
  int y;
  uint8_t *lineptr;
  memset(sc_buf,32,buf_size);
  /* convert line by line */
  for(y=0; y<h; y++) {
    lineptr = pFrame->data[0]+y*pFrame->linesize[0];
    convert2ascii(sc_buf+w*y,lineptr,w);
  }
  display_buffer();
}
void printfc(const char *str,int mod, int fg) {
    printf("\033[%d;%dm%s%s",mod,fg+30,str,reset);
}

void display_buffer() {
  int i;
  unsigned char x,c,m;
  printf("\x1b[H");
  for(i=1;i<buf_size;i++) {
    /* line end? new line! or pixel */
    if(i%w == 0) putchar(10);
    else {
        // decode map
        int x = sc_buf[i];
        if(x==0) {putchar(32); continue;}
        c = x/3;m=x%3;
        printf("\x1b[%s%c%s",mods[m],charmap[c],reset);
    }
  }
}

