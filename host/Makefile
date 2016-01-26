CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

OBJS = main.o

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT)/include -I./include
LDADD += -lteec -lpthread -L$(TEEC_EXPORT)/lib

BINARY=template_ta_host

.PHONY: all
all: $(BINARY)

template_ta_host: $(OBJS)
	$(CC) $(LDADD) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY)
