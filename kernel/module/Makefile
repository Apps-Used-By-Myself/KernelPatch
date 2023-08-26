KP_DIR := ..

CFLAGS += -Wall -fno-builtin -std=gnu11
CFLAGS += -g

INCLUDE_DIRS := libc init/include linux linux/include linux/arch/arm64/include linux/tools/arch/arm64/include

INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(KP_DIR)/$(dir))

objs := testmain.o module.o

all: testmain

testmain: ${objs}
	${CC} -o $@ $^

%.o: %.c
	${CC} $(CFLAGS) $(INCLUDE_FLAGS) -c -O2 -o $@ $<

.PHONY: clean
clean:
	rm -rf testmain
	find . -name "*.o" | xargs rm -f