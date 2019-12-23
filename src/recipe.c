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

char recipe_type_names[20][32] = {
    "breakfast", "dinner", "sidedish", "dessert", "sauce",
    "pasta",     "bread",  "snack",    "basic",   "lifestyle"};

int recipes_by_types_len[lifestyle + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char *recipes_by_types[lifestyle + 1][100];

void categorize_recipe(char *name, enum RecipeType type) {
  recipes_by_types[type][recipes_by_types_len[type]] = name;
  recipes_by_types_len[type]++;
}

Recipe create_recipe(char *name, enum RecipeType type, char *portions, int date,
                     int time) {
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

void set_description(Recipe *r, char *description) {
  r->description = description;
}

void add_instruction(RecipePart *p, char *instruction) {
  p->instructions[p->instructions_len] = instruction;
  p->instructions_len++;
}

void add_serving(RecipePart *p, Ingredient *i, char *quantity) {
  p->servings[p->servings_len] = create_serving(i, quantity);
  p->servings_len++;
}

void add_part(Recipe *r, RecipePart *p) {
  r->parts[r->parts_len] = p;
  r->parts_len++;
}
