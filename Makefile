NAME=ft_nm

BIN = bin
DIR_GUARD=@mkdir -p $(@D)

LIBFT=./libft/libft.a
FILE_NAMES = 	main \
				fileIo \
				elfMap 

OBJ_FILES = $(addprefix $(BIN)/, $(addsuffix .o , $(FILE_NAMES)))
CFLAGS=-std=c11
#-Werror -Wall -Wextra

all: $(NAME)

$(BIN)/%.o: ./src/%.c
	$(DIR_GUARD)
	gcc -c $< $(CFLAGS) -Iinclude -o $@

$(NAME): $(LIBFT) $(OBJ_FILES)
	gcc $(OBJ_FILES) $(CFLAGS) $(LIBFT) -Iinclude  -o $(NAME)

$(LIBFT):
	make -C ./libft/ bonus

clean:
	rm -rf $(BIN)
	make -C ./libft/ clean

fclean: clean
	rm -f $(NAME)
	make -C ./libft/ fclean

re: clean $(NAME)