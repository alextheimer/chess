CC = g++
CFLAGS = -Wall -std=c++17

TARGET = chess

TARGET_DEPS = $(shell find src -type f -not -name main.cpp)
TEST_DEPS = $(shell find test -type f -not -name run)

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -DDEBUG
else
	CFLAGS += -O3 -DNDEBUG -flto
endif

ifeq ($(VECREP), 1)
    CFLAGS += -fopt-info-vec-missed
endif

$(TARGET): src/main.cpp $(TARGET_DEPS)
	$(CC) $(CFLAGS) -o bin/$(TARGET) src/main.cpp $(TARGET_DEPS) -I include

test: $(TARGET_DEPS) $(TEST_DEPS)
	$(CC) $(CFLAGS) -I include -I /usr/src/googletest/googletest/include -L /usr/src/googletest/lib \
	-o test/bin/run $(TARGET_DEPS) $(TEST_DEPS) -lgtest_main -lgtest -lpthread

clean: 
	$(RM) bin/$(TARGET) test/bin/run