CC = clang
CFLAGS = -Wall -Wunreachable-code
BUILD_DIR = build
BIN = molsh

build: src/*.c
	"$(CC)" -o "$(BUILD_DIR)/$(BIN)" $(CFLAGS) $^
	chmod +x "$(BUILD_DIR)/$(BIN)"

clean:
	rm "$(BUILD_DIR)/*"
