# Makefile for CSCI 363 Lab 03 Spring 2007
# L. Felipe Perrone
# 02-08-2007

CC = gcc
HDRS = lab05.h wrappers.h
OBJS = lab05.o wrappers.o
CFLAGS = -Wall -g
EXECS =	source sink gateway reflector

LFLAGS = 

all: $(EXECS)

%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@

source: source.c lab05.o wrappers.o
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ $(LFLAGS)

gateway: gateway.c lab05.o wrappers.o
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ $(LFLAGS)

sink: sink.c lab05.o wrappers.o
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ $(LFLAGS)

reflector: reflector.c lab05.o wrappers.o
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ $(LFLAGS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
