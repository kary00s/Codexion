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
	dongles_manager.c \
	initializer.c\
	manager.c\
	mutex_cond_utils.c\

OBG = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBG)
	$(CC) $(FLAGS) $(OBG) -o $(NAME)  -pthread 
#-fsanitize=thread -g3

%.o: %.c codexion.h
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	rm -f $(OBG)

fclean: clean
	rm -f $(NAME)

re: fclean all

