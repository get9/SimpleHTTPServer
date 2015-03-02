CC = /usr/bin/cc
CFLAGS = -c -Weverything -Wpedantic -std=c11 -D _POSIX_C_SOURCE=200809L -D _XOPEN_SOURCE -D _POSIX_SOURCE
RM = /bin/rm
LDFLAGS = 
SOURCES = main.c server.c tcp.c util.c http.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = myserver

all: $(EXECUTABLE)

debug: CFLAGS += -g -DDEBUG

debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)
