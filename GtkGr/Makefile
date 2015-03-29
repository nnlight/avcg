CC=gcc
CFLAGS=-g -c -Wall `pkg-config --cflags gtk+-2.0`
VCG_CFLAGS=-g -c
MW_CFLAGS=-g -c -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS=-g `pkg-config --libs gtk+-2.0` -lstdc++
VCG_SOURCES=$(wildcard vcg/*.c)
MW_SOURCES=$(wildcard mw/*.c)
#SOURCES= draw_buffer.cpp  GtkGr.cpp stdafx.cpp  ui.cpp gr.cpp preferences.cpp vr_graph.cpp
ALLCXX_SOURCES=$(wildcard *.cpp)
SOURCES=$(ALLCXX_SOURCES:tes_mw.cpp=)
OBJECTS=$(SOURCES:.cpp=.o)
VCG_OBJECTS=$(VCG_SOURCES:.c=.o)
MW_OBJECTS=$(MW_SOURCES:.c=.o)
EXECUTABLE=avcg

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) $(VCG_OBJECTS) $(MW_OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(VCG_OBJECTS) $(MW_OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

# use `::` instead of `:` to tell make that prerequisite may not be an intermediate file
# thats's for disabling chain of implicit rules  %o <- %.c <- %.y
vcg/%.o:: vcg/%.c
	$(CC) $(VCG_CFLAGS) $< -o $@

mw/%.o: mw/%.c
	$(CC) $(MW_CFLAGS) $< -o $@

#vcg.o:
#	$(CC) $(CFLAGS) $(VCG_SOURCES) -o $@


#for testing makefile syntax:)
.PHONY: tes
tes:
	@echo $(SOURCES)
	@echo === 
	@echo $(VCG_SOURCES)
	@echo === 
	@echo $(VCG_OBJECTS)



#depend:
#	gccmakedep -- $(CFLAGS) -- $(SOURCES)
#	makedepend $(SOURCES)


.PHONY: clean
clean:
	rm *.o vcg/*.o mw/*.o $(EXECUTABLE)

.PHONY: doc
doc:
	doxygen

.PHONY: tags
tags:
	ctags -f tags *h *cpp vcg/*[ch]


