CC=gcc
CFLAGS=-g -c -Wall `pkg-config --cflags gtk+-2.0`
VCG_CFLAGS=-g -c
MW_CFLAGS=-g -c -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS=-g `pkg-config --libs gtk+-2.0` -lstdc++
VCG_SOURCES=$(wildcard vcg/*.c)
MW_SOURCES=$(wildcard mw/*.c)
SOURCES=$(patsubst tes_mw.cpp,,$(wildcard *.cpp))
OBJECTS=$(SOURCES:%.cpp=obj/%.o)
VCG_OBJECTS=$(VCG_SOURCES:%.c=obj/%.o)
MW_OBJECTS=$(addprefix obj/,$(MW_SOURCES:.c=.o))
HEADERS=$(wildcard *.h)
VCG_HEADERS=$(wildcard vcg/*.h)
EXECUTABLE=avcg

_=$(shell mkdir -p obj/mw obj/vcg) # there is another way via @mkdir -p $(@D) in each rule

#disable make implicit suffix rules
#this is to the make did not genetare `grammar.c' from `grammar.y'
.SUFFIXES:

.PHONY: all
all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) $(VCG_OBJECTS) $(MW_OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(VCG_OBJECTS) $(MW_OBJECTS) -o $@

obj/%.o: %.cpp $(HEADERS) $(VCG_HEADERS)
	$(CC) $(CFLAGS) $< -o $@

# use `::` instead of `:` to tell make that prerequisite may not be an intermediate file
obj/vcg/%.o: vcg/%.c $(VCG_HEADERS)
	$(CC) $(VCG_CFLAGS) $< -o $@

obj/mw/%.o: mw/%.c
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



.PHONY: clean
clean:
	rm -f *.o vcg/*.o mw/*.o $(EXECUTABLE)
	rm -rf obj

.PHONY: doc
doc:
	doxygen

.PHONY: tags
tags:
	ctags -f tags *h *cpp vcg/*[ch]


