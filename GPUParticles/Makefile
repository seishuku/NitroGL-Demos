PROJECT=engine

OBJS  = 3ds.o
OBJS += dds.o
OBJS += engine.o
OBJS += font.o
OBJS += image.o
OBJS += math.o
OBJS += opengl.o
OBJS += tga.o

CFLAGS = -g -Wall -O0
ASFLAGS = 
LDFLAGS  = -lX11 -lGL -lm

CC = gcc
AS = as
OBJCOPY = objcopy

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(PROJECT)

clean:
	rm -f $(OBJS) $(PROJECT)

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@    

.cpp.o :
	$(CC) $(CFLAGS) -c $< -o $@

.s.o :
	$(AS) $(ASFLAGS) -o $@ $<
