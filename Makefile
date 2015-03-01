CC=/usr/bin/cc
CFLAGS=-c -Wall -Wpedantic
RM=/bin/rm
LDFLAGS=
SOURCES=main.c server.c tcp.c util.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=myserver

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)
