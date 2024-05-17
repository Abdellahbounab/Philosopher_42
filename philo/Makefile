CC = cc

CFALGS = -Wall -Wextra -Werror -pthread

NAME = philo

SRCS = main.c mutix.c parent.c \
	simulation.c time.c utils.c

OBJS = ${SRCS:.c=.o}

INCLU = philo.h

all : 	${NAME}

${NAME} :	${OBJS}
			@${CC} ${CFALGS} ${OBJS} -o ${NAME}
			@echo "\033[32mexcution : ./philo \033[0m"

%.o:%.c ${INCLU}
			@${CC} ${CFALGS} -c $< -o $@


clean	:
		@rm -rf	${OBJS}
		@echo "\033[30m*.o files deleted\033[0m"

fclean	:	clean
		@rm -rf ${NAME}
		@echo "\033[30m./philo file is deleted\033[0m"

re	:	fclean all