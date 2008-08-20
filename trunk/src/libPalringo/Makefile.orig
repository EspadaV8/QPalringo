CFLAGS= -O2 -Wall -Wunused -Wno-uninitialized -Wno-return-type -fno-strength-reduce -g

# DEFINES += -DDEBUG

CFLAGS += ${DEFINES}

.PHONY: message

all: library

again: clean all

# Define which programs use which object files
libraryobj = libPalringo.o callbackFunctions.o DataMap.o crypto.o\
	     MiscFunctions.o salsa20.o my_oldpassword.o md5.o

%.o: %.cc
	$(CXX) $(CFLAGS)  $(INC) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS)  $(INC) -c -o $@ $<

message:
	@echo -
	@echo - Building static Palringo BOT API
	@echo -

library: message ${libraryobj}
	$(AR) cr libPalringo.a $(libraryobj)

clean:
	${RM} *.a *.o

distclean: clean
