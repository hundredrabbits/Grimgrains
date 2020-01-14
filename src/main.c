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

  fputs("<main class='meals'>"
      "<p>A record of what Devine & I eat, everyday, for a period of 2 weeks. All recipes are portioned for 2 people, except breakfasts (we don't eat the same thing).</p>"
      "<p><a href='#day_1'>day 1</a> | <a href='#day_2'>day 2</a> | <a href='#day_3'>day 3</a> | <a href='#day_4'>day 4</a> | <a href='#day_5'>day 5</a> | <a href='#day_6'>day 6</a> | <a href='#day_7'>day 7</a>"
      "<h1><a id='day_1'>Day 1</a></h1><h2>Breakfast</h2><ul><li>230 g (1 cup) quick oats (plain)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>5 ml (1 tsp) ground flax seeds</li><li>60g (1/3 cup) blueberries</li></ul><ul><li>30g (1/2 cup) wholegrain O's cereal</li><li>25g (1/4 cup) granola (homemade)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60g (1/3 cup) blueberries</li></ul>"
      "<h2>Lunch</h2><ul><li>4 Veggie patties</li><li>2 Black olive ciabattas</li><li>10 ml (2 tsp) Dijon mustard</li><li>2 pickle</li><li>Sauerkraut</li><li>2 apples</li></ul><p>Bring a pan to high heat, add some vegetable oil and 1/2 chopped yellow onion, cook until browned, then add 2 minced garlic cloves. Cook for 1 minute, add 1 grated carrot, 1 grated potato and 113g (1/4) grated block of firm tofu. Cook for 5 minutes, add some ground cumin, black pepper and salt, and cook for an additional 5 minutes. Transfer veggies to bowl, process into a paste with an immersion blender or food processor, add 1/2 cup of powdered oats (quick oats that are ground into a powder). Shape into patties, and cook into a pan with a bit of oil until browned on both sides. Makes about 6 patties.</p>"
      "<h2>Dinner</h2><ul><li>3 slices slices of <a href='https://www.lacuisinedejeanphilippe.com/recipe/tourtiere-de-millet/'>millet tourtiere</a></li><li>4 blocks of sea salt black chocolate</li></ul>"
      , myfile);
  fputs("<h1><a id='day_2'>Day 2</a></h1><h2>Breakfast</h2><ul><li>230 g (1 cup) quick oats (plain)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>5 ml (1 tsp) ground flax seeds</li><li>60 g (1/3 cup) blueberries</li> </ul><ul><li>30 g (1/2 cup) wholegrain O's cereal</li><li>25 g (1/4 cup) granola (homemade)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60 g (1/3 cup) blueberries</li></ul>"
    "<h2>Lunch</h2><p><b>Warm lentil salad:</b></p><ul><li>400 ml cooked brown lentils (140ml dry)</li><li>15 ml (1 tbsp) soy sauce</li><li>15 ml (1 tbsp) maple syrup (sweetener)</li><li>15 ml (1 tbsp) apple cider vinegar</li><li>5 ml (1 tsp) dijon mustard</li></ul><p>Cook lentils with vegetable broth (for better flavor). Mix the sauce ingredients together in a bowl, pour over lentils.</p> <p><b>Roasted potatoes and brussel sprouts:</b></p><ul><li>225 g brussel sprouts (of whatever green veggie you have)</li><li>Sweet potatoes</li><li>15 ml of olive oil</li><li>1 minced garlic clove</li><li>5 ml balsamic vinegar</li><li>5 ml brown sugar</li></ul><p>Heat over to 220˚C (425°F). Cut stem and remove outer leaves from brussel sprouts, cut in half. Cut sweet potatoes into thin wedges. In a bowl, mix olive oil, minced garlic, balsamic vinegar and brown sugar, mix well. Add cut sweet potatoes and brussel sprouts, mix until well coated. Spread the veggies on a baking tray, and bake for 30 minutes.</p><p>Mix all the ingredients together, and season with salt and pepper.</p>"
    "<h2>Dinner</h2><ul><li>Left-over slices slices of <a href='https://www.lacuisinedejeanphilippe.com/recipe/tourtiere-de-millet/'>millet tourtiere</a></li><li>Sauerkraut</li><li>2 apples</li></ul>", myfile);
fputs("<h1><a id='day_3'>Day 3</a></h1><h2>Breakfast</h2><ul><li>230 g (1 cup) quick oats (plain)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>5 ml (1 tsp) ground flax seeds</li><li>60 g (1/3 cup) blueberries</li></ul><ul><li>30 g (1/2 cup) wholegrain O's cereal</li><li>25 g (1/4 cup) granola (homemade)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60 g (1/3 cup) blueberries</li></ul>"
  "<h2>Lunch</h2><p>Wholegrain wheat linguine pasta with a roasted beet sauce.</p><ul><li>190g linguine (140ml dry)</li><li><a href='lentils_with_roasted_beet_sauce.html'>Roasted beet pasta sauce</a><li><li>60 ml marinated eggplants</li><li>Apples (dessert)</li></ul>"
  "<h2>Dinner</h2><p>Made <a href='https://grimgrains.com/site/spicy_stirfry_chickpeas.html'>spicy stir-fried chickpeas</a>. Served up with some crackers I made with left-over buckwheat groats from <a href='buckwheat_tea.html'>buckwheat tea</a> I'd saved. I swapped out the oats in the recipe for the buckwheat.</b></p><ul><li>Homemade <a href='crackers.html'>buckwheat and spelt crackers</a></li><li>4 squares of sea salt chocolate (dessert)</li><li>Fruit leather (double dessert)</li></ul>", myfile);
