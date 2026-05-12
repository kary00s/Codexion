NAME = Codexion
FLAGS = $(-pthread)
CC = $(cc)

SRC = main.c \
	  queue.c \
	  dongle.c \ 
	  coder.c \
	  parser.c \
	  
OBG = $(SRC:.c=.o)

all: $(NAME_PROGRAN)

$(NAME_PROGRAN): $(OBG)
	$(CC) $(FLAGS) $(OBG) -o $(NAME_PROGRAN)

%.o: %.c codexion.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBG)

fclean: clean
	rm -f $(NAME_PROGRAN)