#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
#include <stdio.h>
#include <unistd.h>

#define TERM_W 80
#define TERM_H 22 

const unsigned char colors[] = " .:,;+ijtfLGDKW#";
const int clen = 17;
const int buf_size = TERM_W*TERM_H;
unsigned char sc_buf[TERM_W*TERM_H+1];

void display_buffer() {

  int i;
  unsigned char c;
  printf("\x1b[H");
    for(i=1;i<buf_size;i++) {
        c = i%TERM_W == 0 ? 10 : sc_buf[i];
        putchar(c);
    }
}
// copy one line
void convert2ascii(uint8_t *dst, uint8_t *src, int size) {
  int i;
  float c;
  uint8_t *_dst,*_src;
  _dst = dst;
  _src = src;
  for(i=0;i<size;i++,_src+=3,_dst++) {
     c = 0.2126*_src[0]+0.7152*_src[1]+0.0722*_src[2];
     c = clen*c/255.0;
     _dst[i] = colors[(int)c];
  }
}


void saveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;
  
  // Open file
  sprintf(szFilename, "images/frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;
  
  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);
  
  // Write pixel data
  for(y=0; y<height; y++)
    //fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
  
  // Close file
  fclose(pFile);
}

void asciiArt(AVFrame *pFrame, int width, int height) {

  uint8_t *lineptr;
  int y;
  memset(sc_buf,32,buf_size);
  for(y=0; y<height; y++) {
    lineptr = pFrame->data[0]+y*pFrame->linesize[0];
    convert2ascii(sc_buf+width*y,lineptr,width);
  }
  display_buffer();

}


int main(int argc, char **argv) {

  if(argc<2) {
    fprintf(stderr,"Missing filename\n"); 
    return -1;
  }
  // init  all codecs
  av_register_all();

  AVFormatContext *pFmtCtx;
  struct SwsContext *pSwsCtx;

  pFmtCtx = avformat_alloc_context();
  printf("open file\n");
  if(avformat_open_input(&pFmtCtx, argv[1], NULL, NULL) != 0) {
    fprintf(stderr,"Failed to open file: %s\n",argv[1]); 
    return -1;
  }

  printf("find stream info\n");
  if(avformat_find_stream_info(pFmtCtx,NULL) < 0) { 
    fprintf(stderr,"No stream found!\n"); 
    return -1;
  }

  printf("dump format\n");
  av_dump_format(pFmtCtx, 0, argv[1],0);

  int i;
  int videoStream;
  AVCodecContext *pCodecCtx;

  printf("open stream\n");
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

  // find suitable codec
 
  AVCodec * pCodec;
  
  printf("find codec\n");
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

  // allocate data structs
  pFrame = avcodec_alloc_frame();
  pPict  = avcodec_alloc_frame();

  uint8_t *buffer;
  int szPict;;
  int sw,sh;

  sw = pCodecCtx->width;
  sh = pCodecCtx->height;

  szPict = avpicture_get_size(PIX_FMT_RGB24, sw,sh);

  buffer = (uint8_t *)av_malloc(szPict*sizeof(uint8_t));

  // associate frame with out buffer
  avpicture_fill( (AVPicture *)pPict,buffer,PIX_FMT_RGB24, sw, sh);

  int frameFinished;
  AVPacket packet;

  pSwsCtx = sws_getContext(sw,sh,pCodecCtx->pix_fmt,TERM_W,TERM_H,PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
  i=0;
  printf("read stream\n");
  while(av_read_frame(pFmtCtx,&packet)>=0) {
    
    if(packet.stream_index == videoStream) {
      // decode video frame
      avcodec_decode_video2(pCodecCtx,pFrame,&frameFinished,
                            &packet);

      if(frameFinished) {
        sws_scale(pSwsCtx,pFrame->data, pFrame->linesize,0,sh,pPict->data, pPict->linesize);
        asciiArt(pPict,TERM_W,TERM_H);
    
        // do something with picture
      }
    }
    av_free_packet(&packet);
  }

  // tidy up..
  av_free(buffer);
  av_free(pPict);
  av_free(pFrame);
  avcodec_close(pCodecCtx);
  avformat_free_context(pFmtCtx);

  return 0;
}
