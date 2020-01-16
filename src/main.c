#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define STR_BUF_LEN 64

enum RecipeType {
  breakfast,
  dinner,
  sidedish,
  dessert,
  sauce,
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
    "breakfast", "dinner", "sidedish", "dessert", "sauce",
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

char *html_head = "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='description' content='Grim Grains is an illustrated food blog, it features plant-based (vegan) recipes.'><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta name='twitter:card' content='summary'><meta name='twitter:site' content='@RekkaBell'><meta name='twitter:title' content='Grimgrains'><meta name='twitter:description' content='An illustrated food blog.'><meta name='twitter:creator' content='@RekkaBell'><meta name='twitter:image' content='https://grimgrains.com/media/services/icon.jpg'><meta property='og:title' content='Grimgrains'><meta property='og:type' content='article'><meta property='og:url' content='http://grimgrains.com/'><meta property='og:image' content='https://grimgrains.com/media/services/icon.jpg'><meta property='og:description' content='An illustrated food blog.'><meta property='og:site_name' content='Grimgrains'><title>GrimGrains — %s</title><link rel='stylesheet' type='text/css' href='../links/main.css'></head><body class='%s'>";

char *html_header = "<header><a id='logo' href='home.html'><img src='../media/interface/logo.png' alt='Grimgrains'></a></header>";

char *html_nav = "<nav><ul><li class='home'><a href='home.html'>Home</a></li><li class='recipes'><a href='home.html#recipes'>Recipes</a></li><li class='about'><a href='about.html'>About</a></li><li class='tools'><a href='tools.html'>Tools</a></li><li class='nutrition'><a href='nutrition.html'>Nutrition</a></li><li class='right'><a href='http://twitter.com/grimgrains' target='_blank'>Twitter</a></li></ul></nav>";

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
  FILE *myfile = fopen(filepath, "w");

  fprintf(myfile, html_head, recipe->name, "recipe");
  fputs(html_header, myfile);
  fputs(html_nav, myfile);

  fputs("<main class='recipe'>", myfile);
  fprintf(myfile, "<h1>%s</h1>", recipe->name);
  fprintf(myfile, "<h2>%s — %d minutes</h2>", recipe->portions, recipe->time);
  fprintf(myfile, "<img src='../media/recipes/%s.jpg'/>", filename);
  fprintf(myfile, "<p class='col2'>%s</p>", recipe->description);
  for (int i = 0; i < recipe->parts_len; ++i) {
    fputs("<dl class='ingredients'>", myfile);
    fprintf(myfile, "<h3>%s</h3>", recipe->parts[i]->name);
    for (int i2 = 0; i2 < recipe->parts[i]->servings_len; ++i2) {
      char ingr_path[STR_BUF_LEN];
      to_lowercase(recipe->parts[i]->servings[i2].ingredient->name, ingr_path,
                   STR_BUF_LEN);
      fprintf(myfile,
              "<dt><a href='%s.html'><img "
              "src='../media/ingredients/%s.png'/><b>%s</b> <u>%s</u></a></dt>",
              ingr_path, ingr_path,
              recipe->parts[i]->servings[i2].ingredient->name,
              recipe->parts[i]->servings[i2].quantity);
    }
    fputs("</dl>", myfile);
    fputs("<ul class='instructions'>", myfile);
    for (int i2 = 0; i2 < recipe->parts[i]->instructions_len; ++i2) {
      fprintf(myfile, "<li>%s</li>", recipe->parts[i]->instructions[i2]);
    }
    fputs("</ul>", myfile);
  }
  fputs("</main>", myfile);

  fputs(html_footer, myfile);

  fclose(myfile);
}

