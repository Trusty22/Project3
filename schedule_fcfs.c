#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>

struct node *list;
int tid = 0;

void add(char *name, int priority, int burst) {
  Task *toAdd = malloc(sizeof(Task));
  toAdd->name = name;
  toAdd->priority = priority;
  toAdd->burst = burst;
  toAdd->tid = tid++;
  //  insert_end(&list, toAdd);

  // printf("%s\t%d\t%d\t%d\n", toAdd->name, toAdd->priority, toAdd->burst, toAdd->tid);
}

void schedule() {
  double currTime = 0;   // time elapsed before handling current process
  double turnaround = 0; // burst + completion time
  double wait = 0;       // turnaround time - burst time
  double response = 0;   // start time
  struct node *curr = list;
  while (curr) {
    wait += currTime;
    response += currTime;
    run(curr->task, curr->task->burst);
    currTime += curr->task->burst;
    turnaround += currTime;
    curr = curr->next;
  }
  struct node *temp;
  // I'm a responsible C programmer so I clean up after myself
  while (list) {
    temp = list;
    list = list->next;
    free(temp->task);
    free(temp);
  }
  turnaround /= tid;
  wait /= tid;
  response /= tid;
  printf("Avg wait: %.3f\nAvg response: %.3f\nAvg turnaround: %.3f\n", wait, response, turnaround);
  return;
}