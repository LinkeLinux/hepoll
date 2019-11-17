#include <public.h>
#include <ae.h>

aeEventLoop *InitaeEventLoop(int setsize)
{
	aeEventLoop *et;
	et =(aeEventLoop*)malloc(sizeof(aeEventLoop));
	et ->maxFd =0;
	et ->events =malloc(sizeof(aeFileEvent)*setsize);
	et ->setsize =setsize;
	return et;
}
int CreatFileEvent(aeEventLoop *eventLoop, int fd, int mask,
        aeFileProc *proc, void *clientData)
{
	if (fd >=eventLoop->fd)
	{
		return -1;
	}
	if (AddaeEvent(eventLoop,fd,mask) <0)
	{
		return -1;
	}
	aeFileEvent *fe =&eventLoop ->events[fd];
	fe ->mask |= mask;
	if (mask & AE_READABLE)
	{
		fe ->rfileProc =proc;
	}
	if (mask & AE_WRITEABLE)
	{
		fe ->wfileProc =proc;
	}
	fe ->priData =clientData;
	if (fd > eventLoop ->maxFd)
	{
		eventLool ->maxFd =fd;
	}
	return 0;
}
