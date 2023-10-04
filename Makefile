CC = clang
CFLAGS = -Wall -Wunreachable-code
BUILD_DIR = build
BIN = molsh

build: src/*.c
	mkdir -p "$(BUILD_DIR)"
	"$(CC)" -o "$(BUILD_DIR)/$(BIN)" $(CFLAGS) $^
	chmod +x "$(BUILD_DIR)/$(BIN)"

clean:
	rm "$(BUILD_DIR)/*"

.PHONY: run
run:
	make build
	"./$(BUILD_DIR)/$(BIN)"
