CC = gcc
CFLAGS = -Wall -Iinclude

SRC_DIR = src
DEMO_DIR = demo
TEST_DIR = tests

# Main program sources
SOCCER_SRC = \
	$(SRC_DIR)/renderer.c \
	$(SRC_DIR)/canvas.c \
	$(DEMO_DIR)/SoccerDemo.c

# Test (cube) program sources
TEST_SRC = \
	$(SRC_DIR)/math3d.c \
	$(TEST_DIR)/test_math.c

# Clock face (output) program sources
CLOCK_SRC = \
	$(SRC_DIR)/canvas.c \
	$(DEMO_DIR)/ClockFace.c

LIGHT_SRC=\
	$(DEMO_DIR)/LightingDemo.c\
	$(SRC_DIR)/animation.c\
	$(SRC_DIR)/lighting.c

# Output binary names

SOCCER_BIN = soccer
TEST_BIN = cube
CLOCK_BIN = output
LIGHT_BIN= light

# Object files
SOCCER_OBJ = $(SOCCER_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)
CLOCK_OBJ = $(CLOCK_SRC:.c=.o)
LIGHT_OBJ = $(LIGHT_SRC:.c=.o)

# Default rule: build all
all: $(SOCCER_BIN) $(TEST_BIN) $(CLOCK_BIN) $(LIGHT_BIN)

# Build soccer program
$(SOCCER_BIN): $(SOCCER_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Build cube (test_math) program
$(TEST_BIN): $(TEST_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Build clockface (output) program
$(CLOCK_BIN): $(CLOCK_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(LIGHT_BIN): $(LIGHT_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Compile all .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean all binaries and images
clean:
	rm -f $(SOCCER_OBJ) $(TEST_OBJ) $(CLOCK_OBJ) $(LIGHT_OBJ) $(SOCCER_BIN) $(TEST_BIN) $(CLOCK_BIN) $(LIGHT_BIN) frame_*.pgm *.pgm frame*.pgm cube.mp4 light.mp4 output.jpg soccer.mp4 clockface.png
