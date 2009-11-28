CC=g++
CFLAGS=-g -c -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS=-g `pkg-config --libs gtk+-2.0`
SOURCES=GtkGr.cpp draw_buffer.cpp vr_graph.cpp ui.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=avcg

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@



#depend:
#	gccmakedep -- $(CFLAGS) -- $(SOURCES)
#	makedepend $(SOURCES)

clean:
	rm *.o *~ $(EXECUTABLE)


