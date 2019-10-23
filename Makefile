CFLAGS +=-lpthread -lrt 
INC= -I./include
OBJDIR=./obj
SRCDIR=./src
all:
	$(shell [ ! -d $(OBJDIR) ] && mkdir obj)
	make epoll
	make testclient
	
	make clean
epoll:$(OBJDIR)/epoll.o
	gcc -o $@ $? $(CFLAGS)
$(OBJDIR)/epoll.o:$(SRCDIR)/epoll.c
	gcc -c $? -o $@ $(INC) 

testclient:$(OBJDIR)/testclient.o
	gcc -o $@ $? $(CFLAGS)
$(OBJDIR)/testclient.o:$(SRCDIR)/testclient.c
	gcc -c $? -o $@ $(INC) 
clean:
	rm -rf obj
