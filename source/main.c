#include "stdio.h"

#include "ingredient.c"
#include "recipe.c"

int main(void) {

  #include "ingredients.c"
  #include "recipes.c"

  print_recipe(rice_treat);

  return (0);
}