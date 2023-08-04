PROG = read-timeout
CFLAGS += -g -O2 -Wall
CFLAGS += -std=gnu99
# CFLAGS += -pthread
# LDLIBS += -L/usr/local/lib -lmylib
# LDLIBS += -lrt
# LDFLAGS += -pthread

all: $(PROG)
OBJS += $(PROG).o
OBJS += my_socket.o
OBJS += logUtil.o
OBJS += log_et.o
$(PROG): $(OBJS)

clean:
	rm -f *.o $(PROG)
