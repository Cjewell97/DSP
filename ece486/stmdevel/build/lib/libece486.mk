ROOTDIR = ..
include $(ROOTDIR)/Makefile.inc

# Default Architecture 
# (normally overridden by make command line: "make ARCH=STM32L476G -f thisfile")
ARCH = STM32L476xx

TARGET=libece486_$(ARCH).a

RAWOBJS = init486.o \
          sample486.o \
          err486.o \
          usart.o \
          syscalls.o \
          opamp486.o

OBJS = $(addsuffix .$(ARCH), $(RAWOBJS))

VPATH = $(ROOTDIR)/src/libece486/

CFLAGS = $(PRJCFLAGS) -D$(ARCH) -I. -I$(INCLUDEDIR)

.PHONY : clean all depend install uninstall

all: $(TARGET)

%.o.$(ARCH) : %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.o.$(ARCH): %.s
	$(COMPILE.s) -o $@ $<

${TARGET}: ${OBJS}
	${AR} rc ${TARGET} ${OBJS}
	${RANLIB} ${TARGET}

install: $(TARGET)
	mkdir -p $(INSTALLLIB)
	cp $(TARGET) $(INSTALLLIB)

uninstall:
	rm $(INSTALLLIB)/$(TARGET)

clean:
	rm -f *.o.$(ARCH) ${TARGET}
