# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tbruinem <tbruinem@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/02/07 16:04:49 by tbruinem      #+#    #+#                  #
#    Updated: 2022/03/07 16:42:14 by tbruinem      ########   odam.nl          #
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
LINKER := $(foreach lib,$(LIBS),-L $(LIBDIR)/$(dir $(lib)) $(patsubst lib%.a,-l%,$(notdir $(lib)))) -ldl -lglfw
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
		Util.c

OBJ := $(addprefix $(OBJDIR)/, $(SRC:%.c=%.o))
SRC := $(addprefix $(SRCDIR)/, $(SRC))

FLAGS = -Wall -Wextra -Werror
ifdef DEBUG
	FLAGS += -g -fsanitize=address
endif

# all:
# 	echo $(LINKER)
all: $(NAME)

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
