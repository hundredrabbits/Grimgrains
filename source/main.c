#include "stdio.h"

#include "ingredient.c"
#include "recipe.c"

int main(void) {

  #include "ingredients.c"
  #include "recipes.c"

  print_recipe(&sweet_sour_lentils);

  return (0);
}