
typedef struct Ingredient {
  int id;
  char *name;
  char *description;
  struct Ingredient *parent;
  // int children_len;
  // struct Ingredient *children[16];
} Ingredient;

typedef struct {
  Ingredient *ingredient;
  char *quantity;
} Serving;

Ingredient create_ingredient(char *name, char *description) {
  Ingredient a;
  a.name = name;
  a.description = description;
  return a;
}

Ingredient create_child_ingredient(Ingredient *parent, char *name, char *description) {
  Ingredient a;
  a.name = name;
  a.description = description;
  a.parent = parent;
  // parent->children[parent->children_len] = a;
  // parent->children_len++;
  return a;
}

Serving create_serving(Ingredient *ingredient, char *quantity) {
  Serving a;
  a.ingredient = ingredient;
  a.quantity = quantity;
  return a;
}

void print_ingredient(Ingredient *ingredient){
  printf("%s\n", ingredient->name);
}