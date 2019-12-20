
int ingredients_len = sizeof ingredients / sizeof ingredients[0];
int recipes_len = sizeof recipes / sizeof recipes[0];

printf("Found Ingredients: %d, Recipes: %d\n",ingredients_len,recipes_len);



for(int i = 0; i < ingredients_len; ++i) {
  build_ingredient_page(ingredients[i]);
}

for(int i = 0; i < recipes_len; ++i) {
  build_recipe_page(recipes[i]);
}
