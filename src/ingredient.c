
typedef struct {
  int id;
  char *name;
  char *description;
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

Serving create_serving(Ingredient *ingredient, char *quantity) {
  Serving a;
  a.ingredient = ingredient;
  a.quantity = quantity;
  return a;
}

void set_parent(Ingredient *child, Ingredient *parent) {
  // printf("")
}