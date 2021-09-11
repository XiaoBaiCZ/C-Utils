#include"b_queue.h"
#include<stdlib.h>
#include<stdio.h>

void block_queue_init(block_queue *queue, int limit_def, char isExpand) {
    queue->limit = limit_def;
    queue->head = 0;
    queue->tail = 0;
    queue->isExpand = isExpand;
    queue->src = calloc(queue->limit, sizeof(void *));
    queue->cond = malloc(sizeof(pthread_cond_t));
    queue->mutex = malloc(sizeof(pthread_mutex_t));
    pthread_cond_init(queue->cond, NULL);
    pthread_mutex_init(queue->mutex, NULL);
}

int block_queue_free(block_queue *queue) {
    queue->limit = 0;
    queue->head = 0;
    queue->tail = 0;
    queue->isExpand = 0;
    free(queue->src);
    free(queue->cond);
    free(queue->mutex);
    queue->src = NULL;
    queue->cond = NULL;
    queue->mutex = NULL;
}

void block_queue_push(block_queue *queue, void *val) {
    pthread_mutex_lock(queue->mutex);
    while (queue->size == queue->limit) {
        if (queue->isExpand) {
            _block_queue_expand(queue);
            continue;
        }
        pthread_cond_wait(queue->cond, queue->mutex);
    }
    queue->size++;
    queue->src[queue->tail] = val;
    queue->tail = (queue->tail + 1) % queue->limit;
    pthread_cond_signal(queue->cond);
    pthread_mutex_unlock(queue->mutex);
}

void *block_queue_pop(block_queue *queue) {
    pthread_mutex_lock(queue->mutex);
    while (queue->size == 0) {
        pthread_cond_wait(queue->cond, queue->mutex);
    }
    queue->size--;
    void *val = queue->src[queue->head];
    queue->head = (queue->head + 1) % queue->limit;
    pthread_cond_signal(queue->cond);
    pthread_mutex_unlock(queue->mutex);
    return val;
}

void block_queue_iteration(block_queue *queue, block_queue_iterator iterator, void *ext) {
    pthread_mutex_lock(queue->mutex);
    for (int i = 0; i < queue->size; i++) {
        iterator(i, queue->src[(i + queue->head) % queue->limit], ext);
    }
    pthread_mutex_unlock(queue->mutex);
}

void _block_queue_expand(block_queue *queue) {
    const int old_limit = queue->limit;
    //扩容
    queue->limit <<= 1;
    queue->tail += old_limit;
    queue->src = realloc(queue->src, sizeof(void *) * queue->limit);
    if (queue->head == 0) {
        //正序，无需重排
        return;
    }
    //重排，容器大小改变影响数据存取
    for (int i = queue->head - 1; i >= 0; i--) {
        queue->src[i + old_limit] = queue->src[i];
    }
}