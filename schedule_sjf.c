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

void add(char *name, int prio, int burst) {

  if (root == NULL) {
    root = malloc(sizeof(struct node));
    root->task = malloc(sizeof(struct task));

    root->task->burst = burst;
    root->task->name = name;
    root->task->priority = prio;
    root->next = NULL;

    cur = root;

  } else {
    newNode = malloc(sizeof(struct node));
    newNode->task = malloc(sizeof(struct task));

    newNode->task->burst = burst;
    newNode->task->name = name;
    newNode->task->priority = prio;

    if (!cur->next) {
      if ((cur->task->burst < newNode->task->burst) || (cur->task->burst == newNode->task->burst)) {
        cur->next = newNode;
        newNode->next = NULL;

      } else {
        newNode->next = cur;
        cur = newNode;
        root = newNode;
      }
    } else {
      while (tru) {
        if (cur->next->task->burst > newNode->task->burst) {
          if (cur->task->burst < newNode->task->burst) {

            newNode->next = cur->next;
            cur->next = newNode;
            cur = root;
            break;
          } else if (newNode->task->burst < cur->task->burst) {

            root = newNode;
            newNode->next = cur;
            cur = root;
            break;
          }

        } else if (cur->next->task->burst == newNode->task->burst) {
          cur = cur->next;

          if (cur->next == NULL) {
            newNode->next = NULL;
          } else if (cur->next->task->burst == newNode->task->burst) {
            cur = cur->next;
            newNode->next = cur->next;

          } else {
            newNode->next = cur->next;
          }

          cur->next = newNode;
          cur = root;
          break;
        } else if (cur->next->task->burst < newNode->task->burst) {

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

void schedule() {
  int runTime = 0;

  struct node *curr = root;
  while (curr != NULL) {
    run(curr->task, curr->task->burst);

    runTime = runTime + curr->task->burst;
    curr = curr->next;
    printf("        Time is now: %d\n", runTime);
  }
}