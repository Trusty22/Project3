#include "schedulers.h"
#include "list.h"
#include "task.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>


struct node* list;
int tid = 0;
int count = 0;


void add(char* name, int priority, int burst) {
  Task* toAdd = malloc(sizeof(Task));
  toAdd->name = name;
  toAdd->priority = priority;
  toAdd->burst = burst;
  toAdd->tid = tid++;
  count++;
  //insert_end_q(&list, toAdd);

  // printf("%s\t\t%d\t\t%d\t\t%d\n", toAdd->name, toAdd->priority, toAdd->burst, toAdd->tid);
}

void schedule() {
  struct node* temp = list;
  struct node* prev = list;
  int ct = 0;
  double currTime = 0;
  double turnaround = 0;
  double wait = 0;
  double response = 0;
  while (prev->next != list) {
    prev = prev->next;
  }
  while (temp->next && temp != temp->next) {
    ct++;
    run(temp->task, (temp->task->burst < QUANTUM) ? temp->task->burst : QUANTUM);
    if ((temp->task->burst - QUANTUM) < 1) {
      tid--;
      wait += tid * temp->task->burst;
      currTime += temp->task->burst;
      turnaround += currTime;
      struct node* t = temp;
      prev->next = temp->next;
      temp = temp->next;
      free(t->task);
      free(t);
    }
    else {
      wait += (tid - 1) * QUANTUM;
      currTime += QUANTUM;
      temp->task->burst -= QUANTUM;
      temp = temp->next;
      prev = prev->next;
      if (ct < count) {
        response += currTime;
      }
    }
  }
  run(prev->task, prev->task->burst);
  currTime += prev->task->burst;
  turnaround += currTime;
  turnaround /= count;
  wait /= count;
  response /= count;
  printf("\nAvg wait: %.3f\nAvg response: %.3f\nAvg turnaround: %.3f\n", wait, response, turnaround);
}