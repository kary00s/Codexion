NAME = codexion
FLAGS = -pthread
CC = cc

SRC = codexion.c\
	./Parser/parser.c\
	./Dongles/dongles.c\
	./Monitor/monitor.c\
	./Coders/coders.c\
	./Cleaner/cleaner.c\
	timer.c\
	./Queue/queue.c\
	./Controller/routine.c\
	./Dongles/dongles_controller.c \
	./Initializer/initializer.c\
	./Controller/controller.c\
	./Mutexs/mutex_cond_utils.c\

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

