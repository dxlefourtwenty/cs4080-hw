// Node definition
typedef struct Node {
  int data;
  struct Node* next;
  struct Node* prev;
} Node;

// create a new node with given value as data
Node* createNode(int data);

// get start of list from any node
Node* getHeadNode(Node* node);

// insert node at the beginning
void insertAtBeginning(Node** node, int data);

// insert node at the end of the list
void insertAtEnd(Node** node, int data);

// insert node at a given position
void insertAtPosition(Node** node, int data, int position);

// delete node at the beginning of the list
void deleteAtBeginning(Node** node);

// delete node at the end of the list
void deleteAtEnd(Node** node);

// delete node at a given position
void deleteAtPosition(Node** node, int position);

// print list from start -> end
void printList(Node** node);
