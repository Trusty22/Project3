#include "schedulers.h"
#include "list.h"
#include "task.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

struct node* list;
struct node* queues[10];
int tid = 0;
int count = 0;


void add(char* name, int priority, int burst) {
  Task* toAdd = malloc(sizeof(Task));
  toAdd->name = name;
  toAdd->priority = priority;
  toAdd->burst = burst;
  toAdd->tid = tid++;
  count++;
  insert(&list, toAdd);
}

void schedule() {
  struct node* curr = list;
  struct node* prev;
  int subtid[MAX_PRIORITY] = {};
  double currTime = 0;
  double turnaround = 0;
  double wait = 0;
  double response = 0;

  // put nodes in the appropriate bucket
  while (curr) {
    //insert_end_q(&(queues[curr->task->priority - 1]), curr->task);
    subtid[curr->task->priority - 1]++;
    curr = curr->next;
  }


  // for (int i = 0; i < MAX_PRIORITY; i++) {
  //   if (queues[i]) {
  //     traverse(queues[i]);
  //     printf("%d %d\n", i, subtid[i]);
  //   }
  // }
  // printf("________________________________________\n");
  // return;



  // time to RR :)
  for (int i = MAX_PRIORITY - 1; i >= 0; i--) {
    if (subtid[i] == 0) {
      continue;
    }
    int ct = 0;
    int cnt = subtid[i];
    curr = queues[i];
    prev = queues[i];
    for (int j = 0; j < subtid[i]; j++) {
      prev = prev->next;
    }
    while (curr->next && curr != curr->next) {
      ct++;
      run(curr->task, (curr->task->burst < QUANTUM) ? curr->task->burst : QUANTUM);
      if ((curr->task->burst - QUANTUM) < 1) {

        subtid[i]--;
        wait += subtid[i] * curr->task->burst;
        currTime += curr->task->burst;
        turnaround += currTime;
        // struct node* t = curr;
        prev->next = curr->next;
        curr = curr->next;
        // free(t->task);
        // free(t);
      }
      else {
        wait += (subtid[i] - 1) * QUANTUM;
        currTime += QUANTUM;
        curr->task->burst -= QUANTUM;
        curr = curr->next;
        prev = prev->next;
        if (ct < cnt) {
          response += currTime;
        }
      }
    }
    run(prev->task, prev->task->burst);
    currTime += prev->task->burst;
    turnaround += currTime;
  }
  turnaround /= count;
  wait /= count;
  response /= count;
  printf("\nAvg wait: %.3f\nAvg response: %.3f\nAvg turnaround: %.3f\n", wait, response, turnaround);
}