CC=gcc
CFLAGS=-g -c -Wall `pkg-config --cflags gtk+-2.0`
VCG_CFLAGS=-g -c
LDFLAGS=-g `pkg-config --libs gtk+-2.0` -lstdc++
VCG_SOURCES=$(wildcard vcg/*.c)
#SOURCES=$(wildcard *.cpp) $(VCG_SOURCES)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
VCG_OBJECTS=$(VCG_SOURCES:.c=.o)
EXECUTABLE=avcg

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) $(VCG_OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(VCG_OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.c.o:
	$(CC) $(VCG_CFLAGS) $< -o $@

vcg.o:
	$(CC) $(CFLAGS) $(VCG_SOURCES) -o $@


#for testing makefile syntax:)
.PHONY: tes
tes:
	@echo $(VCG_SOURCES)
	@echo === 
	@echo $(VCG_OBJECTS)



#depend:
#	gccmakedep -- $(CFLAGS) -- $(SOURCES)
#	makedepend $(SOURCES)


.PHONY: clean
clean:
	rm *.o vcg/*.o $(EXECUTABLE)

.PHONY: doc
doc:
	doxygen


