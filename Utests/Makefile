NAME =		tests
CC = 		gcc
IFLAGS =	-I ../inc/ \
		-I ../lib/libmpm/inc \
		-I ../lib/libmpm/lib/libmorphux/inc \
		-I ../lib/libmpm/lib/sqlite/ \
		-I ../lib/libmpm/lib/libconfuse/src/ \
		-I ../lib/libmpm/lib/json-c/ \
		-I ../lib/libmpm/lib/libsodium/src/libsodium/include
LFLAGS =	-L../lib/libmpm -lmpm -pthread -ldl -lz


SRCS =		$(wildcard *.c) $(wildcard ../src/*.c)
OBJS =		$(SRCS:%.c=%.o)
COVFLAGS =	"-coverage"

CFLAGS =	-Wall -Wextra -Werror -Wno-unused-result \
		-std=gnu99 -g -DCOMPILE_WITH_TEST $(IFLAGS) $(COVFLAGS)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

check:
	rm -f ../src/main.o
	$(MAKE)
	./$(NAME)

valgrind: $(NAME)
	valgrind --leak-check=full --error-exitcode=1 ./$(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
