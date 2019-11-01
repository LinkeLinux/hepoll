#include <public.h>
#include <thread_pool.h>
void *thread_worker(void *arg);
threadpool_t *thread_init(int size)
{
    int i;
    if(size <=0)
    {
        printf("threadpool num must more than 0\n");
        return NULL;
    }
    threadpool_t *tp;
    tp =(threadpool_t *)malloc(sizeof(threadpool_t));
    if (pthread_mutex_init(&(tp->lock),NULL)!=0){
        goto err;
    }
    if(pthread_cond_init(&(tp->cond),NULL)!=0)
    {
        goto err;
    }
    tp->threads= (pthread_t *)malloc(sizeof(pthread_t)*size);
    tp->task=NULL;
    tp->thread_size =size;
    tp->task_num =0;
    for(i=0;i<size;i++){
        if(pthread_create(&(tp->threads[i]),NULL,thread_worker,(void *)tp)){
            printf("pthread_create error\n");
            goto err;
        }
    
    }
    
    return tp;
err:
    pthread_mutex_destroy(&tp->lock);
    pthread_cond_destroy(&tp->cond);
    if(tp){
        free(tp);
    }
    return NULL;
}
int thread_add(threadpool_t *pool,void (*func)(void *),void *arg)
{
    if (pool==NULL ||func ==NULL)
    {
        printf("pool==NULL ||func ==NULL\n");
        return -1;
    }
    pthread_mutex_lock(&(pool->lock));
    task_t *task =(task_t *)malloc(sizeof(task_t));
    if(task ==NULL)
    {
        printf("malloc failed\n");
        return -1;
    }
    task->worker =func;
    task->arg=arg;
    task->next=pool->task;
    pool->task=task;
    pool->task_num ++;
    printf("pool->task_num=%d,size:%d\n",pool->task_num,pool->thread_size);
    pthread_cond_signal(&(pool->cond));
    pthread_mutex_unlock(&(pool->lock));
    return 0; 
}
void *thread_worker(void *arg)
{
    task_t *task;
    threadpool_t *pool =(threadpool_t *)arg;
    while(1){
        pthread_mutex_lock(&pool->lock);
        if(pool->task_num ==0){
            printf("%s wait\n",__FUNCTION__);
            pthread_cond_wait(&pool->cond,&pool->lock);
        }
        
        task=pool->task;
        if (task ==NULL){
            printf("task is NULL,continue wait\n");
            pthread_mutex_unlock(&pool->lock);
            continue;
        }
        pool->task=task->next;
        pool->task_num --;
        task->worker(task->arg); 
        free(task);
        task=NULL;
        pthread_mutex_unlock(&pool->lock);
    }
}
void print(void *arg)
{
    char *fd;
    char buf[512];
    memset(buf,0x00,sizeof(buf));
    fd =fopen("/root/merge_sort.c","r");
    fread(buf,1,sizeof(buf),fd);
    close(fd);
    printf("read %s\n",buf);
}
int main()
{
    threadpool_t *tp;
    tp =thread_init(4);
    int cnt=100000;
    while(cnt--){
        thread_add(tp,print,NULL);
    }
    sleep(10);
    return 0;
}
