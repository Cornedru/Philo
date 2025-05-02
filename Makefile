NAME		= philo

CC			= cc
FLAGS		= -Wall -Wextra -Werror

SRCS_DIR	= srcs/
OBJS_DIR	= objects/


OBJS		= $(SRCS:.c=.o)
SRCS 		= main.c utils.c set.c utils2.c
SRCS_PREF	= $(addprefix $(SRCS_DIR), $(SRCS))
OBJS_PREF	= $(addprefix $(OBJS_DIR), $(OBJS))


$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	@cc $(FLAGS) -g -c $< -o $@

$(NAME): $(OBJS_PREF) 
	@cc $(FLAGS) $(OBJS_PREF) -g -o $(NAME)
	@echo "Program compiled\n"



all:  $(NAME)

clean:
	@rm -rf $(OBJS_DIR)
	@echo "Removing objects"

fclean: clean 
	@rm -f $(NAME)
	@echo "Cleaning all"

re: fclean all

.PHONY: all clean fclean re