void build_ingredient(Ingredient *ingredient) {
  // New strings
  char filename[STR_BUF_LEN];
  to_lowercase(ingredient->name, filename, STR_BUF_LEN);
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *myfile = fopen(filepath, "w");

  fprintf(myfile, html_head, ingredient->name, "ingredient");
  fputs(html_header, myfile);
  fputs(html_nav, myfile);

  fputs("<main class='ingredient'>", myfile);
  fprintf(myfile, "<h1>%s</h1>", ingredient->name);
  fprintf(myfile, "<img class='right' src='../media/ingredients/%s.png'/>",
          filename);
  fprintf(myfile, "<p>%s</p>", ingredient->description);
  if (ingredient->parent) {
    fprintf(myfile, "<h2>%s</h2>", ingredient->parent->name);
    fprintf(myfile, "<p class='small'>%s</p>", ingredient->parent->description);
  }
  fputs("<hr/>", myfile);
  fputs("</main>", myfile);

  fputs(html_footer, myfile);

  fclose(myfile);
}

void build_home(Ingredient *ingredients[], int ingredients_len,
                int recipes_len) {
  // New strings
  char *filename = "home";
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *myfile = fopen(filepath, "w");

  fprintf(myfile, html_head, "Home", "home");
  fputs(html_header, myfile);
  fputs(html_nav, myfile);

  fputs("<main class='home'>", myfile);
  fprintf(myfile, "<h1>%d Ingredients</h1>", ingredients_len);

  fputs("<dl class='ingredients'>", myfile);
  for (int i = 0; i < ingredients_len; ++i) {
    char ingr_path[STR_BUF_LEN];
    to_lowercase(ingredients[i]->name, ingr_path, STR_BUF_LEN);
    fprintf(myfile,
            "<dt><a href='%s.html'><img "
            "src='../media/ingredients/%s.png'/><b>%s</b></a></dt>",
            ingr_path, ingr_path, ingredients[i]->name);
  }
  fputs("</dl>", myfile);

  fprintf(myfile, "<h1 id='recipes'>%d Recipes</h1>", recipes_len);

  fputs("<ul class='recipes col3'>", myfile);

  for (int i = 0; i < lifestyle + 1; ++i) {
    fprintf(myfile, "<h3>%s</h3>", recipe_type_names[i]);
    for (int j = 0; j < recipes_by_types_len[i]; ++j) {
      char recipe_path[STR_BUF_LEN];
      to_lowercase(recipes_by_types[i][j], recipe_path, STR_BUF_LEN);
      fprintf(myfile, "<li><a href='%s.html'>%s</a></li>", recipe_path,
              recipes_by_types[i][j]);
    }
  }

  fputs("</main>", myfile);

  fputs(html_footer, myfile);

  fclose(myfile);
}

void build_about() {
// New strings
  char *filename = "about";
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *myfile = fopen(filepath, "w");

  fprintf(myfile, html_head, "About", "about");
  fputs(html_header, myfile);
  fputs(html_nav, myfile);

  fputs("<main class='about'>"

    "<h1>About</h1><p>We started <b>Grimgrains</b> to teach ourselves how to cook. This blog, which now doubles as a travel diary, helps track our habits as we adapt to the localy available produce. All of these recipes are plant-based, and tree-nut free.</p><img src= '../media/pages/about/galley.jpg'/>"

    "<p>We are <a href='http://kokorobot.ca/' class='external' target='_blank'>Rekka Bellum</a>(illustrator) and <a href='http://xxiivv.com' class='external' target='_blank'>Devine Lu Linvega</a>(developer), living on a small sailboat somewhere on the shores of the Pacific Ocean. <a href='http://100r.co' class='external' target='_blank'>Hundredrabbits</a> is the name of our floating studio, we write softwares and recipes as we sail around the world looking for new ingredients.</p>"

    "<p>This website showcases our favourite creations, along with hand-drawn illustrations for each ingredient. The pages were generated entirely in the classic C99 programming language, and the sources can be found on <a href='https://github.com/hundredrabbits/Grimgrains' class='external' target='_blank'>Github</a>.</p>"

    "<p>If you have any question, you can find us on on <a rel='me' href='https://merveilles.town/@rek'>Mastodon</a>.</p><img src= '../media/interface/toast.jpg' width='300'/>"

    "<h3>Copyright policy</h3><p>You may adapt our recipes, but a link back or mention would be nice :).</p>"

    "<p>The assets of grimgrains.com, unless stated otherwise, are <a href='http://100r.co/' class='external' target='_blank'>©hundredrabbits</a>. Photographs, drawings or text should not be used, published, reprinted or modified without our permission. The source code of the website is under the <a href='https://github.com/hundredrabbits/Grimgrains/blob/master/LICENSE' class='external' target='_blank'>MIT License</a>.</p>"

    "<p>All recipes featured on grimgrains.com are our own, unless stated otherwise. The information is for food enthusiasts like ourselves, we do not claim to be all-knowing. Nor are we health professionals. Our views are our own, we encourage openness and curiosity whenever possible.</p>"

    "<h3>Privacy statement</h3><p>We reserve the right to alter the blog at our own discretion. Words addressed to us in private will not be shared, nor will we use any of it in future publications.</p>"

    "</main>", myfile);

  fputs(html_footer, myfile);

  fclose(myfile);
}

