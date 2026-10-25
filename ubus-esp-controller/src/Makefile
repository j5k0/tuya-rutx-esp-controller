TARGET_EXEC := esp-controller
BUILD_DIR := ./build
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=build/%.o)
CC := gcc
CFLAGS := -Wall -Wextra -Iinclude -lubox -lubus -lblobmsg_json -lserialport
.PHONY: all clean
all: $(TARGET_EXEC)
$(TARGET_EXEC): $(OBJS)
	$(CC) $^ -o $(TARGET_EXEC) $(CFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
clean:
	rm -r $(TARGET_EXEC) $(OBJS)