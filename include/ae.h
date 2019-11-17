#ifndef __AE_H__
#define __AE_H__

#define AE_NONE 0
#define AE_READABLE 1
#define AE_WRITEABLE 2

struct aeEventLoop;

typedef (void *aeFileProc)(struct aeEventLoop *,int fd,void *clientData,int mask);
typedef struct aeFileEvent{
	//READABLE|WRITEABLE
	int mask;
	aeFileProc *rfileProc;
	aeFileProc *wfileProc;
	void *priData;
};
typedef struct aeEventLoop{
	
	int epfd;
	int maxFd;
	int setsize;
	aeFileEvent *events;
	
}aeEventLoop;
#endif 