fputs("<h1><a id='day_4'>Day 4</a></h1><h2>Breakfast</h2><ul><li>230 g (1 cup) quick oats (plain)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60g (1/3 cup) blueberries</li></ul><ul><li>30g (1/2 cup) wholegrain O's cereal</li><li>25g (1/4 cup) granola (homemade)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60g (1/3 cup) blueberries</li></ul>"
    "<h2>Lunch</h2><p>Wholegrain linguine with spicy garlic sauce</p><ul><li>170g (6oz) linguine</li><li>170 g Broccoli, cut into small florets</li><li>115g cubed firm tofu</li><li>3 garlic cloves</li><li>30 ml (2 tbsp) olive oil</li><li>2.5 g (1/2 tsp) chili flakes</li><li>1.25 g (1/4 tsp) salt</li></ul><p>Bring water to a boil in a large pot. Add pasta and cook until al dente (6-8 minutes). Drain pasta, but reserve some of the cooking water, as it will be re-used to create the sauce. Don't rinse the pasta.</p><p> Heat pan to medium. When hot add 15 ml (1 tbsp) of olive oil. Add firm tofu (chopped into tiny cubes for more surface area) into the pan. Pan-fry for 3-5 minutes, then add brocoli florets. Cook for an additional 2-3 minutes. Transfer to another bowl for later. In that same pan, add more oil (about 30 ml (2 tbsp) olive oil) and 3 smashed (not minced) garlic cloves. Sauté 1-2 minutes. Remove garlic, minced it, and reserve it for later. Add red pepper flakes to your pan to infuse the olive oil for 1-2 minutes. Add cooked pasta, minced garlic tofu and broccoli, and toss to coat. Remove from heat, add salt and black pepper to taste.</p>"
    "<h2>Dinner</h2><p>We ate out! We went to a Sichuanese and Thai cuisine restaurant in town. Shared 4 plates (for 3 ppl) consisting of:<ul><li>Crispy spinach (entree)</a></li><li>Mixed vegetables with tofu in peanut butter sauce</li><li>Pad thai (requested without fish sauce, eggs)</li><li>Broccoli in a garlic sauce</li><li>Small bowl of rice</li><li>3 fortune cookies</li></ul>", myfile);
fputs("<h1><a id='day_5'>Day 5</a></h1><h2>Breakfast</h2><ul><li>230 g (1 cup) quick oats (plain)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60g (1/3 cup) blueberries</li></ul><ul><li>30g (1/2 cup) wholegrain O's cereal</li><li>25g (1/4 cup) granola (homemade)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60g (1/3 cup) blueberries</li></ul>"
    "<h2>Lunch</h2><p>Deconstructed pate chinois. This a type of 'sheperd's pie' i like to make that isn't baked, i just prepare ingredients invidually and them just throw them in a bowl together. I vary the dish a lot, depending what I have available. I used cauliflower, but sometimes I use sweet potatoes, white potatoes etc. Instead of scrambled chickpeas I sometimes used tofu (grated, seasoned and sauteed with onions) or brown lentils.</p><ul><li>1/2 head white cauliflower</li><li>60g spicy marinated eggplants</li><li>1 small can of corn</li><li>1 recipe <a href='scrambled_chickpeas.html'>scrambled chickpeas</a></li><li>Hot sauce</li></ul><p>Cut the white cauliflower into small flowers. Steam in a basket over a pot of water for 5 min or so. Then process into puree with a handstick blender. I like plain cauliflower with a bit of salt, but if you want more flavor you can add a bit of vegan butter.</p><p>Add a layer of corn to the bottom of two bowls, add the scrambled chickpea, add the pureed cauliflower and then the marinated eggplants. Season with black pepper and top with some spicy marinades! I used some spicy marinated eggplants.</p>"
    "<h2>Dinner</h2><p>Lasagna with sunflower seed parmesan.</p></p><ul><li>Marinara sauce</li><li><a href='quick_sunflower_seed_parmesan.html'>Sunflower seed parmesan</a></li><li>1 box of wholegrain lasagna </li><li>1 zucchini</li><li>30 g soy protein</li><li>sunflower seed 'cheese'</li></ul><p>Pour hot water over 140 g (1 cup) sunflower seeds, let soak for 10 minutes. Drain and rinse and transfer to a bowl. Add these ingredients to the bowl: 340 g (1 pack) of soft tofu, 45 g (3 tbsp) nutritional yeast, 5 g (1 tsp) garlic powder and 1.25 g (1/4 tsp) of salt. Mix, and puree using a handstick blender (or food processor).</p><p>To make marinara sauce, bring a pan to medium heat with a bit of olive oil. Add 3 minced garlic cloves cook for a minute, then add 5 g chili pepper flakes and cook for another minute. Add one 795 ml can (280 oz) of diced tomatoes (no salt or spices added), 1 diced (cubed) zucchini, 30 g soy protein, 5 g (1 tsp) dried oregano and 15 g (1 tbsp) of tomato paste. Bring to high heat, then lower to a simmer and cook for 45 minutes. Finally, mix in 45 g (3 tbsp) of nutritional yeast, fresh basil and salt.</p><p>Preheat oven 176 C (to 350 F). Cook lasagna according to package instructions. Coat bottom of 9X13 baking dish with marinara sauce, then add sheets of lasagna. Add another layer of sauce, then add the sunflower/tofu 'cheese', then more lasagna. Repeat until you have 4 layers of noodles. For top of lasagna, add a layer of sauce and top with some <a href='https://grimgrains.com/site/quick_sunflower_seed_parmesan.html'>Sunflower seed parmesan</a>. Place lasagna on middle rack of oven, bake for 45 minutes. Let cool for 10 minutes before serving.</p>", myfile);
