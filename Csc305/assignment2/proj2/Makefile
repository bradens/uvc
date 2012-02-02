# This Makefile makes the `plant' program, from its source and
# header files.
#
# To use this Makefile, type:
#
#        make BOX=<machinetype> 
#
# where
#	<machinetype> is the type of machine you are compiling on --
#			"dell" - Dell linux machines in Taylor basement
#			"linux" - Intel Linux machines, mostly in Painter?
#			"sun" - Sun workstations in Taylor basement
#			"sgi" - SGI O2, very few of these around
#			"ibm" - IBM linux machines in Painter?
#
#
# To change this Makefile, you should only have to worry about the
# section begining with the comment:
# ``Stuff that depends on the specific program we're compiling''
#

# First set up variables we'll use when making things

########
# Variables for stuff that varies on different machines:
# Where the librarys are
# What flags to use with the C compiler
########
# comment this out for handin, this hard codes it to compile for mac
set BOX = mac

ifeq ($(BOX),mac) 
  LDLIBS =  -L/usr/X11R6/lib -lGLU  -lGL -lXmu -lXext -lX11 -lm -L/sw/lib/ -lglut -lstdc++
  CFLAGS = -g 
endif 

ifeq ($(BOX),dell) 
  LDLIBS =  -L/lusr/X11/lib -lglut -lGLU  -lGL -lXext -lX11 -lm -L/lusr/gnu/lib -lstdc++
  CFLAGS = -g 
endif 

ifeq ($(BOX),linux) 
  LDLIBS = -L/p/lib/Mesa -lglut  -lMesaGL -lMesaGLU -L/lusr/X11/lib -lXi -lXmu -lX11 -lm -L/lusr/gnu/lib -lstdc++
  CFLAGS = -g -Wall
endif

ifeq ($(BOX),sun) 
  LDLIBS = -L/p/lib/Mesa -lglut -lMesaGL -lMesaGLU -L/usr/openwin/lib -lXi -lXmu -lX11 -lm -L/lusr/gnu/lib -lstdc++
  CFLAGS = -g 
endif

ifeq ($(BOX),ibm) 
  LDLIBS = -L/p/lib/Mesa -lglut  -lMesaGL -lMesaGLU -lXi -lXmu -lX11 -lm
  CFLAGS = -g -Wall
endif

########
# Some stuff that's the same for all programs and machines 
########

# The commands to call the C and C++ compilers
CC = cc 
C++ = g++

# Where to find the include files:
ifeq ($(BOX),mac)
 INCLUDE = -I/usr/X11R6/include/ -I/sw/include/
else 
INCLUDE = -I/p/include/Mesa -I/lusr/X11/include/
endif

########
# Stuff that depends on the specific program we're compiling
# To use this file for other programs, you'll need to change things
# in this section.
########

# The name of the output program 
PROG = plant

# Object files that go into the final executable
OBJS = plant.o drawplant.o Vec.o Mat.o

# Header files 
HDRS = drawplant.h Vec.h Mat.h

########
# Finally, the commands that actually make stuff
# You shouldn't have to change anything here 
# These commands are of the form:
#   output_thing:  stuff it depends on
#   <TAB> command to make output_thing 
########

# re-link the program when the object files change
$(PROG):  $(OBJS)
	$(C++) $(OBJS) $(LDLIBS) -o $(PROG) 

# change a C object file whenever the corresponding source file
# or any of the header files changes
%.o : %.c $(HDRS) 
	$(CC) -c $(CFLAGS) $(INCLUDE) $<
# What this means: 
# The %.o and %.c mean any .o file and the corresponding .c file
# All .c's are recompiled if you change one of the header files
# The $< symbol refers to the FIRST dependancy - so don't try to
# put $(HDRS) before %.c!

# change a C++ object file whenever the corresponding source file
# or any of the header files changes
%.o : %.cpp $(HDRS) 
	$(C++) -c $(CFLAGS) $(INCLUDE) $<
