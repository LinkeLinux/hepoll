#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_
#define POOL_SIZE 10
typedef struct task{
    void (*worker)(void *arg);
    void *arg;
    struct task *next;
}task_t;

typedef struct thread_pool{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t *threads;
    int task_num;
    int thread_size;
    task_t *task;
}threadpool_t;
#endif 
