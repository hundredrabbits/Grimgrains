#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define STR_BUF_LEN 64

enum RecipeType {
  breakfast,
  dinner,
  sidedish,
  dessert,
  toppings,
  pasta,
  bread,
  snack,
  basic,
  lifestyle
};

void to_lowercase(char *str, char *target, size_t tsize) {
  for (size_t i = 0; i < tsize; i++) {
    target[i] = str[i];
    if (target[i] == '\0') {
      break;
    }
    if (target[i] == ' ') {
      target[i] = '_';
    } else {
      target[i] = tolower(target[i]);
    }
  }
  target[tsize - 1] = '\0';
}

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
  a.parent = NULL;
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

void print_ingredient(Ingredient *ingredient) {
  printf("%s\n", ingredient->name);
}

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
    "breakfast", "dinner", "sidedish", "dessert", "toppings",
    "pasta",     "bread",  "snack",    "basic",   "lifestyle"};

int recipes_by_types_len[lifestyle + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char *recipes_by_types[lifestyle + 1][100];

void categorize_recipe(char *name, enum RecipeType type) {
  recipes_by_types[type][recipes_by_types_len[type]] = name;
  recipes_by_types_len[type]++;
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

char *html_head = "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='description' content='Grim Grains is an illustrated food blog, it features plant-based (vegan) recipes.'><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta name='twitter:card' content='summary'><meta name='twitter:site' content='@hundredrabbits'><meta name='twitter:title' content='Grimgrains'><meta name='twitter:description' content='An illustrated food blog.'><meta name='twitter:creator' content='@hundredrabbits'><meta name='twitter:image' content='https://grimgrains.com/media/services/icon.jpg'><meta property='og:title' content='Grimgrains'><meta property='og:type' content='article'><meta property='og:url' content='http://grimgrains.com/'><meta property='og:image' content='https://grimgrains.com/media/services/icon.jpg'><meta property='og:description' content='An illustrated food blog.'><meta property='og:site_name' content='Grimgrains'><link rel='icon' type='image/x-icon' href='../media/services/favicon.ico'><link rel='icon' type='image/png' href='../media/services/icon.jpg'><link rel='apple-touch-icon' href='../media/services/apple-touch-icon.png' /><title>GrimGrains — %s</title><link rel='stylesheet' type='text/css' href='../links/main.css'></head><body class='%s'>";

char *html_header = "<header><a id='logo' href='home.html'><img src='../media/interface/logo.png' alt='Grimgrains'></a></header>";

char *html_nav = "<nav><ul><li class='home'><a href='home.html'>Home</a></li><li class='recipes'><a href='home.html#recipes'>Recipes</a></li><li class='about'><a href='about.html'>About</a></li><li class='tools'><a href='tools.html'>Tools</a></li><li class='nutrition'><a href='nutrition.html'>Nutrition</a></li><li class='right'><a href='https://merveilles.town/@rek' target='_blank'>Mastodon</a></li></ul></nav>";

char *html_footer =
    "<footer><a href='about.html'>Grimgrains</a> © 2014—2020<br><a "
    "href='http://100r.co/' target='_blank'>Hundred "
    "Rabbits</a></footer></body></html>";

void build_recipe(Recipe *recipe) {
  // New strings
  char filename[STR_BUF_LEN];
  to_lowercase(recipe->name, filename, STR_BUF_LEN);
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *f = fopen(filepath, "w");

  fprintf(f, html_head, recipe->name, "recipe");
  fputs(html_header, f);
  fputs(html_nav, f);

  fputs("<main class='recipe'>", f);
  fprintf(f, "<h1>%s</h1>", recipe->name);
  fprintf(f, "<h2>%s — %d minutes</h2>", recipe->portions, recipe->time);
  fprintf(f, "<img src='../media/recipes/%s.jpg'/>", filename);
  fprintf(f, "<p class='col2'>%s</p>", recipe->description);
  for (int i = 0; i < recipe->parts_len; ++i) {
    fputs("<dl class='ingredients'>", f);
    fprintf(f, "<h3>%s</h3>", recipe->parts[i]->name);
    for (int i2 = 0; i2 < recipe->parts[i]->servings_len; ++i2) {
      char ingr_path[STR_BUF_LEN];
      to_lowercase(recipe->parts[i]->servings[i2].ingredient->name, ingr_path,
                   STR_BUF_LEN);
      fprintf(f,
              "<dt><a href='%s.html'><img "
              "src='../media/ingredients/%s.png'/><b>%s</b> <u>%s</u></a></dt>",
              ingr_path, ingr_path,
              recipe->parts[i]->servings[i2].ingredient->name,
              recipe->parts[i]->servings[i2].quantity);
    }
    fputs("</dl>", f);
    fputs("<ul class='instructions'>", f);
    for (int i2 = 0; i2 < recipe->parts[i]->instructions_len; ++i2) {
      fprintf(f, "<li>%s</li>", recipe->parts[i]->instructions[i2]);
    }
    fputs("</ul>", f);
  }
  fputs("</main>", f);

  fputs(html_footer, f);

  fclose(f);
}

void build_ingredient(Ingredient *ingredient) {
  // New strings
  char filename[STR_BUF_LEN];
  to_lowercase(ingredient->name, filename, STR_BUF_LEN);
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *f = fopen(filepath, "w");

  fprintf(f, html_head, ingredient->name, "ingredient");
  fputs(html_header, f);
  fputs(html_nav, f);

  fputs("<main class='ingredient'>", f);
  fprintf(f, "<h1>%s</h1>", ingredient->name);
  fprintf(f, "<img class='right' src='../media/ingredients/%s.png'/>",
          filename);
  fprintf(f, "<p>%s</p>", ingredient->description);
  if (ingredient->parent) {
    fprintf(f, "<h2>%s</h2>", ingredient->parent->name);
    fprintf(f, "<p class='small'>%s</p>", ingredient->parent->description);
  }
  fputs("<hr/>", f);
  fputs("</main>", f);

  fputs(html_footer, f);

  fclose(f);
}

void build_home(Ingredient *ingredients[], int ingredients_len,
                int recipes_len) {
  // New strings
  char *filename = "home";
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *f = fopen(filepath, "w");

  fprintf(f, html_head, "Home", "home");
  fputs(html_header, f);
  fputs(html_nav, f);

  fputs("<main class='home'>", f);
  fprintf(f, "<h1>%d Ingredients</h1>", ingredients_len);

  fputs("<dl class='ingredients'>", f);
  for (int i = 0; i < ingredients_len; ++i) {
    char ingr_path[STR_BUF_LEN];
    to_lowercase(ingredients[i]->name, ingr_path, STR_BUF_LEN);
    fprintf(f,
            "<dt><a href='%s.html'><img "
            "src='../media/ingredients/%s.png'/><b>%s</b></a></dt>",
            ingr_path, ingr_path, ingredients[i]->name);
  }
  fputs("</dl>", f);

  fprintf(f, "<h1 id='recipes'>%d Recipes</h1>", recipes_len);

  fputs("<ul class='recipes col3'>", f);

  for (int i = 0; i < lifestyle + 1; ++i) {
    fprintf(f, "<h3>%s</h3>", recipe_type_names[i]);
    for (int j = 0; j < recipes_by_types_len[i]; ++j) {
      char recipe_path[STR_BUF_LEN];
      to_lowercase(recipes_by_types[i][j], recipe_path, STR_BUF_LEN);
      fprintf(f, "<li><a href='%s.html'>%s</a></li>", recipe_path,
              recipes_by_types[i][j]);
    }
  }

  fputs("</main>", f);

  fputs(html_footer, f);

  fclose(f);
}

void build_inc(char *name) {
  char *filename = name;
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *f = fopen(filepath, "w");

  char incpath[STR_BUF_LEN];
  snprintf(incpath, STR_BUF_LEN, "inc/%s.htm", filename);

  fprintf(f, html_head, name, name);
  fputs(html_header, f);
  fputs(html_nav, f);

  fprintf(f, "<main class='%s'>", name);
  char buffer[4096];
  FILE *fp = fopen(incpath, "r");
  if(fp == NULL){ return; }

  for (;;) {
    size_t sz = fread(buffer, 1, sizeof(buffer), fp);
    if (sz) {
      fwrite(buffer, 1, sz, f);
    } else if (feof(fp) || ferror(fp)) {
      break;
    }
  }   
  fclose(fp);
  fputs("</main>", f);

  fputs(html_footer, f);

  fclose(f);
}

int main(void) {
  #include "ingredients.c"
  #include "recipes.c"

  int ingredients_len = sizeof ingredients / sizeof ingredients[0];
  int recipes_len = sizeof recipes / sizeof recipes[0];

  printf("Found Ingredients: %d, Recipes: %d\n", ingredients_len, recipes_len);

  for (int i = 0; i < ingredients_len; ++i) {
    build_ingredient(ingredients[i]);
  }
  printf("Built %d ingredients\n", ingredients_len);

  for (int i = 0; i < recipes_len; ++i) {
    build_recipe(recipes[i]);
  }
  printf("Built %d recipes\n", recipes_len);

  build_home(ingredients, ingredients_len, recipes_len);
  printf("Built home\n");

  build_inc("about");
  build_inc("nutrition");
  build_inc("tools");
  build_inc("meals");

  return (0);
}
