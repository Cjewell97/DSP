TARGET = main
OBJS = running_mean.o main.o
CFLAGS = -g -Wall -O2

.PHONY:clean

all: $(TARGET)

$(TARGET): $(OBJS)
	     $(CC) -o $(TARGET) $(OBJS) $(LIBS)

clean: 
	rm -f $(TARGET) $(OBJS) core*

running_mean.o main.o: running_mean.h
