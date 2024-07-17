# name of the program (Minix service)
PROG=lab4 

# source code files to be compiled
SRCS = lab4.c mouse.c utils.c i8042.c
SRCS += ../lab2/timer.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies
DPADD += ${LIBLCF} mouse.h utils.h i8042.h
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

test:
	make
	@echo "LAB4: Running all tests"
	sleep 3
	lcom_run lab4 "packet 5 -t 0"
	sleep 1
	lcom_run lab4 "packet 5 -t 1"
	sleep 1
	lcom_run lab4 "packet 5 -t 2"
	sleep 1
	lcom_run lab4 "packet 5 -t 3"
	sleep 1
	lcom_run lab4 "packet 5 -t 4"
	sleep 1
	lcom_run lab4 "packet 5 -t 5"
	sleep 1
	lcom_run lab4 "async 2 -t 0"
	sleep 1
	lcom_run lab4 "async 2 -t 1"
	sleep 1
	lcom_run lab4 "async 2 -t 2"
	sleep 1
	lcom_run lab4 "async 2 -t 3"
	sleep 1
	lcom_run lab4 "async 2 -t 4"
	sleep 1
	lcom_run lab4 "async 2 -t 5"
	sleep 1
	lcom_run lab4 "gesture 50 5 -t 0"
	sleep 1
	lcom_run lab4 "gesture 50 5 -t 1"
	sleep 1
	lcom_run lab4 "gesture 50 5 -t 2"
	sleep 1
	lcom_run lab4 "gesture 50 5 -t 3"
	sleep 1
	lcom_run lab4 "gesture 50 5 -t 4"
	sleep 1
	lcom_run lab4 "gesture 50 5 -t 5"
	