CFLAGS +=-lpthread -lrt 
INC= -I./include
OBJDIR=./obj
SRCDIR=./src
all:
	$(shell [ ! -d $(OBJDIR) ] && mkdir obj)
	#make epoll
	make testclient
	make thread_pool
	make clean
thread_pool:$(OBJDIR)/thread_pool.o
	gcc -o $@ $? $(CFLAGS)
epoll:$(OBJDIR)/epoll.o $(OBJDIR)/thread_pool.o
	gcc -o $@ $? $(CFLAGS)
$(OBJDIR)/epoll.o:$(SRCDIR)/epoll.c
	gcc -c $? -o $@ $(INC) 
$(OBJDIR)/thread_pool.o:$(SRCDIR)/thread_pool.c
	gcc -c $? -o $@ $(INC) 

testclient:$(OBJDIR)/testclient.o
	gcc -o $@ $? $(CFLAGS)
$(OBJDIR)/testclient.o:$(SRCDIR)/testclient.c
	gcc -c $? -o $@ $(INC) 
clean:
	rm -rf obj
