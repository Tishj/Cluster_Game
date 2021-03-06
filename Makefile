# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tbruinem <tbruinem@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/02/07 16:04:49 by tbruinem      #+#    #+#                  #
#    Updated: 2022/03/11 08:29:00 by tbruinem      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = cluster

LIBDIR = lib
SRCDIR = src
OBJDIR = obj
INCDIR = inc

LIBS =	mlx/libmlx42.a \
		vec/libvec.a

HEADERS = platformer.h
LINKER := $(foreach lib,$(LIBS),-L $(LIBDIR)/$(dir $(lib)) $(patsubst lib%.a,-l%,$(notdir $(lib)))) -ldl -lglfw -lm -lpthread
LIBS := $(foreach lib, $(LIBS), $(LIBDIR)/$(lib))
INCLUDES := -I ${INCDIR} -I lib/mlx/include -I lib/vec/incl

SRC =	main.c \
		Error.c \
		MLX.c \
		Command.c \
		Board.c \
		Game.c \
		Draw.c \
		Sort.c \
		Player.c \
		List.c \
		split.c \
		abspath.c \
		Sack.c \
		Util.c

OBJ := $(addprefix $(OBJDIR)/, $(SRC:%.c=%.o))
SRC := $(addprefix $(SRCDIR)/, $(SRC))

FLAGS = -Wall -Wextra -Werror
ifeq ($(DEBUG),1)
	FLAGS += -g
endif
ifeq ($(DEBUG),2)
	FLAGS += -g3 -fsanitize=address
endif


all: $(NAME) bot client

bot: starter_kit.c
	gcc $(FLAGS) starter_kit.c -o $@

client: client.c
	gcc $(FLAGS) client.c -o $@

$(NAME): $(OBJ) $(LIBS)
	@echo "Compiling Cluster..."
	gcc $(FLAGS) $(OBJ) -o $(NAME) $(INCLUDES) $(LINKER)

lib/mlx/libmlx42.a:
	@$(MAKE) -sC $(dir $@)

lib/vec/libvec.a:
	@$(MAKE) -sC $(dir $@)

obj/%.o: src/%.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	gcc -c $(INCLUDES) $(FLAGS) $< -o $@

clean:
	@echo "Removing objects of libraries..."
	@$(foreach lib, $(LIBS), $(MAKE) -sC $(dir $(lib)) fclean -j;)
	@echo "Removing objects directory..."
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "Removing libraries and game executable..."
	@$(foreach lib, $(LIBS), $(MAKE) -sC $(dir $(lib)) fclean -j;)
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean all re