fputs("<h1><a id='day_6'>Day 6</a></h1><h2>Breakfast</h2><p>Pancakes!</p><ul><li>240 ml (1 cup) fortified soy milk</li><li>5 ml (1 tsp) apple cider vinegar</li><li>120 g (1 cup) spelt flour</li><li>5 ml (1 tsp) vanilla extract</li><li>1.25 g (1/4 tsp) baking soda</li><li>2.5 g (1/2 tsp) baking powder</li><li>5 ml (1 tsp) of vegetable oil, or vegan butter</li><li>Maple syrup</li></ul><p>Mix soy milk with apple cider vinegar to create 'buttermilk', let stand for 5 minutes. Add vanilla extract, mix well.</p><p>Mix dry ingredients together, then add soy 'buttermilk'. In a pan, heat 5 ml (1 tsp) of vegetable oil (or vegan butter) at medium heat. When pan is hot, add 30 ml (~2 tbsp) of batter into pan. Help spread into wider circle using the back of the spoon. Cook until bubbles form on the top, then flip, and cook for another 2 minutes. Repeat for rest of pancake mix and serve with maple syrup!</p>"
  "<h2>Lunch</h2><p>Had rice noodles with peanut sauce.</p><ul><li>200 g (7 oz pack) rice noodles</li><li>150 g (1/3 block) of firm tofu</li><li>15 g (1 tbsp) cornstarch</li> <li>1 Zucchini, julienned</li><li>1/2 green bell pepper, cut into thin long strips</li><li>1 clove garlic</li><li>5 g (1 tsp) chili flakes</li></ul><p>Sauce</p><ul><li>60 g (1/4 cup) peanut butter</li><li>22 ml (1 1/2 tbsp) soy sauce</li><li>10 g (2 tsp) sugar</li><li>10 g (2 tsp) cornstarch</li><li>2 garlic clove, minced</li><li>5 g (1 tsp) grated ginger</li><li>5 g (1 tsp) curry powder</li><li>60 ml (1/4 cup) water</li></ul><p>Cut tofu into small cubes. Put 15 g (1 tbsp) of cornstarch in a bowl, add tofu cubes and toss until well-coated. Keep aside.</p><p>Mix sauce ingredients (except for the water) in a bowl. Then add the water and mix until well incorporated. In a pan, heat some oil at high heat. When hot, add the minced garlic and the chili flakes, cook for 30 seconds, add zucchini and bell pepper. Add the tofu, cook until outside of tofu has crisped up. Add noodles, then add the sauce overtop. Cook for 1-2 minutes until sauce starts to thicken. Serve in two bowls.</p>"
  "<h2>Dinner</h2><p>Ate left-over lasagna from Day 5.</p></p>", myfile);
