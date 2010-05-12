CC=gcc
CFLAGS=-g -c -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS=-g `pkg-config --libs gtk+-2.0` -lstdc++
VCG_SOURCES=vcg/lex.yy.c vcg/y.tab.c vcg/main.c vcg/alloc.c vcg/timelim.c vcg/options.c vcg/folding.c vcg/step0.c vcg/step1.c vcg/step2.c vcg/step3.c vcg/step4.c vcg/tree.c vcg/prepare.c vcg/drawlib.c vcg/drawstr.c
SOURCES=GtkGr.cpp stdafx.cpp draw_buffer.cpp vr_graph.cpp ui.cpp gr.cpp preferences.cpp $(VCG_SOURCES)
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

.PHONY: clean
clean:
	rm *.o *~ $(EXECUTABLE)

.PHONY: doc
doc:
	doxygen


