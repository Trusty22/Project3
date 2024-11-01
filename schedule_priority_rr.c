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
struct node *cur = NULL;
int tru = 1;

void add(char *name, int prio, int burst) {
  if (root == NULL) {
    root = malloc(sizeof(struct node));
    root->task = malloc(sizeof(struct task));

    root->task->name = name;
    root->task->burst = burst;
    root->task->priority = prio;

    root->next = NULL;
    cur = root;

  } else {

    newNode = malloc(sizeof(struct node));
    newNode->task = malloc(sizeof(struct task));

    newNode->task->burst = burst;
    newNode->task->name = name;
    newNode->task->priority = prio;

    if (!(cur->next)) {
      if ((cur->task->priority > newNode->task->priority) || (cur->task->priority == newNode->task->priority)) {
        cur->next = newNode;
        newNode->next = NULL;

      } else {
        newNode->next = cur;
        root = newNode;
        cur = newNode;
      }
    } else {

      while (tru) {

        if (cur->next->task->priority < newNode->task->priority) {

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

          } else if (cur->next->task->priority == newNode->task->priority) {

            cur = cur->next;
            newNode->next = cur->next;
          } else {

            newNode->next = cur->next;
          }

          cur->next = newNode;

          cur = root;
          break;
        } else if (cur->next->task->priority > newNode->task->priority) {
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
  int nextBurst = 0;
  int runTime = 0;
  int tru = 1;
  cur = root;

  struct node *tempNode = root;
  struct node *start = malloc(sizeof(struct node));

  while (tempNode != NULL) {
    if (tempNode->next == NULL) {
      runTime = runTime + 10;

      run(tempNode->task, tempNode->task->burst);
      printf("        Time is now: %d\n", runTime);
      break;
    } else {

      if ((tempNode->task->priority != tempNode->next->task->priority)) {
        runTime = runTime + tempNode->task->burst;

        run(tempNode->task, tempNode->task->burst);
        tempNode = tempNode->next;
        printf("        Time is now: %d\n", runTime);
      } else {

        if (tempNode->next != NULL) {
          if ((tempNode->next->task->priority == tempNode->task->priority)) {
            cur = tempNode;
            while (cur->next != NULL) {
              cur = cur->next;
              if (cur->next->task->priority != cur->task->priority) {

                start = cur->next;
                cur->next = NULL;

                cur = tempNode;
                break;
              }
            }

            while (tempNode != NULL) {
              if (tempNode->task->burst >= 10) {

                nextBurst = tempNode->task->burst - 10;
                tempNode->task->burst = 10;
                runTime = runTime + 10;

                run(tempNode->task, 10);
                printf("        Time is now: %d\n", runTime);
              } else if (tempNode->task->burst < 10) {
                runTime = runTime + tempNode->task->burst;
                nextBurst = 0;

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

                  tempNode = tempNode->next;
                  cur = start;
                  break;
                }
              }
            }
            tempNode = start;
          }
        }
      }
    }
  }
}