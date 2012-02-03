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

#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
#include <stdio.h>
#include <unistd.h>
#include "screen.h" 

int main(int argc, char **argv) {

  if(argc<2) {
    fprintf(stderr,"Missing arguments\n"); 
    fprintf(stderr,"Usage: %s [-g termsize] filename\n",argv[0]); 
    return -1;
  }
  char *filename = argv[1];
  char *screensize = 0;
  // screen resolution
  int tw = TERM_W;
  int th = TERM_H;

  /* parse arguments*/
  if(argc>3) {
    int i;
    if(strcmp(argv[1],"-g")==0) {
      screensize = argv[2];
      filename = argv[3];
      sscanf(screensize,"%dx%d",&tw,&th); 
    }
  }
  

  printf("before init w = %d, h = %d\n",tw,th);
  init_screen(tw,th);
  
  /* init all codecs */
  av_register_all();

  AVFormatContext *pFmtCtx;
  struct SwsContext *pSwsCtx;

  pFmtCtx = avformat_alloc_context();
  if(avformat_open_input(&pFmtCtx, filename, NULL, NULL) != 0) {
    fprintf(stderr,"Failed to open file: %s\n",argv[1]); 
    return -1;
  }

  if(avformat_find_stream_info(pFmtCtx,NULL) < 0) { 
    fprintf(stderr,"No stream found!\n"); 
    return -1;
  }

  av_dump_format(pFmtCtx, 0, filename,0);

  int i;
  int videoStream;
  AVCodecContext *pCodecCtx;

  videoStream = -1;
  for(i=0; i<pFmtCtx->nb_streams; i++) {
    if(pFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
      videoStream = i;
      break;
    }
  }

  if(videoStream == -1) {
    fprintf(stderr,"No stream found!\n"); 
    return -1;
  }

  pCodecCtx = pFmtCtx->streams[videoStream]->codec;

  /* find suitable codec */
 
  AVCodec * pCodec;
  
  pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
  if(!pCodec) {
    fprintf(stderr,"No suitable decoder found!\n"); 
    return -1;
  }

  if(avcodec_open2(pCodecCtx,pCodec,NULL)<0) {
    fprintf(stderr,"Could not open codec!\n"); 
    return -1;

  }

  AVFrame *pFrame;
  AVFrame *pPict;

  /* allocate data structs */
  pFrame = avcodec_alloc_frame();
  pPict  = avcodec_alloc_frame();

  uint8_t *buffer;
  int szPict;;
  int sw,sh;

  sw = pCodecCtx->width;
  sh = pCodecCtx->height;

  // allocate buffer of picture size
  szPict = avpicture_get_size(PIX_FMT_RGB24, sw,sh);

  buffer = (uint8_t *)av_malloc(szPict*sizeof(uint8_t));

  /* associate frame with out buffer */
  avpicture_fill( (AVPicture *)pPict,buffer,PIX_FMT_RGB24, sw, sh);

  int frameFinished;
  AVPacket packet;

  /* init scale context to scale to terminal resolution */
  pSwsCtx = sws_getContext(sw,sh,pCodecCtx->pix_fmt,tw,th,PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

  i=0;

  /* read as long we have packets in the stream */
  while(av_read_frame(pFmtCtx,&packet)>=0) {
    
     /* we only need packets of our video stream*/
    if(packet.stream_index == videoStream) {
      
      /* decode video frame */
      avcodec_decode_video2(pCodecCtx,pFrame,&frameFinished,
                            &packet);

      if(frameFinished) {
        /* scale, display and sleep for ~30ms*/
        sws_scale(pSwsCtx,pFrame->data, pFrame->linesize,0,sh,pPict->data, pPict->linesize);
        ascii_art(pPict);
        usleep(30000);
    
      }
    }
    /* free current packet struct */
    av_free_packet(&packet);
  }

  /* tidy up.. */
  av_free(buffer);
  av_free(pPict);
  av_free(pFrame);
  avcodec_close(pCodecCtx);
  avformat_free_context(pFmtCtx);

  return 0;
}
