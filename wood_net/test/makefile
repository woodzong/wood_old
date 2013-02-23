#####include $(C2C_BASE_PATH)/comm/makefile.cgicomm

CXX     = g++
CFLAGS  = -g -Wall -O2

INCLUDE = 

LIB	=    

OUTPUT = server client

INSTALL = ./

#####################################
.PHONY :all
all:$(OUTPUT) 

.SUFFIXES: .cpp

.cpp:
	$(CXX) $(CFLAGS) $(INCLUDE) -I. -g -o $* $*.cpp $(LIB)
#	cp $* $(INSTALL)

.PHONY :clean	
clean:
	rm -f *.o *.~ *.bak
	rm -f $(OUTPUT)


