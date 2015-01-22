#
# Makefile for cryptouio

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld

CFLAGS = -O3 -W -Wall -g -Wno-unused-parameter
INCLUDES = -I.
LIBSINCLUDES =
LDFLAGS=-shared
LIBS=

RM=rm -f
MKDIR=mkdir

OBJFILES := hmac.o sha1.o sha224-256.o sha384-512.o usha.o md5.o


EXEC_SHA = shatest
EXEC_MD5 = md5test

all: $(EXEC_SHA) $(EXEC_MD5)

$(EXEC_SHA): $(OBJFILES) shatest.o
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJFILES) shatest.o $(LIBS) $(LIBSINCLUDES) -o $(EXEC_SHA)

$(EXEC_MD5): $(OBJFILES) mddriver.o
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJFILES) mddriver.o $(LIBS) $(LIBSINCLUDES) -o $(EXEC_MD5)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJFILES)
	rm -f $(EXEC_SHA) $(EXEC_MD5)
