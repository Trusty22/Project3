#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include "task.h"

#include "list.h"

#include "schedulers.h"

#include "cpu.h"

struct node *root = NULL;
struct node *end = NULL;
struct node *newNode = NULL;
struct node *test = NULL;
int num = 0;

void add(char *name, int priority, int burst) {
  num = num + 1;
  if (root == NULL) {
    root = malloc(sizeof(struct node));
    end = malloc(sizeof(struct node));
    root->task = malloc(sizeof(struct task));

    root->task->burst = burst;
    root->task->name = name;
    root->task->priority = priority;
    root->next = NULL;

    end = root;
  } else {

    newNode = malloc(sizeof(struct node));
    end->next = newNode;
    newNode->task = malloc(sizeof(struct task));
    newNode->task->name = name;
    newNode->task->burst = burst;
    newNode->task->priority = priority;
    newNode->next = NULL;
    end = newNode;
  }
}

// invoke the scheduler
void schedule() {

  struct node *current = root;
  struct node *ref = root;
  struct node *monitor = NULL;
  monitor = malloc(sizeof(struct node));
  monitor = root;
  int newNodeburst = 0;
  int totburst = 0;
  float turnaroundtime = 0;
  float ResponseTime = 0;
  float WaitTime = 0;
  

  while (ref != NULL) {

    if (ref->task->burst >= 10) {

      newNodeburst = ref->task->burst - 10; // set newNodeburst to 10

      if (current != root) {
        while (monitor != current) {
          if (strcmp(current->task->name, monitor->task->name) == 0) {

            break;
          }
          monitor = monitor->next;
        }
        monitor = root;
      }

      totburst = totburst + 10; // totburst is 60
      ref->task->burst = 10;
      run(ref->task, 10);
      printf("        Time is now: %d\n", totburst);
    } else if (ref->task->burst < 10) {
      newNodeburst = 0;

      totburst = totburst + ref->task->burst;

      run(ref->task, ref->task->burst);
      printf("        Time is now: %d\n", totburst);
    }

    while (1) {
      if (current->next != NULL) {
        current = current->next; // current = T4;
        if (current->next == NULL) {
          if (newNodeburst != 0) {
            struct node *newNodenode = malloc(sizeof(struct node));
            newNodenode->task = malloc(sizeof(struct task));
            newNodenode->task->name = ref->task->name;
            newNodenode->task->burst = newNodeburst;
            newNodenode->task->priority = ref->task->priority;
            ref = ref->next;             // ref points to P2
            current->next = newNodenode; // P4 points to newNode Node P1
            newNodenode->next = NULL;    // P4 points to null
            current = ref;               // current points to P2

            break;
          }

          else if (newNodeburst == 0) {
            ref = ref->next; // ref points to T3
            current = ref;   // current points to T3
            break;
          }
        }
      } else { // if this is the end element

        if (newNodeburst != 0) {
          struct node *newNode = malloc(sizeof(struct node));
          newNode->task = malloc(sizeof(struct task));
          newNode->task->name = ref->task->name;
          newNode->task->priority = ref->task->priority;
          newNode->task->burst = newNodeburst;
          ref->next = newNode;
          ref = ref->next;
          newNode->next = NULL;
          current = newNode;
        } else {
          ref = ref->next;
        }
        break;
      }
    }
  }
  WaitTime = turnaroundtime - totburst;
  printf("The average turnaround time is : %f time units \n", turnaroundtime / num);
  printf("The average response time is : %f time units \n", ResponseTime / num);
  printf("The average Wait time is : %f time units \n", WaitTime / num);
}
