#include <stdio.h>
#include <ctype.h>

#include "helpers.c"
#include "ingredient.h"
#include "recipe.h"

#include "builder.c"

int main(void) {
  #include "ingredients.c"
  #include "recipes.c"

  int ingredients_len = sizeof ingredients / sizeof ingredients[0];
  int recipes_len = sizeof recipes / sizeof recipes[0];

  printf("Found Ingredients: %d, Recipes: %d\n",ingredients_len,recipes_len);

  for(int i = 0; i < ingredients_len; ++i) {
    build_ingredient(ingredients[i]);
  }
  printf("Built %d ingredients\n", ingredients_len);

  for(int i = 0; i < recipes_len; ++i) {
    build_recipe(recipes[i]);
  }
  printf("Built %d recipes\n", recipes_len);

  build_home(ingredients, recipes, ingredients_len, recipes_len);
  printf("Built home\n");

  build_about();
  printf("Built about\n");

  build_nutrition();
  printf("Built nutrition\n");

  build_tools();
  printf("Built tools\n");

  return (0);
}