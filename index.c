#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
  int code;
  char name[30];
  float price;
  int qt_stock;
};
 
struct Node {
  struct Product product;
  struct Node* next;
  struct Node* prev;
};

struct Cart {
  int codeprod;
  int qt_buy;
};

struct CNode {
  struct Cart cart;
  struct CNode* next;
  struct CNode* prev;
};

struct Product buildProduct(
  int code,
  char *name,
  float price,
  int qt_stock
) {
  struct Product product;

  product.code = code;
  strcpy(product.name, name);
  product.price = price;
  product.qt_stock = qt_stock;
  
  return product;
}

struct Node* search(struct Node** head_ref, int code) {
  struct Node *head = *head_ref;
  struct Node* tmp = NULL;

  while (head != NULL) {
    if (head->product.code == code) {
      tmp = head;
    }

    head = head->next;
  }

  return tmp;
}

void append(struct Node** head_ref) {
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
  struct Node* last = *head_ref;
  int code, qt_stock;
  char name[30];
  float price;

  printf("\nProduct\n");
  printf("Code: ");
  scanf("%d", &code);
  printf("Name: ");
  scanf("%s", name);
  printf("Price: $");
  scanf("%f", &price);
  printf("Quantity in stock: ");
  scanf("%d", &qt_stock);

  struct Product product = buildProduct(code, name, price, qt_stock);

  struct Node* search_result = search(head_ref, code);

  if (search_result != NULL) {
    printf("\nA product with this code already exists! Try again.\n");
    return;
  }

  new_node->product = product;
  new_node->next = NULL;

  if (*head_ref == NULL) {
    new_node->prev = NULL;
    *head_ref = new_node;
    return;
  }

  while (last->next != NULL) last = last->next;
  
  last->next = new_node;
  new_node->prev = last;
}

void list(struct Node* head) {
  if (head == NULL) {
    printf("\nEmpty list!\n");
    return;
  }

  while (head != NULL) {
    printf("%d - ", head->product.code);
    printf("%s, ", head->product.name);
    printf("$%.2f\n", head->product.price);
    head = head->next;
  }
}

int main() {
  struct Node* head = NULL;
  struct CNode* cart_head = NULL;
  int opt;

  printf("Welcome to AmazonCC!\n");

  while (opt != 0) {
    printf("\nMenu:\n");
    printf("1 - Create product\n");
    printf("2 - List all products\n");
    printf("3 - Find a product\n");
    printf("4 - Delete a product\n");
    printf("5 - Buy\n");
    printf("0 - Exit\n\n");
    printf("Option: ");

    scanf("%d", &opt);

    if (opt == 1) append(&head);
    else if (opt == 2) list(head);
    else if (opt == 3) printf("\nFind\n\n");
    else if (opt == 4) printf("\nDelete\n\n");
    else if (opt == 5) printf("\nCart\n\n");
    else if (opt == 0) printf("\nThanks for the preference. Bye!\n\n");
    else printf("\nPlease, select a valid option!\n\n");
  }

  return 0;
}
