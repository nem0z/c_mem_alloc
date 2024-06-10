SRC	=	*.c

CFLAGS	=	-Iinclude -W -Wall -Wextra

OBJ	=	$(SRC:.c=.o)

NAME	=	mem.o

all:
	gcc *.c -o $(NAME) $(CFLAGS)

clean:
	rm -f $(OBJ)
	rm -f *.o

fclean:	clean
	rm -f $(NAME)

re:	fclean all