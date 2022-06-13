# Project: Present
# Makefile created by ouzdeville
CC       = gcc
OBJ      = present.o main.o 
LINKOBJ  = present.o main.o 
LIBS     = 
INCS     = 
CXXINCS  = 
BIN      = present
LFLAGS=
CFLAGS= -c -Wall -I. 
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) -O3 $(LINKOBJ) -o $(BIN) $(LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	