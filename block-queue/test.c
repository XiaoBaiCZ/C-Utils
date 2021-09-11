#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"b_queue.h"

block_queue q;

void *run(void *);

void test_block();

void test_iteration();

void iterator(int index, void *val, void *ext);

int main(void) {

    block_queue_init(&q, 32, 0);

    test_block();

    // test_iteration();

    return 0;
}

void test_block() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, run, NULL);
    pthread_create(&t2, NULL, run, NULL);

    while (1) {
        void *val = block_queue_pop(&q);
        printf("pop: %d; size: %d\n", *(int *)val, q.size);
        free(val);
    }
};

void test_iteration() {
    char *str = "ok";
    int i1 = 1, i2 = 2, i3 = 3, i4 = 4, i5 = 5, i6 = 6;
    block_queue_push(&q, &i1);
    block_queue_push(&q, &i2);
    block_queue_push(&q, &i3);
    block_queue_push(&q, &i4);
    block_queue_push(&q, &i5);
    block_queue_push(&q, &i6);

    block_queue_iteration(&q, iterator, str);
}

void iterator(int index, void *val, void *ext) {
    printf("%s %02d: %d\n", (char *)ext, index, *(int *)val);
}

int sum = 0;

void *run(void *args) {
    pthread_t t = pthread_self();
    while (1) {
        int *val = malloc(sizeof(int));
        *val = sum++;
        block_queue_push(&q, val);
        printf("push: 0x%lx: %d\n", t % 0xffff, *(int *)val);
    }
    return NULL;
}