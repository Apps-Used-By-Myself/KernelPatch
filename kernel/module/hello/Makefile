ifndef TARGET_COMPILE
    $(error TARGET_COMPILE is not set)
endif

CC = $(TARGET_COMPILE)gcc
LD = $(TARGET_COMPILE)ld

KP_DIR := ../..

INCLUDE_DIRS := include libc init/include linux linux/include linux/arch/arm64/include linux/tools/arch/arm64/include

INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(KP_DIR)/$(dir))

objs := hello.o

all: hello.kpm

hello.kpm: ${objs}
	${CC} -r -o $@ $^

%.o: %.c
	${CC} $(CFLAGS) $(INCLUDE_FLAGS) -c -O2 -o $@ $<

.PHONY: clean
clean:
	rm -rf hello.kpm
	find . -name "*.o" | xargs rm -f