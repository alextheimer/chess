CC = g++
CFLAGS  = -Wall

TARGET = chess

TARGET_DEPS = $(shell find src -type f -not -name *$(TARGET).cpp)
TEST_DEPS = $(shell find test -type f)

default: $(TARGET)

ifdef $(DEBUG)
CFLAGS += -g -O0 
endif

$(TARGET): $(TARGET_DEPS)
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET_DEPS) -I include

test: $(TARGET_DEPS) $(TEST_DEPS)
	$(CC) -I include -I /usr/src/googletest/googletest/include -L /usr/src/googletest/lib -o test/bin/run $(TARGET_DEPS) $(TEST_DEPS) -lgtest_main -lgtest -lpthread

clean: 
	$(RM) bin/$(TARGET) test/bin/run