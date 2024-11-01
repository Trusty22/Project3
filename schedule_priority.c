#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include "task.h"

#include "list.h"

#include "schedulers.h"

#include "cpu.h"

struct node *root = NULL;
struct node *cur = NULL;
struct node *newNode = NULL;
int tru = 1;

void add(char *name, int priority, int burst) {

  if (root == NULL) {
    root = malloc(sizeof(struct node));
    root->task = malloc(sizeof(struct task));

    root->task->name = name;
    root->task->burst = burst;
    root->task->priority = priority;

    root->next = NULL;

    cur = root;
  } else {

    newNode = malloc(sizeof(struct node));
    newNode->task = malloc(sizeof(struct task));

    newNode->task->burst = burst;
    newNode->task->name = name;
    newNode->task->priority = priority;

    if (!cur->next) {
      if ((cur->task->priority > newNode->task->priority) || (cur->task->priority == newNode->task->priority)) {

        newNode->next = NULL;

      } else {
        newNode->next = cur;
        root = newNode;
        cur = newNode;
      }
    } else {

      while (tru) {

        if ((cur->next->task->priority < newNode->task->priority)) {

          if (cur->task->priority > newNode->task->priority) {

            newNode->next = cur->next;
            cur->next = newNode;
            cur = root;
            break;
          } else if (cur->task->priority < newNode->task->priority) {

            root = newNode;
            newNode->next = cur;
            cur = root;
            break;
          } else if (cur->task->priority == newNode->task->priority) {

            newNode->next = cur->next;
            cur->next = newNode;
            break;
          }

        } else if (cur->next->task->priority == newNode->task->priority) {
          cur = cur->next;

          if (cur->next == NULL) {

            newNode->next = NULL;

          } else if (newNode->task->priority == cur->next->task->priority) {

            cur = cur->next;
            newNode->next = cur->next;

          } else {

            newNode->next = cur->next;
          }

          cur->next = newNode;

          cur = root;
          break;
        } else if ((newNode->task->priority) < (cur->next->task->priority)) {
          cur = cur->next;

          if (cur->next == NULL) {
            cur->next = newNode;
            newNode->next = NULL;
            cur = root;
            break;
          }
        }
      }
    }
  }
}

// invoke the scheduler
void schedule() {
  int num = 0;
  float ResponseTime = 0;
  float turnaroundtime = 0;
  float WaitTime = 0;
  int burst = 0;

  struct node *ref = root;
  while (ref != NULL) {
    num = num + 1;
    run(ref->task, ref->task->burst);
    burst = burst + ref->task->burst;
    turnaroundtime = turnaroundtime + burst; // 5 (5+10+5)20  50      (5 + 5+10 + 5+10+15)
    if (ref->next != NULL) {
      ResponseTime = ResponseTime + burst;
    }
    ref = ref->next;
  }

  WaitTime = turnaroundtime - burst;

  printf("The average turnaround time is : %f time units \n", (float)turnaroundtime / num);
  printf("The average ResponseTime is : %f time units \n", (float)ResponseTime / num);
  printf("The average WaitTime is : %f time units\n ", (float)WaitTime / num);
}