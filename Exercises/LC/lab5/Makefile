# name of the program (Minix service)
PROG=lab5 

# source code files to be compiled
SRCS = lab5.c video.c vbe.c utils.c 
SRCS += ../lab3/keyboard.c ../lab2/timer.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

test:
	make
	clear
	@echo "LAB5: Running selected test"
	sleep 1
	# lcom_run lab5 "init 14C 2 -t 1"
	lcom_run lab5 "rectangle 14C 100 100 200 200 FFFFFF -t 1"
	@echo "TODO: Add tests"
	