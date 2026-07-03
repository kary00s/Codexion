NAME = codexion
FLAGS = -pthread
CC = cc

SRC = ./Main/codexion.c\
	timer.c\
	./Parser/parser.c\
	./Dongles/dongles_controller.c \
	./Dongles/dongles.c\
	./Coders/routine.c\
	./Coders/coders.c\
	./Coders/coders_controller.c\
	./Controller/controller.c\
	./Monitor/monitor.c\
	./Cleaner/cleaner.c\
	./Queue/queue.c\
	./Initializer/initializer.c\
	./Mutexs/mutex_cond_utils.c\

OBG = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBG)
	$(CC) $(FLAGS) $(OBG) -o $(NAME) 
#-fsanitize=thread -g3

%.o: %.c codexion.h
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	rm -f $(OBG)

fclean: clean
	rm -f $(NAME)

re: fclean all

