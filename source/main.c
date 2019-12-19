#include "stdio.h"

// cc -std=c99 -Wall main.c -o main -lm; and ./main

typedef struct {
  int id;
  char *name;
  char *description;
} Ingredient;

typedef struct {
  Ingredient *ingredient;
  char *quantity;
} Serving;

typedef struct {
  int id;
  char *name;
  char *description;
  int servings_len;
  Serving servings[10];
} Recipe;

Recipe create_recipe(int id, char *name, char *description) {
  Recipe a;
  a.id = id;
  a.name = name;
  a.description = description;
  a.servings_len = 0;
  return a;
}

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

void print_recipe(Recipe recipe) {
  printf("recipe #%d: %s - %s\n",recipe.id,recipe.name,recipe.description);
  // int len = sizeof(recipe.ingredients)/sizeof(recipe.ingredients[0]);
  // printf("%d\n", len);
  // for(int i = 0; i < len; ++i) {
  //   printf("ingredient: %s\n", recipe.ingredients[i]);
  // }
}

void add_serving(Recipe *r, Ingredient *i, char *quantity){
  r->servings[r->servings_len] = create_serving(i,quantity);
  r->servings_len++;
}

int main(void) {
  Ingredient pepper = create_ingredient("pepper","pepper is good");
  Recipe cake = create_recipe(16,"cake","Some cake description. トチシマクチトシ");

  add_serving(&cake,&pepper,"1tbs");

  print_recipe(cake);

  return (0);
}