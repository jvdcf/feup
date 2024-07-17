# name of the program (Minix service)
PROG=proj 

# Folders to compile
.PATH:${.CURDIR}/utils
.PATH:${.CURDIR}/timer
.PATH:${.CURDIR}/kbc
.PATH:${.CURDIR}/video
.PATH: ${.CURDIR}/serial

# source code files to be compiled
SRCS = proj.c model.c view.c controller.c# Project related files
SRCS += utils.c                          # Utils for controllers
SRCS += timer.c                          # Timer
SRCS += i8042.c keyboard.c mouse.c       # Keyboard and Mouse
SRCS += vbe.c video.c                    # Video card
SRCS += drv.c ringbuffer.c               # Serial port

# additional compilation flags ("-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set)
CFLAGS=-Wall -Wextra -Werror -std=gnu11 -Wno-unused-parameter -I.
CFLAGS+= -pedantic
CFLAGS+=-std=c11
CFLAGS += -D_LCOM_OPTIMIZED_ -O2       # Optimization flags
# CFLAGS += -DDEBUG												 # Extra debug messages from the project

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
