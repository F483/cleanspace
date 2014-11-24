# Copyright (c) 2013 Fabian Barkhau <fabian.barkhau@gmail.com>                   
# License: MIT (see LICENSE.TXT file)                                            


MODE = debug
EMULATOR = vba
GAME = cleanspace


#########
# PATHS #
#########

PATH_DEVKITPRO = $(shell echo ~)/devkitPro
PATH_DEVKITARM = $(PATH_DEVKITPRO)/devkitARM
LIB_PATHS = -L$(PATH_DEVKITARM)/lib


#########
# FILES #
#########

ROM = pkg/$(GAME).gba

# c source files
C_SRC := $(shell find | grep -i "src/.*\.c$$") # source
C_SRC += $(shell find | grep -i "gfx/.*\.c$$") # graphics
C_SRC += $(shell find | grep -i "snd/.*\.c$$") # sound

# asm source files
ASM_SRC := $(shell find | grep -i "src/.*\.s$$") # source
ASM_SRC += $(shell find | grep -i "gfx/.*\.s$$") # graphics
ASM_SRC += $(shell find | grep -i "snd/.*\.s$$") # sound


#########
# FLAGS #
#########

CFLAGS := -std=c99 -Wall -Wextra -Werror -ffast-math -O3
CFLAGS += -D$(shell echo $(MODE) | tr '[a-z]' '[A-Z]')
CFLAGS += -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork
LDFLAGS := -specs=gba.specs -mthumb -mthumb-interwork
ASFLAGS = -x assembler-with-cpp -mthumb-interwork -mthumb

ifeq ($(MODE), debug)
CFLAGS += -g
LDFLAGS += -g -Wl,-Map,$(GAME).elf.map
endif


############
# COMPILER #
############

CC = gcc
OBJCOPY = objcopy
CC = $(PATH_DEVKITARM)/bin/arm-none-eabi-gcc
AS = $(PATH_DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY = $(PATH_DEVKITARM)/bin/arm-none-eabi-objcopy


########
# HELP #
########

help:
	@echo "                                                                      "
	@echo "Usage:                                                                "
	@echo "  make <command> <option>=<value> ... <option>=<value>                "
	@echo "                                                                      "
	@echo "Commands:                                                             "
	@echo "  help           Display this help text.                              "
	@echo "  clean          Remove compiled files and savegame.                  "
	@echo "  run            Run game.                                            "
	@echo "  build          Compile game.                                        "
	@echo "  build_and_run  Combines build and run commands.                     "
	@echo "                                                                      "
	@echo "Options:         First option listed is the default.                  "
	@echo "  MODE           debug|release                                        "
	@echo "                                                                      "

_compile_c:
	@$(foreach SRC,$(C_SRC), \
		$(CC) $(CFLAGS) -I . -c -o $(SRC:.c=.o) $(SRC);\
	)

_compile_asm:
	@$(foreach SRC,$(ASM_SRC), \
		$(AS) -MMD -MP -MF $(SRC:.s=.d) $(ASFLAGS) -c $(SRC) -o $(SRC:.s=.o);\
	)

_link: _compile_c _compile_asm
	@$(CC) $(LDFLAGS) $(shell find | grep -i ".*\.o$$") -o $(GAME).elf

build: clean _link
	@$(OBJCOPY) -O binary $(GAME).elf $(ROM)
	@$(PATH_DEVKITARM)/bin/gbafix $(ROM)

run:
	@$(EMULATOR) $(ROM)

build_and_run: build run
	@echo ""

clean:
	@find | grep -i ".*\.o$$" | xargs -r -L1 rm
	@find | grep -i ".*\.d$$" | xargs -r -L1 rm
	@find | grep -i ".*\.map$$" | xargs -r -L1 rm
	@find | grep -i ".*\.elf$$" | xargs -r -L1 rm
	@find | grep -i ".*\.sav$$" | xargs -r -L1 rm
	@find | grep -i ".*\.gba$$" | xargs -r -L1 rm

