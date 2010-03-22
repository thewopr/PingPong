# Makefile for CSCI 363 Lab 03 Spring 2007
# L. Felipe Perrone
# 02-08-2007

CC = gcc
HDRS = lab05.h wrappers.h
OBJS = lab05.o wrappers.o
CFLAGS = -Wall -g -lm
EXECS =	source sink gateway reflector pingpong

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

pingpong: pingpong.c lab05.o wrappers.o
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ $(LFLAGS)

runtests: 
	for i in {1..1000}; do ./pingpong 1 localhost 9000 TCP >> test_1_TCP_local_local.txt; done
	for i in {1..1000}; do ./pingpong 16383 localhost 9000 TCP >> test_16383_TCP_local_local.txt; done
	for i in {1..1000}; do ./pingpong 32767 localhost 9000 TCP >> test_32767_TCP_local_local.txt; done
	for i in {1..1000}; do ./pingpong 65000 localhost 9000 TCP >> test_65000_TCP_local_local.txt; done

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
