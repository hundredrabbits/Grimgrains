
typedef struct {
  char *name;
  char *portions;
  char *description;
  int date;
  int time;
  int instructions_len;
  char *instructions[256];
  int servings_len;
  Serving servings[10];
} Recipe;

Recipe create_recipe(char *name, char *portions, int date, int time) {
  Recipe a;
  a.name = name;
  a.portions = portions;
  a.date = date;
  a.time = time;
  a.servings_len = 0;
  return a;
}

void add_recipe_description(Recipe *r, char *description){
  r->description = description;
}

void add_recipe_instruction(Recipe *r, char *instruction){
  r->instructions[r->instructions_len] = instruction;
  r->instructions_len++;
}

void add_recipe_serving(Recipe *r, Ingredient *i, char *quantity){
  r->servings[r->servings_len] = create_serving(i,quantity);
  r->servings_len++;
}

void add_recipe_part(Recipe *r, Part *p) {

}

void print_recipe(Recipe recipe) {
  printf("name:%s, portions:%s date:%d time:%d\n",recipe.name,recipe.portions,recipe.date,recipe.time);
  printf("===========\nInstructions:\n");
  for(int i = 0; i < recipe.instructions_len; ++i) {
    printf("%s\n", recipe.instructions[i]);
  }
  printf("===========\nIngredients:\n");
  for(int i = 0; i < recipe.servings_len; ++i) {
    printf("%s %s\n", recipe.servings[i].ingredient->name,recipe.servings[i].quantity);
  }
}
