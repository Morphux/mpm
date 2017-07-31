################################### LICENSE ####################################
#                            Copyright 2017 Morphux                            #
#                                                                              #
#        Licensed under the Apache License, Version 2.0 (the "License");       #
#        you may not use this file except in compliance with the License.      #
#                  You may obtain a copy of the License at                     #
#                                                                              #
#                 http://www.apache.org/licenses/LICENSE-2.0                   #
#                                                                              #
#      Unless required by applicable law or agreed to in writing, software     #
#       distributed under the License is distributed on an "AS IS" BASIS,      #
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  #
#        See the License for the specific language governing permissions and   #
#                       limitations under the License.                         #
################################################################################

NAME =		mpm
CC =		gcc
AUTHOR =	Louis Solofrizzo <louis@ne02ptzero.me>

IFLAGS =	-I inc/ \
		-I lib/libmpm/inc \
		-I lib/libmpm/lib/libmorphux/inc \
		-I lib/libmpm/lib/sqlite/ \
		-I lib/libmpm/lib/libconfuse/src/ \
		-I lib/libmpm/lib/json-c/ \
		-I lib/libmpm/lib/libsodium/src/libsodium/include \

OFLAGS =	-std=gnu99 -g -O3

SFLAGS =	-Wall -Wextra -Werror -Wno-unused-result

LFLAGS =	-Llib/libmpm -lmpm -pthread -ldl -lz

DFLAGS =	-DNAME="\"$(NAME)\"" -DAUTH="\"$(AUTHOR)\"" -DMPM_SUGG

CFLAGS =	$(SFLAGS) $(IFLAGS) $(OFLAGS) $(LFLAGS) $(DFLAGS)

SRCS =		$(wildcard src/*.c)
OBJS =		$(SRCS:%.c=%.o)

all: $(NAME)

%.o: %.c
	@echo "CC\t\t$@"
	@$(CC) $(CFLAGS) -c -o $@ $^

$(NAME): $(OBJS)
	@echo "CCLD\t\t$(NAME)"
	@$(CC) $(OBJS) -o $(NAME) $(CFLAGS)

init:
	make -C lib/libmpm init re

check: all
	make -C tests/

coverage:
	make -C lib/libmpm test
	$(MAKE) fclean all OFLAGS="-std=gnu99 -g -O0 -coverage -DCOMPILE_WITH_TEST -fno-inline"
	make -C tests/
	gcov -o src/ $(SRCS)

doc:
	doxygen docs/doxyfile

clean:
	rm -f $(OBJS)
	rm -f *.gcov
	rm -f src/*.gcno
	rm -f src/*.gcda

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all
