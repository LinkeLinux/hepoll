#include <public.h>
#include <thread_pool.h>
threadpool_t *thread_init(int size)
{
    if(size <=0)
    {
        printf("threadpool num must more than 0\n");
        return NULL;
    }
    threadpool_t *tp;
    tp =(thread_pool *)malloc(sizeof(threadpool_t));
    if (pthread_mutex_init(&(tp->mutex),NULL)!=0){
        goto err;
    }
    if(pthread_cond_init(&(tp->cond),NULL)!=0)
    {
        pthread_mutex_destroy(&tp->mutex);
        goto err;
    }
    tp->head=NULL;
    return tp;
err:
    if(tp){
        free(tp);
    }
}
