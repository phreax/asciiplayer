INCLUDES= -I./ -I/usr/local/include/libavcodec/ -I/usr/local/include/libavformat/ -I/usr/local/include/libswscale/
LIBPATH = -L/usr/local/lib
LDFLAGS = -lavcodec -lavformat -lavutil -lz  -pthread -lm -lxvidcore -lswscale
RM 		= /bin/rm -f
CC		= colorgcc 
SOURCES = player.c screen.c
OBJECTS = $(SOURCES:.c=.o)
MAIN	= asciiplayer 
TEST_OBJ= player.o screen.o 

all: $(MAIN) 

$(MAIN): $(OBJECTS)
	$(CC) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< 

clean:
	$(RM) *~ *.o $(MAIN)

