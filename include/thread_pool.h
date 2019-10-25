#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_
typedef struct task{
    
    struct task *next;
}task_t;

typedef struct thread_pool{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    task_t *head;
    int free;
    int use;
    int thread_size;
}threadpool_t;
#endif 
