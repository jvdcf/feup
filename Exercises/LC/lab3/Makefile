# name of the program (Minix service)
PROG=lab3 

# source code files to be compiled
SRCS = lab3.c keyboard.c utils.c
SRCS += ../lab2/timer.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB3

# list of library dependencies
DPADD += ${LIBLCF} keyboard.h i8042.h
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

test:
		make
		@echo "LAB3: Executing all tests:"
		sleep 3
		lcom_run lab3 "scan -t 0"
		sleep 1
		lcom_run lab3 "scan -t 1"
		sleep 1
		lcom_run lab3 "scan -t 2"
		sleep 1
		lcom_run lab3 "scan -t 3"
		sleep 1
		lcom_run lab3 "scan -t 4"
		sleep 1
		lcom_run lab3 "scan -t 5"
		sleep 1
		lcom_run lab3 "poll -t 0"
		sleep 1
		lcom_run lab3 "poll -t 1"
		sleep 1
		lcom_run lab3 "poll -t 2"
		sleep 1
		lcom_run lab3 "poll -t 3"
		sleep 1
		lcom_run lab3 "poll -t 4"
		sleep 1
		lcom_run lab3 "poll -t 5"
		sleep 1
		lcom_run lab3 "timed 2 -t 0"
		sleep 1
		lcom_run lab3 "timed 2 -t 1"
		sleep 1
		lcom_run lab3 "timed 2 -t 2"
		sleep 1
		lcom_run lab3 "timed 2 -t 3"
		sleep 1
		lcom_run lab3 "timed 2 -t 4"
		sleep 1
		lcom_run lab3 "timed 2 -t 5"
		sleep 1
		lcom_run lab3 "timed 2 -t 6"
		sleep 1
		lcom_run lab3 "timed 2 -t 7"
		sleep 1
		lcom_run lab3 "timed 2 -t 8"
		sleep 1
		lcom_run lab3 "timed 2 -t 9"
		sleep 1
		lcom_run lab3 "timed 2 -t 10"
