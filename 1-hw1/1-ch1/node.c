#include "node.h"
#include <stdio.h>
#include <stdlib.h>

// create a new node with given value as data
Node* createNode(int data) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  newNode->prev = NULL;

  return newNode;
}

// helper to get list length
int getLength(Node** node) {
  if (*node == NULL) {
    return 0;
  }

  int len = 1;
  Node* temp = getHeadNode(*node);
  while (temp->next != NULL) {
    temp = temp->next;
    len++;
  }

  return len;
}

// get start of list from any node
Node* getHeadNode(Node* node) {
  if (node == NULL) {
        return NULL;
  }

  while (node->prev != NULL) {
      node = node->prev;
  }

  return node;
}

// insert node at the beginning
void insertAtBeginning(Node** node, int data) {
  Node* newNode = createNode(data);

  // if list is empty, set newNode to be head of list
  if (*node == NULL) {
    *node = newNode;
    return;
  }

  Node* head = getHeadNode(*node);

  // dereference head and get it's previous then set to newNode
  head->prev = newNode;

  // set newNode's next to be old head node
  newNode->next = head;

  // newNode will be new head
  head = newNode; 
}

// insert node at the end of the list
void insertAtEnd(Node** node, int data) {
  Node* newNode = createNode(data);

  if (*node == NULL) {
    *node = newNode;
    return;
  }

  // create temp node to traverse list, start at node
  Node* temp = *node;
  while (temp->next != NULL) {
    temp = temp->next;
  }

  // set last node's next to be newNode
  temp->next = newNode;

  // set newNode's previous to be 2nd to last node
  newNode->prev = temp;
}

// insert node at a given position
void insertAtPosition(Node** node, int data, int position) {
  Node* newNode = createNode(data);
  int len = getLength(node);

  // out of bounds check
  if (position < 0 || position > len) return;

  if (position == 0) {
    insertAtBeginning(node, data);
    return;
  }

  if (position == len) {
    insertAtEnd(node, data);
    return;
  }

  // traverse the list until finding node
  // before desired position
  int i = 0;
  Node* temp = *node;
  while (i < position - 1) {
    temp = temp->next;
    i++;
  }

  // point newNode to temp's next
  newNode->next = temp->next;

  // insert newNode in between
  temp->next = newNode;
  newNode->prev = temp;
}

// delete node at the beginning of the list
void deleteAtBeginning(Node** node) {
  if (*node == NULL) return;

  if ((*node)->next == NULL) {
    *node = NULL;
    free(*node);
    return;
  }

  // point the node to the head node
  *node = getHeadNode(*node);

  (*node) = (*node)->next;
  (*node)->prev = NULL;

  // free the memory of the old head node
  free((*node)->prev);
}

// delete node at the end of the list
void deleteAtEnd(Node** node) {
  if (*node == NULL) return;

  Node* temp = *node;
  if (temp->next == NULL) {
    *node = NULL;
    free(temp);
    return;
  }

  while((*node)->next != NULL) {
    *node = (*node)->next;
  }

  (*node)->prev->next = NULL;

  free((*node)->prev->next);
}

// delete node at a given position
void deleteAtPosition(Node** node, int position) {
    if (*node == NULL) return;

    Node* head = getHeadNode(*node);
    int len = getLength(&head);

    if (position < 0 || position >= len) return;

    // delete first node
    if (position == 0) {
        Node* toDelete = head;
        head = head->next;
        if (head != NULL) {
            head->prev = NULL;
        }
        *node = head;
        free(toDelete);
        return;
    }

    // traverse
    Node* temp = head;
    for (int i = 0; i < position; i++) {
        temp = temp->next;
    }

    // reconnect neighbors
    temp->prev->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }

    free(temp);
}

// print list from start -> end
void printList(Node** node) {
  if (*node == NULL) {
    printf("Error: list is empty.");
    return;
  }

  Node* temp = getHeadNode(*node);
  printf("%d", temp->data);
  while (temp->next != NULL) {
    temp = temp->next;
    printf(" <-> %d", temp->data);
  }

  printf("\n");
}

