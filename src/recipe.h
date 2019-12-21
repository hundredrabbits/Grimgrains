typedef struct {
  char *name;
  int instructions_len;
  char *instructions[16];
  int servings_len;
  Serving servings[16];
} RecipePart;

typedef struct {
  char *name;
  enum RecipeType type;
  char *portions;
  char *description;
  int date;
  int time;
  int parts_len;
  RecipePart *parts[10];
} Recipe;

int recipes_sauce_len = 0; char *recipes_sauce[16];
int recipes_dinner_len = 0; char *recipes_dinner[16];
int recipes_basic_len = 0; char *recipes_basic[16];
int recipes_sidedish_len = 0; char *recipes_sidedish[16];
int recipes_breakfast_len = 0; char *recipes_breakfast[16];
int recipes_bread_len = 0; char *recipes_bread[16];
int recipes_dessert_len = 0; char *recipes_dessert[16];
int recipes_snack_len = 0; char *recipes_snack[16];
int recipes_pasta_len = 0; char *recipes_pasta[16];
int recipes_cookies_len = 0; char *recipes_cookies[16];

void categorize_recipe(char *name, enum RecipeType type) {
  if(type == sauce){ recipes_sauce[recipes_sauce_len] = name; recipes_sauce_len++;  }
  // else if(strcmp(type,"dinner") == 0){ recipes_dinner[recipes_dinner_len] = name; recipes_dinner_len++;  }
  // else if(strcmp(type,"basic") == 0){ recipes_basic[recipes_basic_len] = name; recipes_basic_len++;  }
  // else if(strcmp(type,"sidedish") == 0){ recipes_sidedish[recipes_sidedish_len] = name; recipes_sidedish_len++;  }
  // else if(strcmp(type,"breakfast") == 0){ recipes_breakfast[recipes_breakfast_len] = name; recipes_breakfast_len++;  }
  // else if(strcmp(type,"bread") == 0){ recipes_bread[recipes_bread_len] = name; recipes_bread_len++;  }
  // else if(strcmp(type,"dessert") == 0){ recipes_dessert[recipes_dessert_len] = name; recipes_dessert_len++;  }
  // else if(strcmp(type,"pasta") == 0){ recipes_pasta[recipes_pasta_len] = name; recipes_pasta_len++;  }
  // else if(strcmp(type,"cookies") == 0){ recipes_cookies[recipes_cookies_len] = name; recipes_cookies_len++;  }
  else{ printf("Unknown type: %s -> %d\n", name, type); }
}

Recipe create_recipe(char *name, enum RecipeType type, char *portions, int date, int time) {
  Recipe a;
  a.name = name;
  a.type = type;
  a.portions = portions;
  a.date = date;
  a.time = time;
  a.parts_len = 0;
  categorize_recipe(name, type);
  return a;
}

RecipePart create_part(char *name) {
  RecipePart a;
  a.name = name;
  a.instructions_len = 0;
  a.servings_len = 0;
  return a;
}

void set_description(Recipe *r, char *description){
  r->description = description;
}

void add_instruction(RecipePart *p, char *instruction){
  p->instructions[p->instructions_len] = instruction;
  p->instructions_len++;
}

void add_serving(RecipePart *p, Ingredient *i, char *quantity){
  p->servings[p->servings_len] = create_serving(i,quantity);
  p->servings_len++;
}

void add_part(Recipe *r, RecipePart *p){
  r->parts[r->parts_len] = p;
  r->parts_len++;
}