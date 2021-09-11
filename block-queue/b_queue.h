#ifndef BLOCK_QUEUE
#define BLOCK_QUEUE

#include<pthread.h>

//阻塞队列
typedef struct block_queue {
    //容器上限
    int limit;
    //元素个数
    int size;
    //元素容器
    void **src;
    //头下标
    int head;
    //尾下标
    int tail;
    //是否可扩容
    char isExpand;
    //条件量
    pthread_cond_t *cond;
    //互斥体
    pthread_mutex_t *mutex;
} block_queue;

//迭代器
typedef void (*block_queue_iterator)(int index, void *val, void *ext);

//初始化队列
void block_queue_init(block_queue *queue, int limit_def, char isExpand);

//队列释放
int block_queue_free(block_queue *queue);

//入队
void block_queue_push(block_queue *queue, void *val);

//出队
void *block_queue_pop(block_queue *queue);

//元素个数
int block_queue_size(block_queue *queue);

//队列迭代遍历
void block_queue_iteration(block_queue *queue, block_queue_iterator iterator, void *ext);

//扩容，队满时双倍扩展
void _block_queue_expand(block_queue *queue);

#endif