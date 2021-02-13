CC = g++
CFLAGS  = -Wall

TARGET = chess

TARGET_DEPS = $(shell find src -type f)
TEST_DEPS = $(shell find test -type f)

default: $(TARGET)

ifdef $(DEBUG)
CFLAGS += -g -O0 
endif

$(TARGET): $(TARGET_DEPS)
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET_DEPS) -I include

test: $(TARGET_DEPS) $(TEST_DEPS)
	$(CC) -o test/bin/run $(TARGET_DEPS) $(TEST_DEPS) /usr/local/lib/libgtest_main.a /usr/local/lib/libgtest.a -I include -lpthread

clean: 
	$(RM) $(TARGET) *.o *~