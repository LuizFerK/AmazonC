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

// Build a product struct
struct Product build_product(
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

// Find a product by the given ID
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

// Find a cart node by the given ID
struct CNode* search_cart(struct CNode** cart_head_ref, int code) {
  struct CNode *cart_head = *cart_head_ref;
  struct CNode* tmp = NULL;

  while (cart_head != NULL) {
    if (cart_head->cart.codeprod == code) {
      tmp = cart_head;
    }

    cart_head = cart_head->next;
  }

  return tmp;
}

// Add a product to the final of the product list
void append(struct Node** head_ref) {
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
  struct Node* last = *head_ref;
  int code, qt_stock;
  char name[30];
  float price;

  printf("\nCreate product\n");
  printf("Code: ");
  scanf("%d", &code);
  printf("Name: ");
  scanf("%s", name);
  printf("Price: $");
  scanf("%f", &price);
  printf("Quantity in stock: ");
  scanf("%d", &qt_stock);

  if (code == 0) {
    printf("\n0 is not a valid product code! Try again.\n");
    return;
  }

  struct Product product = build_product(code, name, price, qt_stock);

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

// Delete a product from the product list
void delete(struct Node** head_ref) {
  struct Node *tmp = *head_ref, *prev;
  int code;

  if (tmp == NULL) {
    printf("\nEmpty list!\n");
    return;
  }

  printf("\nDelete product\n");
  printf("Code: ");
  scanf("%d", &code);

  // If the product is in the first position on the list
  if (tmp != NULL && tmp->product.code == code) {
    *head_ref = tmp->next;
    free(tmp);
    return;
  }

  while (tmp != NULL && tmp->product.code != code) {
    prev = tmp;
    tmp = tmp->next;
  }

  if (tmp == NULL) {
    printf("\nProduct not registered.\n");
    return;
  }

  printf("\n%s deleted from the product list.\n", tmp->product.name);
  prev->next = tmp->next;

  free(tmp);
}

// Find a product
void find(struct Node** head_ref) {
  int code;

  if (*head_ref == NULL) {
    printf("\nEmpty list!\n");
    return;
  }

  printf("\nFind product\n");
  printf("Code: ");
  scanf("%d", &code);

  struct Node* found_node = search(head_ref, code);

  if (found_node == NULL) {
    printf("\nProduct not registered.\n");
    return;
  }

  printf("\nProduct found\n");
  printf("%d - ", found_node->product.code);
  printf("%s, ", found_node->product.name);
  printf("$%.2f ", found_node->product.price);
  printf("(%d units in stock)\n", found_node->product.qt_stock);
}

// List products
void list(struct Node* head) {
  if (head == NULL) {
    printf("\nEmpty list!\n");
    return;
  }

  printf("\nProduct list\n");
  while (head != NULL) {
    printf("%d - ", head->product.code);
    printf("%s, ", head->product.name);
    printf("$%.2f ", head->product.price);
    printf("(%d units in stock)\n", head->product.qt_stock);
    head = head->next;
  }
}

// Calculate the total cart price
void total_price(struct Node* head, struct CNode* cart_head) {
  float total = 0;

  // If there is no product on the cart
  if (cart_head == NULL) {
    printf("Your cart price: $0.00\n");
    return;
  }

  while (cart_head != NULL) {
    struct Node* found_node = search(&head, cart_head->cart.codeprod);
    total += found_node->product.price * cart_head->cart.qt_buy;
    cart_head = cart_head->next;
  }

  printf("Your cart price: $%.2f\n", total);
}

// Validate user purchase quantity
int get_qt_buy(struct Product product) {
  int qt_buy;

  if (product.qt_stock == 0) {
    printf("\nSorry, this product run out of stock.\n");
    return 0;
  }

  printf("\nHow many units of %s do you want?\n", product.name);
  printf("Quantity: ");
  scanf("%d", &qt_buy);

  if (qt_buy > product.qt_stock) {
    printf("\nSorry, this product has only %d units left in stock. Try again.\n", product.qt_stock);
    return 0;
  }

  if (qt_buy <= 0) {
    printf("\nPlease, select a valid number of units.\n");
    return 0;
  }

  return qt_buy;
}

// After add to cart, decrease product stock
void update_product_stock(struct Node** head_ref, int code, int qt_buy) {
  struct Node *head = *head_ref;

  while (head != NULL) {
    if (head->product.code == code) {
      head->product.qt_stock -= qt_buy;
    }

    head = head->next;
  }
}

// After remove from cart, reset product stock
void reset_product_stock(struct Node** head_ref, int code, int qt_buy) {
  struct Node *head = *head_ref;

  while (head != NULL) {
    if (head->product.code == code) {
      head->product.qt_stock += qt_buy;
    }

    head = head->next;
  }
}

// Add a product to the cart
void append_to_cart(
  struct Node** head_ref,
  struct CNode** cart_head,
  struct Product product
) {
  // Validate the user purchase quantity
  int qt_buy = get_qt_buy(product);

  if (qt_buy == 0) return;

  struct CNode* cart_node = search_cart(cart_head, product.code);

  // If the product is already on the cart, sum the quantity
  if (cart_node != NULL) {
    cart_node->cart.qt_buy += qt_buy;
    return;
  }

  struct CNode* new_cart_node = (struct CNode*)malloc(sizeof(struct CNode));
  struct CNode* last = *cart_head;
  struct Cart cart;

  cart.codeprod = product.code;
  cart.qt_buy = qt_buy;

  new_cart_node->cart = cart;
  new_cart_node->next = NULL;

  if (*cart_head == NULL) {
    new_cart_node->prev = NULL;
    *cart_head = new_cart_node;
  } else {
    while (last->next != NULL) {
      last = last->next;
    }
    
    last->next = new_cart_node;
    new_cart_node->prev = last;
  }

  // Decrease product stock
  update_product_stock(head_ref, product.code, qt_buy);

  printf("\n%d units of %s has been added to your cart!\n", qt_buy, product.name);
}

//
void remove_cart_product(
  struct Node** head_ref,
  struct CNode** cart_head_ref,
  int code,
  int qt_buy
) {
  struct CNode *tmp = *cart_head_ref, *prev;

  if (tmp != NULL && tmp->cart.codeprod == code) {
    *cart_head_ref = tmp->next;
    free(tmp);
    reset_product_stock(head_ref, code, qt_buy);
    return;
  }

  while (tmp != NULL && tmp->cart.codeprod != code) {
    prev = tmp;
    tmp = tmp->next;
  }

  printf("\nProduct with code %d deleted from the cart list.\n", tmp->cart.codeprod);
  prev->next = tmp->next;

  free(tmp);

  // Reset product stock
  reset_product_stock(head_ref, code, qt_buy);
}

// List the cart and let the user delete products from it
void checkout(struct Node** head_ref, struct CNode** cart_head_ref) {
  int opt;
  int qt_buy = 0;
  struct CNode* tmp = *cart_head_ref;
  
  if (tmp == NULL) {
    printf("\nYour cart is empty!\n");
    return;
  }

  // Print cart
  printf("\nYour cart\n");
  while (tmp != NULL) {
    struct Node* found_node = search(head_ref, tmp->cart.codeprod);
    struct Product product = found_node->product;

    printf("%d - ", product.code);
    printf("%s, ", product.name);
    printf("$%.2f ", product.price * tmp->cart.qt_buy);
    printf("(%d units)\n", tmp->cart.qt_buy);
    tmp = tmp->next;
  }

  // Reset tmp variable to reuse it
  tmp = *cart_head_ref;

  // Let the user delete products from the cart
  printf("\nType the product code bellow to remove from cart (0 to return to main menu)\n");
  printf("Option: ");
  scanf("%d", &opt);

  // Returns to main menu
  if (opt == 0) return;

  while (tmp != NULL) {
    if (tmp->cart.codeprod == opt) {
      qt_buy = tmp->cart.qt_buy;
    }
    tmp = tmp->next;
  }

  // Found no product, means the code is invalid
  if (qt_buy == 0) {
    printf("\nPlease, select a valid option.\n");
    checkout(head_ref, cart_head_ref);
    return;
  }

  // Remove product from cart and call the function again
  remove_cart_product(head_ref, cart_head_ref, opt, qt_buy);
  checkout(head_ref, cart_head_ref);
}

// Add products to cart by the given code and quantity
void buy(struct Node** head_ref, struct CNode** cart_head_ref) {
  char code[2];

  if (*head_ref == NULL) {
    printf("\nEmpty list!\n");
    return;
  }

  printf("\nSelect the product you want to add to your cart\n");
  printf("\"M\" - return to main menu\n");
  printf("\"C\" - see cart and/or checkout\n\n");

  total_price(*head_ref, *cart_head_ref);
  list(*head_ref);

  printf("\nOption: ");
  scanf("%s", code);

  // Return to main menu
  if (strcmp(code, "M") == 0) return;

  // Print cart and let the user delete products from it
  if (strcmp(code, "C") == 0) {
    checkout(head_ref, cart_head_ref);
    return;
  }
  
  struct Node* found_node = search(head_ref, atoi(code));

  if (found_node == NULL) {
    printf("\nPlease, select a valid option.\n");
    buy(head_ref, cart_head_ref);
    return;
  }

  // Add product to cart and call the function again
  append_to_cart(head_ref, cart_head_ref, found_node->product);
  buy(head_ref, cart_head_ref);
}

// Free memory from product and cart list
void free_memory(struct Node* head, struct CNode* cart_head) {
  struct Node *tmp;
  struct CNode *c_tmp;
  int count = 0;

  printf("\nThanks for the preference. Bye!\n");

  while (head != NULL) {
    tmp = head;
    head = head->next;
    free(tmp);
    count++;
  }

  if (count > 1) printf("%d product nodes has been freed from memory.\n", count);
  else if (count > 0) printf("%d product node has been freed from memory.\n", count);

  count = 0;

  while (cart_head != NULL) {
    c_tmp = cart_head;
    cart_head = cart_head->next;
    free(c_tmp);
    count++;
  }

  if (count > 1) printf("%d cart nodes has been freed from memory.\n", count);
  else if (count > 0) printf("%d cart node has been freed from memory.\n", count);
}

// Main menu
int main() {
  struct Node* head = NULL;
  struct CNode* cart_head = NULL;
  int opt;

  printf("Welcome to AmazonC!\n");

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
    else if (opt == 3) find(&head);
    else if (opt == 4) delete(&head);
    else if (opt == 5) buy(&head, &cart_head);
    else if (opt == 0) free_memory(head, cart_head);
    else printf("\nPlease, select a valid option!\n\n");
  }

  return 0;
}
