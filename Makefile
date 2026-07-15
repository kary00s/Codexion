NAME = codexion
FLAGS = -Wall -Wextra -Werror -g -pthread#-fsanitize=address
CC = cc 

SRC = ./Main/codexion.c\
		./Parser/parser.c\
		./Dongles/dongles.c\
		./Dongles/dongles_tools.c \
		./Dongles/dongles_tools_sec.c \
		./Coders/coders.c\
		./Coders/coders_tools.c\
		./Coders/coders_tools_sec.c\
		./Controller/controller.c\
		./Controller/controller_tools.c\
		./Monitor/monitor.c\
		./Cleaner/cleaner.c\
		./Cleaner/cleaner_sec.c\
		./Queue/queue.c\
		./Queue/queue_tools.c\
		./Initializer/initializer.c\
		./Mutexs/mutexs.c\
		./Mutexs/destroyer.c\
		./Routine/actions.c\
		./Routine/routine.c\
		./Timer/timer.c\
		./Timer/timer_sec.c\
		
OBJ = $(SRC:%.c=%.o)

%.o: %.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@ 

$(NAME): $(OBJ) $(HEADER)
	$(CC)  $(FLAGS) $(OBJ) -o $(NAME)
#	-fsanitize=address 

all: $(NAME)
	

clean:
	rm -rf $(OBJ)

fclean: clean
	rm $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean 

#-pthread -fsanitize=address 