void build_meals() {
// New strings
  char *filename = "meals";
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *myfile = fopen(filepath, "w");

  fprintf(myfile, html_head, "Meals", "meals");
  fputs(html_header, myfile);
  fputs(html_nav, myfile);

  fputs("<main>", myfile);
  char buffer[4096];
  FILE *fp = fopen("inc/meals.htm", "r");
  if(fp == NULL){ return; }

  for (;;) {
    size_t sz = fread(buffer, 1, sizeof(buffer), fp);
    if (sz) {
      fwrite(buffer, 1, sz, myfile);
    } else if (feof(fp) || ferror(fp)) {
      break;
    }
  }   
  fclose(fp);
  fputs("</main>", myfile);

  fputs(html_footer, myfile);

  fclose(myfile);
}

void build_nutrition() {
// New strings
  char *filename = "nutrition";
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *myfile = fopen(filepath, "w");

  fprintf(myfile, html_head, "Nutrition", "nutrition");
  fputs(html_header, myfile);
  fputs(html_nav, myfile);

  fputs("<main class='nutrition'>", myfile);
  char buffer[4096];
  FILE *fp = fopen("inc/nutrition.htm", "r");
  if(fp == NULL){ return; }

  for (;;) {
    size_t sz = fread(buffer, 1, sizeof(buffer), fp);
    if (sz) {
      fwrite(buffer, 1, sz, myfile);
    } else if (feof(fp) || ferror(fp)) {
      break;
    }
  }   
  fclose(fp);
  fputs("</main>", myfile);

  fputs(html_footer, myfile);

  fclose(myfile);
}

void build_tools() {
// New strings
  char *filename = "tools";
  char filepath[STR_BUF_LEN];
  snprintf(filepath, STR_BUF_LEN, "../site/%s.html", filename);
  FILE *myfile = fopen(filepath, "w");

  fprintf(myfile, html_head, "Tools", "tools");
  fputs(html_header, myfile);
  fputs(html_nav, myfile);

  fputs("<main class='tools'>", myfile);
  char buffer[4096];
  FILE *fp = fopen("inc/tools.htm", "r");
  if(fp == NULL){ return; }

  for (;;) {
    size_t sz = fread(buffer, 1, sizeof(buffer), fp);
    if (sz) {
      fwrite(buffer, 1, sz, myfile);
    } else if (feof(fp) || ferror(fp)) {
      break;
    }
  }   
  fclose(fp);
  fputs("</main>", myfile);

  fputs(html_footer, myfile);

  fclose(myfile);
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

  build_about();
  printf("Built about\n");

  build_nutrition();
  printf("Built nutrition\n");

  build_tools();
  printf("Built tools\n");

  build_meals();
  printf("Built meals\n");

  return (0);
}