fputs("<h1><a id='day_7'>Day 7</a></h1><h2>Breakfast</h2><ul><li>230 g (1 cup) quick oats (plain)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60g (1/3 cup) blueberries</li></ul><ul><li>30g (1/2 cup) wholegrain O's cereal</li><li>25g (1/4 cup) granola (homemade)</li><li>120 ml (1/2 cup) fortified soy milk</li><li>60g (1/3 cup) blueberries</li></ul>"
  "<h2>Lunch</h2><p>Today we had one slice of lasagna left over from day 5, we split that in two and served it with steamed brussel sprouts and hearts of palm salad.</p><ul><li>10 brussel sprouts, halved</li></ul><p>Hearts of palm salad</p><ul><li>1 Whole can (398 ml) of heart of palm, cut in slices</li><li>10 ml (2 tsp) dijon mustard</li><li>30 ml (2 tbsp) soy milk</li><li>15 ml (1 tbsp) canola oil</li><li>15 ml (1 tbsp) apple cider vinegar</li><li>1.25 g (1/4 tsp) black pepper<li>1.25 g (1/4 tsp) salt</li></ul><p>Rinse brussel sprouts, cut stems, remove outer leaves, and cut in half. Place in steam basket over pot of water, steam until tender. I like to eat them as is, but you can season them with salt/pepper, vegan butter etc.</p><p>Cut hearts of palms into slices. Mix the salad dressing ingredients together until well incorporated, then add hearts of palm and mix until well-coated. Serve brussel sprouts, hearts of palm salad and lasagna into two bowls.</p>"
  "<h2>Dinner</h2><p>Vege pate over slices of spelt bread, with spicy marinated eggplants, sauerkraut and veggies on the side. I made vege pate a month ago. The recipe makes 4 blocks, so I unfroze one this morning to thaw for dinner. My parents have a bread machine, so today I made some spelt sandwich bread with it. Started it after lunch and the loaf was ready for dinner.</p><ul><li><a href='veggie_pate.html'>1 block of vege pate</a></li><li>4 slices of spelt bread</li><li>Spicy marinated eggplants (store-bought)</li><li>Sauerkraut</li><li>1 carrot (peeled and cut into sticks)</li><li>1 green bell pepper (cut into thin slices)</li><li>Dried fruit for dessert</li></ul>", myfile);
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
  fputs("<h1><a id='nutrition'>Nutrition</a></h1><p>The information in this guide may change as research develops. If you are concerned about your intake of vitamin and minerals, we highly recommend seeing a <b>dietitian</b> who specializes in plant-based diets.</p><ul><li><a href='#protein'>Protein</a></li><li><a href='#iron'>Iron</a></li><li><a href='#calcium'>Calcium</a></li><li><a href='#vitamin-d'>Vitamin D</a></li><li><a href='#vitamin-b12'>Vitamin B12</a></li><li><a href='#iodine'>Iodine</a></li><li><a href='#omega-3'>Omega-3's</a></li><li><a href='#vitamin-a'>Vitamin A</a></li><li><a href='#zinc'>Zinc</a></li><li><a href='#meal-plans'>Meal plans</a></li><li><a href='#nutrition-references'>Nutrition references</a></li></ul><img src='../media/pages/nutrition/nutrition.png'>", myfile);
  fputs("<h3>General rules:</h3><ul><li>Eat at least <b>4 servings of vegetables</b> per day.</li><li>Eat at least <b>3 servings of legumes</b> per day.</li><li>Eat at least <b>4 servings of grains</b> per day.</li><li>Eat at least <b>3 servings of fruits</b> per day.</li><li>Eat at least <b>1 serving of seeds</b> per day.</li></ul><h3><a id='protein'>Protein</a></h3><img src='../media/pages/nutrition/protein.png'><p>Proteins are necessary for the growth and repair of muscles, bones, hormones, digestive enzymes and to absorb nutrients. They are composed of <b>20 amino acids</b>, 11 of which our bodies can synthesize. The other 9 must come from the food that we eat, they are called the 'essential amino acids'.</p><h4>Sources</h4><p>Consume 3-4 servings of:</p><ul><li>1/2 cup of dried <a href='lentils.html'>lentils</a> or beans</li><li>1/2 cup <a href='tofu.html'>tofu</a> or <a href='tempeh.html'>tempeh</a></li><li>1 cup <a href='soy_milk.html'>soy milk</a></li><li>1 cup quinoa or amaranth</li><li>1/4 cup <a href='peanuts.html'>peanuts</a>(or, 2 tbsp <a href='peanut_butter.html'>peanut butter</a>)</li></ul><p>Every plant protein contain all 9 of the essential amino acids, but in varying amounts. The body maintains reserves of amino acids for later use, the proteins of one meal <b>can combine with proteins from another meal eaten later that day</b>.</p><p>Legumes are the best source of <b>lysine</b>, one of the essential amino acids. Quinoa and amaranth are two grains that are rich in lysine, although not as much as legumes.</p><p><a href='https://veganhealth.org/protein-part-1/' class='external' target='_blank'>Read more</a> about proteins. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='iron'>Iron</a></h3><img src='../media/pages/nutrition/iron.png'><p>Iron is necessary for red blood cells and to form hemoglobins which helps circulate oxygen around the body. We lose some of this mineral everyday from the skin and intestines, menstruating women will lose a lot of it.</p><h4>Sources</h4><ul><li>Beans</li><li>Soy (high in iron and not affected by phytic acid)</li><li>Nuts</li><li>Seeds</li><li>Winter squashes</li><li>Dark leafy greens</li><li>Dried fruit</li><li>Oatmeal</li><li>Quinoa</li><li>Pearled barley</li><li>Whole grain bread</li></ul><p>Iron-rich plant foods are high in <b>phytic acid</b>, a compound that binds to iron and other minerals and that makes it difficult for our bodies to absorb. Avoiding foods that contain phytic acid isn't necessary, but it's important to eat them in a way that minimizes their effects on iron.</p><p>Iron is plentiful in vegan diets, you can increase your absorption of this vitamin by adding vitamin C to your meals (can increase absorption by 4-6X), avoiding tea (chamomille or pepppermint) of coffee at meals, eating more legumes, eating fermented grains (like yeast-leavened bread), cooking foods in a cast iron skillet (as the iron leeches into the food), and by avoiding calcium with meals.</p><p>Other iron inhibitors include: turmeric and coriander, or calcium rich foods.</p><p><a href='https://veganhealth.org/iron/' class='external' target='_blank'>Read more</a> about iron. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='calcium'>Calcium</a></h3><img src='../media/pages/nutrition/calcium.png'><h4>Sources</h4><p>Consume at least 3 1/2 to 4 cups a day of:</p><ul><li>Calcium-set tofu</li><li>Cooked Chinese cabbage</li><li>Turnip greens</li><li>Mustard greens</li><li>Collards</li><li>Fortified plant milks (shake well before consuming)</li><li>Fortified juices.</li></ul><p>This, with extra servings of beans, broccoli, <a href='kale.html'>kale</a>, okra, <a href='sweet_potatoes'>sweet potatoes</a>, figs, <a href='corn_tortillas'>corn tortillas</a>, <a href='oranges.html'>navel oranges</a> and blackstrap molasses.</p><p>The amount of calcium in plant foods is somewhat dependent on the amount of calcium in the soil. The World Health Organization recommends 1,000mg of calcium per day, and 1,200mg if over 50, but what is most important is how much is truly absorbed. Most people absorb 25-30 percent of calcium. Absorption varies depending on food sources, so <b>it's important to consume sources of well-absorbed calcium</b>, like <b>calcium-set tofu</b>, <b>kale</b>, <b>turnip greens</b>, <b>mustard greens</b> and <b>broccoli</b>. Beans, nuts and seeds have moderate amounts of calcium and a slow absorption rate, soaking nuts and seeds aids in absorption. Fortified plant milk generally contains calcium with an absorption rate of 25%, similar to calcium-set tofu, although it is important to shake it before using it as the calcium tends to set at the bottom.</p><p><a href='https://veganhealth.org/oxalate/' class='external' target='_blank'>Oxalate molecules</a>, found in certain plant foods (such as <a href='spinach.html'>spinach</a>, <a href='beets'>beets</a>, beet greens, <a href='sweet_potatoes'>sweet potatoes</a>, <a href='peanuts.html'>peanuts</a>, rhubarb, and swiss chard), can prevent the absorption of calcium. Oxalates plays a part in calcium-oxalate kidney stones. Consuming calcium and high-oxalate foods is fine and necessary for good health, but it's important to drink plenty of fluids. Doing this helps oxalate and calcium “bind” to one another in the stomach and intestines before reaching the kidneys, making it less likely for kidney stones to form. Boiling high-oxalate greens before consumption and discarding the cooking water also helps.</p><p><a href='https://veganhealth.org/calcium/' class='external' target='_blank'>Read more</a> about calcium. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='vitamin-d'>Vitamin D</a></h3><img src='../media/pages/nutrition/vitamind.png'><p>Vitamin D can help prevent osteoporosis by increasing calcium absorption in the body.</p><p>We make plenty of vitamin D when our skin is exposed to sunlight, but because of smog, cloud coverage and sunscreen, we may be producing less than our body requires. Light-skinned people require 10-15 minutes in the sun, while dark-skinned people might need <a href='https://veganhealth.org/vitamin-d-part-2-the-research/#dark' class='external' target='_blank'>two hours</a>. In winter, it is important to get a dietary source of this nutrient, especially for people with darker skin.</p><h4>Sources</h4><ul><li>The sun</li><li>Fortified plant milk</li><li>Fortified foods</li><li>Fortified margarine</li><li><a href='mushrooms.html'>Mushrooms</a> (UV ray-treated)</li></ul><p>There are two forms of supplemental vitamin D:</p><ul><li>Ergocalciferol or D2</li><li>Cholecalciferol or D3</li></ul><p>D2 is vegan, made from exposing fungi to UV Rays, while D3 comes from fish oil or sheep's wool. Vitamin D2 is not as effective as vitamin D3 if taken weekly, it must be consumed daily.</p><p><a href='https://veganhealth.org/vitamin-d/' class='external' target='_blank'>Read more</a> about vitamin D. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='vitamin-b12'>Vitamin B12</a></h3><img src='../media/pages/nutrition/vitaminb12.png'><p>Vitamin b-12 is needed for the production of DNA and for maintaining nerve cells. Levels need to be over 400 pg/ml — or 4 to 6 mcg per day — for good health. This vitamin does not occur naturally in any plant.</p><h4>Sources</h4><ul><li>Fortified plant milks</li><li>Fortified foods</li><li><a href='nutritional_yeast'>nutritional yeast</a></li><li>Cyanocobalamin Supplements</li></ul><p>Eat two servings a day of fortified foods that contain at least 2-3.5 mcg of vitamin B12, these need to be consumed at least 4h apart for optimal absorption. Another way, is to take a daily supplement of <b>cyanocobalamin B12</b> that provides 25-100 mcg of vitamin B12, or a supplement that provides 1,000 mcg twice per week.</p><p>Absorption of vitamin b-12 is tricky, because small amounts attach to receptors that become quickly saturated by just 1-2 mcg of B12 and will stay like this for several hours. This is what you get from a single meal of fortified foods. Ingesting a large dose of B12 from a supplement, you can absorb the excess that can't latch onto receptors, but that is a very small amount. It is easier to meet needs from many small servings of B12 to be eaten throughout the day, but if you rely on daily single-dose supplements you need to have a lot more since absorption rates are very low.</p><p><a href='https://veganhealth.org/vitamin-b12/' class='external' target='_blank'>Read more</a> about vitamin B12. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='iodine'>Iodine</a></h3><img src='../media/pages/nutrition/iodine.png'><p>Iodine is necessary for healthy thyroid function which regulates our metabolism.</p><h4>Sources</h4><ul><li>Some fortified plant milks (few are fortified with it)</li><li><a href='salt.html'>Iodized salt</a>, 1/4 tsp/day</li><li><a href='seaweed.html'>Seaweed</a></li></ul><p>A quantity of 90 micrograms is recommended daily.</p><p>Note that the amount of iodine in <a href='seaweed.html'>seaweed</a> and other sea vegetables is variable, it is the same for sea salt and other natural salts. <b>The only reliable sources are <a href='salt.html'>iodized salt</a> or a supplement</b>.</p><p><a href='https://veganhealth.org/iodine/' class='external' target='_blank'>Read more</a> about iodine. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='omega-3'>Omega-3's</a></h3><img src='../media/pages/nutrition/omega3.png'><p>Omega3's are needed for optimal cognition, and helps reduce blood clotting (which protects against heart attacks).</p><h4>Sources</h4><ul><li><a href='chia_seeds'>Chia seeds</a>, 1 tsp | 713mg</li><li><a href='flax_seeds'>Flax seeds</a>, 1 tsp, ground | 570mg</li><li><a href='soy_beans.html'>soy beans</a>, cooked, 1/2 cup | 500mg</li><li><a href='tofu.html'>Tofu</a>, 1 cup | 400mg</li><li><a href='tempeh.html'>Tempeh</a>, 1 cup | 400mg</li><li><a href='canola_oil'>Canola oil</a>, 1 tsp | 433mg</li><li>Flaxseed oil, 1/4 tsp | 608mg</li></ul><p>There are 3 omega-3 fatty acids:</p><ul><li><b>Alpha linolenic (ALA)</b>, found in soy, walnuts, canola oil, flaxseeds, <a href='shelled_hemp_seeds.html'>hemp seeds oil</a> etc. The body cannot produce its own.</li><li><b>Eicosapentaneoic acid (EPA)</b>, found in small amounts in <a href='seaweed.html'>seaweed</a>.</li><li><b>Docosahexaneoic acid (DHA)</b>, found in small amounts in seaweed. The body can convert EPA into DHA.</li></ul><p>Adult males require 1,600 mg/day while females require 1,100 mg/day of ALA. The numbers vary for <a href='https://veganhealth.org/daily-needs/#Omega-3-Fats' class='external' target='_blank'>babies, infants and pregnant women</a>. To be safe, it is recommended to take 200-300 mg of DHA per day.</p><p><a href='https://veganhealth.org/omega-3s-part-2/' class='external' target='_blank'>Read more</a> about omega-3's. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='vitamin-a'>Vitamin A</a></h3><img src='../media/pages/nutrition/vitamina.png'><p>Vitamin A is important for growth and developments in young children, and for red blood cell formation.</p><h4>Sources</h4><p>Consume 2 or more per day.</p><ul><li>Butternut squash</li><li><a href='acorn_squash.html'>Pumpkin</a></li><li><a href='sweet_potatoes'>Sweet potatoes</a></li><li><a href='kale.html'>Kale</a></li><li><a href='carrots.html'>Carrots</a></li><li><a href='spinach.html'>Spinach</a></li><li>Cantaloupe</li></ul><p>The body can convert 50 carotenoids into vitamin A, the most common being <b>beta-carotene</b>. Eating vegetables high in carotenoids (carrots, pumpkin etc) with some fat increased the absorption and synthesis of Vitamin A.</p><p><a href='https://veganhealth.org/vitamin-a/' class='external' target='_blank'>Read more</a> about vitamin A. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='zinc'>Zinc</a></h3><img src='../media/pages/nutrition/zinc.png'><p>Zinc is important for immunity.</p><h4>Sources</h4><ul><li><a href='chickpeas.html'>Chickpeas</a>, 1/3 cup (cooked)</li><li><a href='peanuts.html'>Peanuts</a>, 1/4 cup (raw)</li><li><a href='oats'>Oatmeal</a>, 1 cup (cooked)</li><li><a href='tempeh/html'>Tempeh</a>, 1/2 cup</li><li>Bread</li></ul><p>Adult males (19-50yrs) require 11mg/day, while adult females (19-50yrs) require 40mg/day. <a href='https://veganhealth.org/daily-needs/#zinc' class='external' target='_blank'>These amounts vary</a> for newborns, infants, teens, pregnant/breastfeeding women and older adults.</p><p><a href='https://www.nap.edu/read/10026/chapter/14#480' class='external' target='_blank'>Phytates</a>, found in some plant foods, reduce zinc absorption. This is why it's important to increase our intake of zinc by at least 50%. Consuming protein — like legumes, leavened bread, fermented soy foods (<a href='miso.html'>miso</a> or <a href='tempeh.html'>tempeh</a>) and nuts&nbsp;– increases zinc absorption.</p><p><a href='https://veganhealth.org/zinc/' class='external' target='_blank'>Read more</a> about zinc. Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='meal-plans'>Meal plans</a></h3></p><ul><li><a href='https://www.vrg.org/journal/vj2006issue2/2006_issue2_mealplans.php' class='external' target='_blank'>Low-Cost Vegan Meal Plans</a></li><li><a href='https://veganhealth.org/meal-plans/' class='external' target='_blank'>Calorie-Based Vegan Meal Plans</a></li></ul><p>Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
  fputs("<h3><a id='nutrition-references'>Nutrition references</a></h3></p><ul><li><a href='https://academic.oup.com/jn/article/137/4/1097/4664654' class='external' target='_blank'>Processing and Preparation Practices to Enhance the Bioavailability of Micronutrients in Plant-Based Diets</a></li><li><a href='https://www.dsm.com/content/dam/dsm/nip/en_US/documents/stability.pdf' class='external' target='_blank'>Fortification basics</a></li><li><a href='https://www.theveganrd.com/' class='external' target='_blank'>The vegan RD</a></li><li><a href='https://veganhealth.org/' class='external' target='_blank'>Vegan Health</a></li><li><a href='https://nutritionfacts.org/' class='external' target='_blank'>Nutrition Facts</a></li><li><a href='https://world.openfoodfacts.org/' class='external' target='_blank'>Open Food Facts</a></li></ul><p>See a mistake? <a href='https://github.com/hundredrabbits/Grimgrains' class='external' target='_blank'>Make a pull request</a> on Github. Or contact us on <a href='https://twitter.com/grimgrains' class='external' target='_blank'>twitter</a> or <a href='https://merveilles.town/@rek' class='external' target='_blank'>mastodon</a>. Information last updated on 15/11/19.</p><p>Jump back to <a href='#nutrition'>top</a>.</p>", myfile);
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
  fputs("<h1>Tools</h1><p>Entering a store, with the goal of purchasing kitchenware can be a dizzying experience. There are many choices, but few are worth your money and attention. An ideal kitchen will have as few items as possible, be composed of things that make you happy and that you chose with purpose. With this in mind, for a tool to be awarded a place in your home, it needs to be effective and versatile. I made this list of notes on what to look for in a tool, based on experience and research:</p><img src='../media/pages/tools.jpg'><ul><li><b>Durability</b>: Choose a material that will last, of finer grade, or that can be composted.</li><li><b>Simplicity</b>: Prioritize unibody tools, the fewer parts the better. Opting for non-electric is important for that same reason. Less prone to breakage.</li><li><b>Multi-functional</b>: Having too many tools that have a single function, adds clutter to our home and in our minds.</li></ul><p>Note: If you look at the photo, you can see that my kitchenware doesn't have all the features listed below. That's because this list is an ideal, a study of what an optimal toolset would be. It may not be realistic to you, and that's okay. In the end, you've got to work with what you have. The resulting food matters <b>more</b> than the <a id='tools'>tools</a> you use.</p><ul><li><a href='#cutting-boards'>Cutting boards</a></li><li><a href='#coffee-grinder'><Manual coffee grinder</a></li><li><a href='#mortar-and-pestle'>Mortar and pestle</a></li><li><a href='#cleaver'>Cleaver</a></li><li><a href='#shears'>Shears</a></li><li><a href='#rolling-pin'>Rolling pin</a></li><li><a href='#pressure-cooker'>Pressure cooker</a></li><li><a href='#skillet'>Skillet</a></li></ul>", myfile);
  fputs("<h3><a id='cutting-boards'>Cutting boards</a></h3><p><b>Material</b>: Opt for a maple or bamboo hardwood cutting board. Avoid plastic. Contrary to popular belief, it is not more sanitary and can harbour a lot of bacteria. A knife-scarred plastic surface is difficult to disinfect, and studies have shown that washing the board – whether by hand or with a dishwasher – was not enough to kill lingering bacteria.</p><p>Wood doesn't scar as easily, and is the preferred material. After cleaning, it must be allowed to dry thoroughly.</p><ul><li><a href='https://www.johnboos.com/mailchimppdf/WOODvsPLASTIC-GERMANSTUDY2015-V1.pdf' class='external' target='_blank'>Comparative studies on hygienic qualities of wood and plastic cutting boards in a laboratory</a> Dr. Ulrike Kleiner, the Laboratory of Hygiene Research at the Anhalt University in Bernburg, Germany</li><li><a href='../media/blog/ucdavisfoodsafety.cuttingboard.pdf'>Plastic and wooden cutting boards</a> Dr. Dean O. Cliver, University of California, USA</li></ul><p>Jump back to <a href='#tools'>top</a></p>.", myfile);
  fputs("<h3><a id='coffee-grinder'>Manual coffee grinder</a></h3><p>If you enjoy <a href='coffee.html'>coffee</a>, getting a grinder should be on the top of your list. Grinding your own beans, means you can choose the size of the grind to match the selected brewing method.</p><p><b>Material</b>: Choose a manual grinder with ceramic burrs, the two plates crush the beans into even grinds, and is the best way to release the oils that are responsible for the flavour of the coffee. Steel burrs, while cheaper, don't last as long as the ceramic kind. Aim for long-lasting, rather than cheap. As for blade grinders, they grind unevenly and will not do.</p><ul><li><a href='https://www.nature.com/articles/srep24483' class='external' target='_blank'>The effect of bean origin and temperature on grinding roasted coffee</a> Uman, E. et al. Sci. Rep. 6, 24483; doi: 10.1038/srep24483, 2016</li></ul><p>Jump back to <a href='#tools'>top</a></p>", myfile);
  fputs("<h3><a id='mortar-and-pestle'>Mortar and pestle</a></h3><p>Unlike other types of tools, your intended use will determine the choice of material of your future mortar and pestle. Will you grind spices with it? Nuts? Fibrous herbs?</p><p><b>Material</b>: The best material, for the most varied jobs, is granite. Its weight and irregular surface makes it easy to grind most spices, nuts, and can handle fibrous herbs well enough.</p><p>If you have no intention of grinding nuts and herbs, a marble mortar with a smooth surface will do fine. Wood mortars are discouraged, that is, unless you want to re-use it for the same purpose over and over again. Flavor lingers into the bowl, and may result in unwanted tastes in other recipes. There are other materials to consider, but all in all, a heavy, textured, granite bowl is your best bet as a good all-around mortar.</p><p>Jump back to <a href='#tools'>top</a></p>", myfile);
  fputs("<h3><a id='cleaver'>Cleaver</a></h3><p>The cleaver, or the chinese chef's knife, is a less brutish version of the meat cleaver. This tool has a wide variety of purposes, with it, you can dice, slice and julienne vegetables. After the item has been cut into bits, you can use the wide blade to scoop everything up. There is no real need for other knives in your arsenal.</p><p><b>Material</b>: When selecting your cleaver, aim for a harder grade of steel – somewhere between 57-58 on the <a href='https://en.wikipedia.org/wiki/Rockwell_scale' class='external' target='_blank'>Rockwell Hardness Scale</a>. Good steel produces a finer edge, and holds its sharpness. The angle of the blade is also important, 22 degrees is most common and this too, ensures longer-lasting sharpness. Traditionally, Chinese cleavers are made of carbon steel, but because these are prone to rust stainless steel is preferred (a mixture of carbon and SS is also good).</p><p><b>Shape</b>: The handle of the cleaver should not be too thick or too small, the size has to allow the hand to wrap around it and to just touch the other side of your thumb.</p><p>Jump back to <a href='#tools'>top</a></p>", myfile);
  fputs("<h3><a id='shears'>Shears (Nigiri)</a></h3><p>Traditional Japanese shears, or Nigiri, can be used to cut some herbs from the garden to add to a meal, or cutting dried mushrooms or chilies into smaller bits, there are over 100 uses for it.</p><p><b>Material</b>: Most shears are made from carbon steel, stainless steel is again, preferable.</p><p><b>Shape</b>: Nigiri are forged using a single piece of metal and spring back to the open position when released. While the design is simple and durable, it offers more precision than scissors. A pair with blades that make more than half of the length of the tool, will permit for a greater variety of uses. The shears rest inside your hand, between your index finger and thumb, giving greater control of the speed of the cut. This tool, is also ideal for left-handed people, making cutting a pain-free experience.</p><p>Jump back to <a href='#tools'>top</a></p>", myfile);
  fputs("<h3><a id='rolling-pin'>Rolling pin</a></h3><p>A simple tool, essential for baking at home.</p><p><b>Material</b>: Choosing a hardwood like maple is recommended, because of its strength and because of how easy it is to clean.</p><p><b>Shape</b>: A good rolling pin will not have handles, and will often cost less. According to some experts, having no handles gives a better sense of the dough. Aim for a tapered french-style pin, its shape permits rolling the dough into rounds and oblongs with ease.</p><ul><li><a href='https://thewirecutter.com/reviews/best-rolling-pin/' class='external' target='_blank'>The best rolling pin</a> Wirecutter, 2017</li></ul><p>Jump back to <a href='#tools'>top</a></p>", myfile);
  fputs("<h3><a id='pressure-cooker'>Pressure cooker</a></h3><p>A stovetop pressure cooker will help save time and energy, while providing you with good meals quickly. Instead of a meal simmering for hours, all can be done within 60 minutes.</p><p><b>Material</b>: Aim for Stainless Steel.</p><p><b>Shape</b>: Pick an 8-Quart model, with a spring-loaded valve and pressure indicator. If you chose well, this is a tool that you will follow you into old age. A model with a thicker base, of at least 7mm, will retain heat better, reach pressure more quickly and will keep food from burning. Pick a cooker that offers a low pressure setting of 8-9 PSI per square inch  and a high pressure setting of 15PSI, anything else is below standard, and will result in your food not cooking well. A pressure cooker can also serve as a regular large stock-pot without the lid.</p><ul><li><a href='https://thewirecutter.com/reviews/best-pressure-cooker/' class='external' target='_blank'>The best pressure cooker</a> Wirecutter, 2018</li><li><a href='http://www.cooksillustrated.com/equipment_reviews/1398-stovetop-pressure-cookers?ref=new_search_experience_1&amp;incode=MCSCD00L0' class='external' target='_blank'>Stovetop pressure cooker</a> Cook’s Illustrated, 2013</li><li><a href='http://www.finecooking.com/articles/test-drive-pressure-cookers.aspx?pg=0' class='external' target='_blank'>Test Drive: Pressure Cookers</a> Nicki Pendleton Wood, Fine Cooking, 2011</li></ul><p>Jump back to <a href='#tools'>top</a></p>", myfile);
  fputs("<h3><a id='skillet'>Skillet</a></h3><p>Skillets are used to stir-fry, bake, simmer and cook a grand variety of dishes.</p><p><b>Material</b>: Cast-iron is the material of choice, it is durable, cheaper, and offers more versatility than stainless steel skillets. It can be used to cook in the oven, safe past 500 degrees. Cast-iron cookware gets better with time as it develops layers of seasoning. Buying a pre-seasoned skillet is best. Cooking in cast iron pans helps with your daily nutritional requirements of iron.</p><p><b>Shape</b>: A large cooking area is ideal, with large spouts to pour out contents with ease. The skillet will be heavy, rounded-stick handles make it easier and more comfortable to lift.</p><p>Jump back to <a href='#tools'>top</a></p>", myfile);
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
