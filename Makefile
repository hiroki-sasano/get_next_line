# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/26 15:56:25 by hisasano          #+#    #+#              #
#    Updated: 2025/05/27 01:22:56 by hisasano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := get_next_line.a

SRC_M  := get_next_line.c \
        get_next_line_utils.c
		   
SRC_B  := get_next_line_bonus.c \
           get_next_line_utils_bonus.c

SRCDIR   := src
BONUSDIR := bonus_src
OBJDIR   := obj
INCDIR   := include

CC      := cc
CFLAGS  := -Wall -Wextra -Werror
AR      := ar
ARFLAGS := rcs
RM      := rm -f

MANDATORY_SRCS := $(addprefix $(SRCDIR)/,$(SRC_M))
BONUS_SRCS     := $(addprefix $(BONUSDIR)/,$(SRC_B))

ifeq ($(filter bonus test_bonus testrun_bonus,$(MAKECMDGOALS)),)
	SRCS := $(MANDATORY_SRCS)
else
	SRCS := $(BONUS_SRCS)
endif

OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.c=.o)))

vpath %.c $(SRCDIR) $(BONUSDIR)

all: $(NAME)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	$(RM) $(OBJS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: $(NAME)

test: re
	$(CC) -I$(INCDIR) 
	     main_basic.c $(NAME) $(LIBFT) -o test_mandatory

testrun: test
	./test_mandatory

test_bonus: fclean bonus
	$(CC) -I$(INCDIR) main_bonus.c $(NAME) -o test_bonus

testrun_bonus: test_bonus
	./test_bonus

TEST_BSIZE_SMALL := 1
TEST_BSIZE_MIDDLE := 9999
TEST_BSIZE_LARGE := 10000000

# 小バッファサイズ（mandatory + main_basic）
test_small: fclean
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(TEST_BSIZE_SMALL) \
		-I$(INCDIR) $(MANDATORY_SRCS) main_basic.c -o gnl_test_small

run_small: test_small
	./gnl_test_small test_long.txt

test_middle: fclean
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(TEST_BSIZE_MIDDLE) \
		-I$(INCDIR) $(MANDATORY_SRCS) main_basic.c -o gnl_test_middle

run_middle: test_middle
	./gnl_test_middle test_long.txt

test_large: fclean
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(TEST_BSIZE_LARGE) \
		-I$(INCDIR) $(MANDATORY_SRCS) main_basic.c -o gnl_test_large

run_large: test_large
	./gnl_test_large test_long.txt

test_small_b: fclean
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(TEST_BSIZE_SMALL) \
		-I$(INCDIR) $(BONUS_SRCS) main_bonus.c -o gnl_test_small

run_small_b: test_small_b
	./gnl_test_small test_long.txt

test_middle_b: fclean
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(TEST_BSIZE_MIDDLE) \
		-I$(INCDIR) $(BONUS_SRCS) main_bonus.c -o gnl_test_middle

run_middle_b: test_middle_b
	./gnl_test_middle test_long.txt

test_large_b: fclean
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(TEST_BSIZE_LARGE) \
		-I$(INCDIR) $(BONUS_SRCS) main_bonus.c -o gnl_test_large

run_large_b: test_large_b
	./gnl_test_large test_long.txt

.PHONY:  all bonus clean fclean re test testrun test_bonus\
		testrun_bonus test_small run_small test_middle run_middle\
		test_large run_large\ test_small_b run_small_b test_middle_b\
		run_middle_b test_large_b run_large_b
