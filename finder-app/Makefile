#References : ECEN5813

# Define commands
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Werror

# The executable
EXEC = writer

all: $(EXEC)

$(EXEC): writer.o
	$(CC) $(CFLAGS) -o $(EXEC) writer.o

writer.o: writer.c
	$(CC) $(CFLAGS) -c writer.c

clean:
	rm -f $(EXEC) *.o