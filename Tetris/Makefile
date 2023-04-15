TARGET=tetris
OBJS=3ds.o dds.o Engine.o font.o Game.o image.o index_t.o math.o opengl.o overlay.o tga.o

CC=gcc
CFLAGS=-Wall -O3 -c -I/usr/X11/include
LDFLAGS=-L/usr/X11/lib -lGL -lopenal -lX11 -lm

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) libvorbisidec.a $(LDFLAGS)

.c: .o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	-rm *.o
	-rm $(TARGET)
