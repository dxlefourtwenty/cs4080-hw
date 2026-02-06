#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

int main(int argc, const char* argv[]) {
  printf("Hello, World!\n");

  Node* node1 = createNode(4);
  printList(&node1);

  insertAtBeginning(&node1, 10);
  printList(&node1);

  insertAtEnd(&node1, 25);
  printList(&node1);

  insertAtPosition(&node1, 225, 1);
  printList(&node1);

  insertAtPosition(&node1, 47, 3);
  printList(&node1);

  deleteAtBeginning(&node1);
  printList(&node1);

  deleteAtEnd(&node1);
  printList(&node1);

  deleteAtPosition(&node1, 1);
  printList(&node1);

  return 0;
}

