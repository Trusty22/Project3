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

void add(char *name, int prio, int burst) {

  if (root == NULL) {
    root = malloc(sizeof(struct node));
    end = malloc(sizeof(struct node));
    root->task = malloc(sizeof(struct task));

    root->task->burst = burst;
    root->task->name = name;
    root->task->priority = prio;
    root->next = NULL;

    end = root;
  } else {

    newNode = malloc(sizeof(struct node));
    end->next = newNode;
    newNode->task = malloc(sizeof(struct task));

    newNode->task->name = name;
    newNode->task->burst = burst;
    newNode->task->priority = prio;
    newNode->next = NULL;

    end = newNode;
  }
}

// invoke the scheduler
void schedule() {

  struct node *cur = root;
  struct node *tempNode = root;
  int newNodeburst = 0;
  int runTime = 0;

  while (tempNode != NULL) {

    if (tempNode->task->burst >= 10) {

      newNodeburst = tempNode->task->burst - 10; // set newNodeburst to 10

      runTime = runTime + 10; // runTime is 60
      tempNode->task->burst = 10;
      run(tempNode->task, 10);
      printf("        Time is now: %d\n", runTime);
    } else if (tempNode->task->burst < 10) {
      newNodeburst = 0;

      runTime = runTime + tempNode->task->burst;

      run(tempNode->task, tempNode->task->burst);
      printf("        Time is now: %d\n", runTime);
    }

    while (1) {
      if (cur->next != NULL) {
        cur = cur->next; // cur = T4;
        if (cur->next == NULL) {
          if (newNodeburst != 0) {
            struct node *newNodenode = malloc(sizeof(struct node));
            newNodenode->task = malloc(sizeof(struct task));
            newNodenode->task->name = tempNode->task->name;
            newNodenode->task->burst = newNodeburst;
            newNodenode->task->priority = tempNode->task->priority;
            tempNode = tempNode->next; // tempNode points to P2
            cur->next = newNodenode;   // P4 points to newNode Node P1
            newNodenode->next = NULL;  // P4 points to null
            cur = tempNode;            // cur points to P2

            break;
          }

          else if (newNodeburst == 0) {
            tempNode = tempNode->next; // tempNode points to T3
            cur = tempNode;            // cur points to T3
            break;
          }
        }
      } else { // if this is the end element

        if (newNodeburst != 0) {
          struct node *newNode = malloc(sizeof(struct node));
          newNode->task = malloc(sizeof(struct task));
          newNode->task->name = tempNode->task->name;
          newNode->task->priority = tempNode->task->priority;
          newNode->task->burst = newNodeburst;
          tempNode->next = newNode;
          tempNode = tempNode->next;
          newNode->next = NULL;
          cur = newNode;
        } else {
          tempNode = tempNode->next;
        }
        break;
      }
    }
  }
}