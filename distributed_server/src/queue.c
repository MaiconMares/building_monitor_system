#include "../inc/queue.h"

#include <stdio.h>
#include <stdlib.h>
#define LIMIT 1000

int queue[LIMIT];
static int front = -1, rear = -1;
int i;

void insert(int msg_code) {
  if (rear == LIMIT - 1)
    printf("Queue Overflow\n");
  else {
    if (front == -1)
      front = 0;
    rear++;
    queue[rear] = msg_code;
  }
}

int remove_element() {
  if (front == -1 || front > rear) {
    printf("Queue Underflow \n");
  } else {
    return queue[front++];
  }
  return -1;
}

int is_empty(){
  if (front == -1 || front > rear)
    return 1;
  else
    return 0;
}
