#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include "task.h"

#include "list.h"

#include "schedulers.h"

#include "cpu.h"

struct node *root = NULL;
struct node *newNode = NULL;
struct node *end = NULL;

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

    newNode->task->burst = burst;
    newNode->task->name = name;
    newNode->task->priority = prio;
    newNode->next = NULL;

    end = newNode;
  }
}

void schedule() {
  int runTime = 0;

  struct node *cur = root;
  while (cur != NULL) {
    run(cur->task, cur->task->burst);

    runTime = runTime + cur->task->burst;
    cur = cur->next;
    printf("        Time is now: %d\n", runTime);
  }
}