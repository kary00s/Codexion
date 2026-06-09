NAME = codexion
FLAGS = -pthread
CC = cc

SRC = codexion.c\
	parser.c\
	dongles.c\
	monitor.c\
	coders.c\
	cleaner.c\
	timer.c\
	queue.c\
	routine.c\
	holder.c

OBG = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBG)
	$(CC) $(FLAGS) $(OBG) -o $(NAME) 

%.o: %.c codexion.h
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	rm -f $(OBG)

fclean: clean
	rm -f $(NAME)