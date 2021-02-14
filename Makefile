CC = g++
CFLAGS  = -Wall

TARGET = chess

TARGET_DEPS = $(shell find src -type f -not -name "*main.cpp")
TEST_DEPS = $(shell find test -type f)

default: $(TARGET)

debug: CFLAGS += -g -O0 
debug: $(TARGET)

$(TARGET): src/main.cpp $(TARGET_DEPS)
	$(CC) $(CFLAGS) -o bin/$(TARGET) src/main.cpp $(TARGET_DEPS) -I include

test: $(TARGET_DEPS) $(TEST_DEPS)
	$(CC) -I include -I /usr/src/googletest/googletest/include -L /usr/src/googletest/lib -o test/bin/run $(TARGET_DEPS) $(TEST_DEPS) -lgtest_main -lgtest -lpthread

clean: 
	$(RM) bin/$(TARGET) test/bin/run