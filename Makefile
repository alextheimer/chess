CC = g++
CFLAGS  = -g -Wall

TARGET = chess

DEPS = $(shell find src -type f)

default: $(TARGET)

$(TARGET): $(DEPS)
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(DEPS) -I include

clean: 
	$(RM) $(TARGET) *.o *~