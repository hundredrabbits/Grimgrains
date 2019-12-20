
typedef struct Ingredient {
  int id;
  char *name;
  char *description;
  struct Ingredient *parent;
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

void build_ingredient_page(Ingredient *ingredient){
  char ingredient_path[1024];
  to_lowercase(ingredient->name, ingredient_path);



  char ingredient_filepath[1024];

  sprintf(ingredient_filepath, "../site/%s.txt", ingredient_path);
  printf("%s -> %s\n", ingredient->name, ingredient_filepath);


  // FILE *myfile = fopen("../site/somefile.txt", "w");

  // fprintf(myfile, "hello!\n");

  // fclose(myfile);


}