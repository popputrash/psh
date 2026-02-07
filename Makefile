CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

psh: $(OBJS)
	$(CC) $(CFLAGS) -o psh $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	

clean:
	rm -f psh $(SRC_DIR)/*.o

.PHONY: clean

