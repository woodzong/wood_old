#####include $(C2C_BASE_PATH)/comm/makefile.cgicomm

CXX     = g++
CFLAGS  = -g -Wall -O2

INCLUDE = 

LIB	= -lz

INSTALL = ./

#####################################
OUTPUT  = nigwind

OBJS    = main.o CWRegex.o CWZipOp.o CWHttpResp.o CWLoadOneHttp.o

all:$(OUTPUT)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDE) -c $^ 

.o:
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ $^

$(OUTPUT): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ ${LIB}
	
clean:
	rm -f *.o *.~ *.bak
	rm -f $(OUTPUT)


