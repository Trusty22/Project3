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
      if (((newNode->task->priority) < (cur->task->priority)) || ((newNode->task->priority) == (cur->task->priority))) {
        cur->next = newNode;
        newNode->next = NULL;

      } else {
        newNode->next = cur;
        root = newNode;
        cur = newNode;
      }
    } else {

      while (tru) {

        if ((newNode->task->priority > cur->next->task->priority)) {

          if (newNode->task->priority < cur->task->priority) {
            newNode->next = cur->next;
            cur->next = newNode;
            cur = root;
            break;
          } else if (newNode->task->priority > cur->task->priority) {

            root = newNode;
            newNode->next = cur;
            cur = root;
            break;
          } else if (newNode->task->priority == cur->task->priority) {
            newNode->next = cur->next;
            cur->next = newNode;
            break;
          }

        } else if (newNode->task->priority == cur->next->task->priority) {
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

  cur = root;
  struct node *ref = root;
  struct node *beginning = malloc(sizeof(struct node));
  int newNodeburst = 0;
  while (ref != NULL) {

    if (ref->next == NULL) {
      run(ref->task, ref->task->burst);
      break;
    } else {

      if ((ref->task->priority != ref->next->task->priority)) {

        run(ref->task, ref->task->burst);
        ref = ref->next;
      } else {

        if (ref->next != NULL) {

          if ((ref->task->priority == ref->next->task->priority)) {
            cur = ref;

            while (cur->next != NULL) {

              cur = cur->next;

              if (cur->task->priority != cur->next->task->priority) {

                beginning = cur->next;

                cur->next = NULL;
                cur = ref;
                break;
              } else {
              }
            }

            while (ref != NULL) {
              if (ref->task->burst >= 10) {
                newNodeburst = ref->task->burst - 10;
                ref->task->burst = 10;
                run(ref->task, 10);
              } else if (ref->task->burst < 10) {
                newNodeburst = 0;
                run(ref->task, ref->task->burst);
              }

              while (1) {
                if (cur->next != NULL) {
                  cur = cur->next;
                  if (cur->next == NULL) {
                    if (newNodeburst != 0) {
                      struct node *newNodenode = malloc(sizeof(struct node));
                      newNodenode->task = malloc(sizeof(struct task));
                      newNodenode->task->name = ref->task->name;
                      newNodenode->task->burst = newNodeburst;
                      newNodenode->task->priority = ref->task->priority;
                      ref = ref->next;
                      cur->next = newNodenode;
                      newNodenode->next = NULL;
                      cur = ref;

                      break;
                    }

                    else if (newNodeburst == 0) {
                      ref = ref->next;
                      cur = ref;
                      break;
                    }
                  }
                } else {

                  ref = ref->next;
                  cur = beginning;

                  break;
                }
              }
            }

            ref = beginning;
          }
        }
      }
    }
  }
}
