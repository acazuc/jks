NAME = libjks.a

CC = gcc
#CC = x86_64-w64-mingw32-gcc

CFLAGS = -Wall -Wextra -O2 -g

INCLUDES = include/

SRCS_PATH = src/

SRCS_NAME = array.c \
	    list.c \
	    hmap.c \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.c=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

TEST_PATH = test/

TEST_NAME = test.c \
	    array.c \
	    list.c \
	    hmap.c \

TEST_SRCS = $(addprefix $(TEST_PATH), $(TEST_NAME))

TEST_OBJS = $(TEST_SRCS:.c=.o)

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo "AR $(NAME)"
	@ar -rc $(NAME) $(OBJS)
	@ranlib $(NAME)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -o $@ -c $< -I$(INCLUDES)

$(TEST_PATH)%.o: $(TEST_PATH)%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -o $@ -c $< -I$(INCLUDES)

test: $(TEST_OBJS)
	@echo "CC test"
	@$(CC) $(CFLAGS) -o $(TEST_PATH)/test $^ -L . -ljks

odir:
	@mkdir -p $(OBJS_PATH)

clean:
	@rm -f $(OBJS)
	@rm -f $(NAME)

size:
	@wc `find $(SRCS_PATH) -type f \( -name \*.c -o -name \*.h \) ` | tail -n 1

objsize:
	@wc -c `find $(OBJS_PATH) -type f` | tail -n 1

.PHONY: clean odir test size objsize
