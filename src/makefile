CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp view.cpp lexer.cpp token.cpp parser.cpp operator.cpp tokenizer.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=AQL

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o AQL