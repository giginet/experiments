#####################################
# Define implicit compilation rules #
#####################################

# uncomment following lines for UNIX computers
# nominally tested on Sun, Cray UNICOS, and Vax Ultrix systems
CC=g++ 
OPT= -O3

LDLIBS  = -L/usr/lib -L/usr/local/lib
MATHLIB = -lm
XLIB    = -L/usr/X11R6/lib -lX11


SRCCODE=action.cc A-society.cc xgra5-3.cc 
	
OBJECTS=action.o A-society.o xgra5-3.o 

A-society: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDLIBS) $(MATHLIB) $(XLIB) $(OPT)
	chmod 755 $@

$(OBJECTS): xgra5-3.h

clean:
	rm -f A-society
	rm -f *.o
	rm -f *~
	rm -f *#

