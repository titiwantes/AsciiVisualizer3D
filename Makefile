# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRC_DIR = src
BUILD_DIR = build
PROG_NAME = AsciiVizualizer3D

SRC_FILES = $(SRC_DIR)/main.c\
			$(SRC_DIR)/screen/screen.c\
			$(SRC_DIR)/shapes/shape.c\
			$(SRC_DIR)/shapes/cube.c\
			$(SRC_DIR)/shapes/torus.c\
			$(SRC_DIR)/screen/light.c\

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(PROG_NAME)

$(BUILD_DIR)/$(PROG_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(PROG_NAME)

distclean:
	rm -rf $(BUILD_DIR)
