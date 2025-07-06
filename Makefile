# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Source and object files
SRCS = main.c config.c incident.c log.c map.c unit.c simulation.c utils.c
OBJS = $(SRCS:.c=.o)

# Output executable name
TARGET = emergency_dispatch

# Default target: build the executable
all: $(TARGET)

# Linking the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up intermediate and output files
clean:
	del /Q *.o *.exe 2>nul || rm -f *.o *.exe
