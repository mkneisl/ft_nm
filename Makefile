NAME=ft_nm

BIN = bin
DIR_GUARD=@mkdir -p $(@D)

LIBFT=./libft/libft.a
FILE_NAMES = 	main \
				fileIo \
				args \
				print \
				elfMap \
				symbols

MARCH_FILES = 	elfParse \
				symbolParse

OBJ_FILES = $(addprefix $(BIN)/, $(addsuffix .o , $(FILE_NAMES)))
MARCH_BIN = $(addprefix $(BIN)/march/, $(MARCH_FILES))
MARCH_OBJ_x64 = $(addsuffix x64.o, $(MARCH_BIN))
MARCH_OBJ_x32 = $(addsuffix x32.o, $(MARCH_BIN))
CFLAGS=-Werror -Wall -Wextra -std=c11

all: $(NAME)

$(BIN)/march/%: ./src/%.c
	$(DIR_GUARD)
	gcc -c $< $(CFLAGS) -Iinclude -Ilibft/include -DTARGET_ARCH=32 -o $(addsuffix x32.o , $@)
	gcc -c $< $(CFLAGS) -Iinclude -Ilibft/include -DTARGET_ARCH=64 -o $(addsuffix x64.o , $@)

$(BIN)/%.o: ./src/%.c
	$(DIR_GUARD)
	gcc -c $< $(CFLAGS) -Iinclude -Ilibft/include -DTARGET_ARCH=32 -o $@

$(NAME): $(LIBFT) $(OBJ_FILES) $(MARCH_BIN)
	gcc $(OBJ_FILES) $(MARCH_OBJ_x64) $(MARCH_OBJ_x32) $(CFLAGS) $(LIBFT) -Iinclude  -o $(NAME)

$(LIBFT):
	make -C ./libft/

clean:
	rm -rf $(BIN)
	make -C ./libft/ clean

fclean: clean
	rm -f $(NAME)
	make -C ./libft/ fclean

re: fclean $(NAME)