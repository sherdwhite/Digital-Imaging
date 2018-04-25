CC	= g++
CFLAGS = -g `pkg-config --cflags opencv`
TARGET1	= display
TARGET2	= gray_pxls
TARGET3 = trackbar
OBJS1	= display.o
OBJS2	= gray_pxls.o
OBJS3	= trackbar.o
LIBS	= `pkg-config --libs opencv`

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(OBJS1)
	$(CC) -o $@ $(OBJS1) $(LIBS)

$(TARGET2): $(OBJS2)
	$(CC) -o $@ $(OBJS2) $(LIBS)

$(TARGET3): $(OBJS3)
	$(CC) -o $@ $(OBJS3) $(LIBS)

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	/bin/rm -f core *.o $(TARGET1) $(TARGET2) $(TARGET3)
