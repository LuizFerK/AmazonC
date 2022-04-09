#include <stdio.h>
#include <stdlib.h>

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

int main() {
  struct Node* head = NULL;
  struct CNode* cart_head = NULL;
  int opt;

  printf("Welcome to AmazonCC!\n\n");

  while (opt != 0) {
    printf("Menu:\n");
    printf("1 - Create product\n");
    printf("2 - List all products\n");
    printf("3 - Find a product\n");
    printf("4 - Delete a product\n");
    printf("5 - Buy\n");
    printf("0 - Exit\n\n");
    printf("Option: ");

    scanf("%d", &opt);

    if (opt == 1) {
      printf("\nCreate\n\n");
    } else if (opt == 2) {
      printf("\nList\n\n");
    } else if (opt == 3) {
      printf("\nFind\n\n");
    } else if (opt == 4) {
      printf("\nDelete\n\n");
    } else if (opt == 5) {
      printf("\nCart\n\n");
    } else if (opt == 0) {
      printf("\nThanks for the preference. Bye!\n\n");
    } else {
      printf("\nPlease, select a valid option!\n\n");
    }
  }

  return 0;
}
