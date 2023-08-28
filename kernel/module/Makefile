KP_DIR := ..

CFLAGS += -Wall -std=gnu11
CFLAGS += -g

INCLUDE_DIRS := libc init/include linux linux/include linux/arch/arm64/include linux/tools/arch/arm64/include

INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(KP_DIR)/$(dir))

srcs := testmain.c module.c

all: testmain

testmain: ${srcs}
	${CC} $(CFLAGS) $(INCLUDE_FLAGS) -o $@ $^


.PHONY: clean
clean:
	rm -rf testmain
	find . -name "*.o" | xargs rm -f