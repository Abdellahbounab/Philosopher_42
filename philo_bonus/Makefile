CC = cc

CFALGS = -Wall -Wextra -Werror -pthread

NAME = philo_bonus

SRCS = main_bonus.c philosophers_bonus.c sema_bonus.c \
		simulation_bonus.c thread_bonus.c timer_bonus.c \
		utils_bonus.c

OBJS = ${SRCS:.c=.o}

INCLU = philo_bonus.h

all : 	${NAME}

${NAME} :	${OBJS}
			@${CC} ${CFALGS} ${OBJS} -o ${NAME}
			@echo "\033[32mexcution : ./philo_bonus \033[0m"

%.o:%.c ${INCLU}
			@${CC} ${CFALGS} -c $< -o $@

clean	:
		@rm -rf	${OBJS}
		@echo "\033[30m*.o files deleted\033[0m"


fclean	:	clean
		@rm -rf ${NAME}
		@echo "\033[30m./philo file is deleted\033[0m"

re	:	fclean all