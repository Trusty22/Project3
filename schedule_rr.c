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

void schedule() {

  struct node *cur = root;
  struct node *tempNode = root;

  int nextBurst = 0;
  int runTime = 0;
  int tru = 1;

  while (tempNode != NULL) {

    if (tempNode->task->burst >= 10) {

      nextBurst = tempNode->task->burst - 10;
      runTime = runTime + 10;
      tempNode->task->burst = 10;

      run(tempNode->task, 10);
      printf("        Time is now: %d\n", runTime);
    } else if (tempNode->task->burst < 10) {

      nextBurst = 0;
      runTime = runTime + tempNode->task->burst;

      run(tempNode->task, tempNode->task->burst);
      printf("        Time is now: %d\n", runTime);
    }

    while (tru) {
      if (cur->next != NULL) {
        cur = cur->next;
        if (cur->next == NULL) {
          if (nextBurst != 0) {
            struct node *newNodenode = malloc(sizeof(struct node));
            newNodenode->task = malloc(sizeof(struct task));

            newNodenode->task->name = tempNode->task->name;
            newNodenode->task->burst = nextBurst;
            newNodenode->task->priority = tempNode->task->priority;

            tempNode = tempNode->next;
            cur->next = newNodenode;
            newNodenode->next = NULL;

            cur = tempNode;
            break;
          }

          else if (nextBurst == 0) {

            tempNode = tempNode->next;
            cur = tempNode;
            break;
          }
        }
      } else {

        if (nextBurst != 0) {
          struct node *newNode = malloc(sizeof(struct node));
          newNode->task = malloc(sizeof(struct task));

          newNode->task->name = tempNode->task->name;
          newNode->task->priority = tempNode->task->priority;
          newNode->task->burst = nextBurst;

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