#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define STR_BUF_LEN 64
#define NAME "Grimgrains"
#define DOMAIN "https://grimgrains.com/"

/* Types */

enum RecipeType {
	maindish,
	sidedish,
	sweet,
	toppings,
	snack,
	basic,
	tropical,
	lifestyle
};

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

/* Helpers */

int
slen(char *s)
{
	int i = 0;
	while(s[i] && s[++i])
		;
	return i;
}

int
clca(int c)
{
	return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c;
}

char *
scpy(char *src, char *dst)
{
	int i = 0;
	while((dst[i] = src[i]))
		i++;
	return dst;
}

int
scmp(char *a, char *b)
{
	int i = 0;
	while(a[i] == b[i])
		if(!a[i++])
			return 1;
	return 0;
}

char *
slca(char *s)
{
	int i;
	for(i = 0; i < slen(s); i++)
		s[i] = clca(s[i]);
	return s;
}

char *
scsw(char *s, char a, char b)
{
	int i;
	for(i = 0; i < slen(s); i++)
		s[i] = s[i] == a ? b : s[i];
	return s;
}

void
to_lowercase(char *str, char *target, size_t tsize)
{
	for(size_t i = 0; i < tsize; i++) {
		target[i] = str[i];
		if(!target[i])
			break;
		if(target[i] == ' ')
			target[i] = '_';
		else
			target[i] = tolower(target[i]);
	}
	target[tsize - 1] = '\0';
}

void
fpRFC2822(FILE *f, time_t t)
{
	struct tm *tm = localtime(&t);
	char *days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	fprintf(f, "%s, %02d %s %d 00:00:00 +0900", days[tm->tm_wday], tm->tm_mday, months[tm->tm_mon], tm->tm_year + 1900);
}

time_t
intdate(int date)
{
	struct tm str_time;
	int y = date / 10000;
	int m = (date / 100) % 100;
	int d = date % 100;
	str_time.tm_year = y - 1900;
	str_time.tm_mon = m - 1;
	str_time.tm_mday = d - 1;
	str_time.tm_hour = 0;
	str_time.tm_min = 0;
	str_time.tm_sec = 0;
	str_time.tm_isdst = 0;
	return mktime(&str_time);
}

/* Generics */

Ingredient
create_ingredient(char *name, char *description)
{
	Ingredient a;
	a.name = name;
	a.description = description;
	a.parent = NULL;
	return a;
}

Ingredient
create_child_ingredient(Ingredient *parent, char *name, char *description)
{
	Ingredient a;
	a.name = name;
	a.description = description;
	a.parent = parent;
	return a;
}

Serving
create_serving(Ingredient *ingredient, char *quantity)
{
	Serving a;
	a.ingredient = ingredient;
	a.quantity = quantity;
	return a;
}

char recipe_type_names[20][32] = {"main", "sidedish", "sweet", "toppings", "snack", "basic", "tropical", "lifestyle"};
int recipes_by_types_len[lifestyle + 1] = {0, 0, 0, 0, 0, 0, 0, 0};
char *recipes_by_types[lifestyle + 1][100];

void
categorize_recipe(char *name, enum RecipeType type)
{
	recipes_by_types[type][recipes_by_types_len[type]] = name;
	recipes_by_types_len[type]++;
}

Recipe
create_recipe(char *name, enum RecipeType type, char *portions, int date, int time)
{
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

RecipePart
create_part(char *name)
{
	RecipePart a;
	a.name = name;
	a.instructions_len = 0;
	a.servings_len = 0;
	return a;
}

void
set_description(Recipe *r, char *description)
{
	r->description = description;
}

void
add_instruction(RecipePart *p, char *instruction)
{
	p->instructions[p->instructions_len] = instruction;
	p->instructions_len++;
}

void
add_serving(RecipePart *p, Ingredient *i, char *quantity)
{
	p->servings[p->servings_len] = create_serving(i, quantity);
	p->servings_len++;
}

void
add_part(Recipe *r, RecipePart *p)
{
	r->parts[r->parts_len] = p;
	r->parts_len++;
}

char *html_head = "<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='description' content='Grim Grains is an illustrated food blog, it features plant-based (vegan) recipes.'><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta name='twitter:card' content='summary'><meta name='twitter:site' content='@hundredrabbits'><meta name='twitter:title' content='Grimgrains'><meta name='twitter:description' content='An illustrated food blog.'><meta name='twitter:creator' content='@hundredrabbits'><meta name='twitter:image' content='https://grimgrains.com/media/services/icon.jpg'><meta property='og:title' content='Grimgrains'><meta property='og:type' content='article'><meta property='og:url' content='http://grimgrains.com/'><meta property='og:image' content='https://grimgrains.com/media/services/icon.jpg'><meta property='og:description' content='An illustrated food blog.'><meta property='og:site_name' content='Grimgrains'><link rel='icon' type='image/x-icon' href='../media/services/favicon.ico'><link rel='icon' type='image/png' href='../media/services/icon.jpg'><link rel='apple-touch-icon' href='../media/services/apple-touch-icon.png' /><title>GrimGrains — %s</title><link rel='alternate' type='application/rss+xml' title='RSS Feed' href='../links/rss.xml' /><link rel='stylesheet' type='text/css' href='../links/main.css'></head><body class='%s'>";

char *html_header = "<header><a id='logo' href='home.html'><img src='../media/interface/logo.png' alt='Grimgrains'></a></header>";

char *html_nav = "<nav><ul><li class='home'><a href='home.html'>Home</a></li><li class='recipes'><a href='home.html#recipes'>Recipes</a></li><li class='about'><a href='about.html'>About</a></li><li class='tools'><a href='tools.html'>Tools</a></li><li class='nutrition'><a href='nutrition.html'>Nutrition</a></li><li class='right'><a href='https://grimgrains.com/links/rss.xml'>RSS feed</a> | <a href='https://merveilles.town/@rek' target='_blank'>Mastodon</a></li></ul></nav>";

char *html_footer = "<footer><a href='about.html'>Grimgrains</a> © 2014—2020<br><a href='http://100r.co/' target='_blank'>Hundred Rabbits</a></footer></body></html>";

void
build_recipe(Recipe *recipe)
{
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
	fprintf(f, "<div class='col2'>%s</div>", recipe->description);
	for(int i = 0; i < recipe->parts_len; ++i) {
		fputs("<dl class='ingredients'>", f);
		fprintf(f, "<h3>%s</h3>", recipe->parts[i]->name);
		for(int i2 = 0; i2 < recipe->parts[i]->servings_len; ++i2) {
			char ingr_path[STR_BUF_LEN];
			to_lowercase(recipe->parts[i]->servings[i2].ingredient->name, ingr_path, STR_BUF_LEN);
			fprintf(f,
				"<dt><a href='%s.html'><img src='../media/ingredients/%s.png'/><b>%s</b></a><u>%s</u></dt>",
				ingr_path,
				ingr_path,
				recipe->parts[i]->servings[i2].ingredient->name,
				recipe->parts[i]->servings[i2].quantity);
		}
		fputs("</dl>", f);
		fputs("<ul class='instructions'>", f);
		for(int i2 = 0; i2 < recipe->parts[i]->instructions_len; ++i2) {
			fprintf(f, "<li>%s</li>", recipe->parts[i]->instructions[i2]);
		}
		fputs("</ul>", f);
	}
	fputs("</main>", f);

	fputs(html_footer, f);

	fclose(f);
}

void
build_ingredient(Recipe *recipes[], int recipes_len, Ingredient *ingredient)
{
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
	fprintf(f, "<img class='right' src='../media/ingredients/%s.png'/>", filename);
	fprintf(f, "<div>%s</div>", ingredient->description);
	if(ingredient->parent) {
		fprintf(f, "<h2>%s</h2>", ingredient->parent->name);
		fprintf(f, "<div class='small'>%s</div>", ingredient->parent->description);
	}
	/* Related recipes */
	fputs("<ul>", f);
	for(int i = 0; i < recipes_len; ++i) {
		for(int j = 0; j < recipes[i]->parts_len; ++j) {
			for(int k = 0; k < recipes[i]->parts[j]->servings_len; ++k) {
				if(scmp(ingredient->name, recipes[i]->parts[j]->servings[k].ingredient->name)) {
					char recipe_filename[STR_BUF_LEN];
					to_lowercase(recipes[i]->name, recipe_filename, STR_BUF_LEN);
					fprintf(f, "<li><a href='%s.html'>%s</a></li>", recipe_filename, recipes[i]->name);
				}
			}
		}
	}
	fputs("</ul>", f);

	fputs("<hr/>", f);
	fputs("</main>", f);

	fputs(html_footer, f);

	fclose(f);
}

void
build_home(Ingredient *ingredients[], int ingredients_len, int recipes_len)
{
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
	for(int i = 0; i < ingredients_len; ++i) {
		char ingr_path[STR_BUF_LEN];
		to_lowercase(ingredients[i]->name, ingr_path, STR_BUF_LEN);
		fprintf(f,
			"<dt><a href='%s.html'><img src='../media/ingredients/%s.png' loading='lazy'/><b>%s</b></a></dt>",
			ingr_path,
			ingr_path,
			ingredients[i]->name);
	}
	fputs("</dl>", f);

	fprintf(f, "<h2 id='recipes'>%d Recipes</h2>", recipes_len);

	fputs("<ul class='recipes col3'>", f);
	for(int i = 0; i < lifestyle + 1; ++i) {
		fprintf(f, "<h3>%s</h3>", recipe_type_names[i]);
		for(int j = 0; j < recipes_by_types_len[i]; ++j) {
			char recipe_path[STR_BUF_LEN];
			to_lowercase(recipes_by_types[i][j], recipe_path, STR_BUF_LEN);
			fprintf(f, "<li><a href='%s.html'>%s</a></li>", recipe_path, recipes_by_types[i][j]);
		}
	}
	fputs("</ul>", f);

	fputs("</main>", f);

	fputs(html_footer, f);

	fclose(f);
}

void
build_inc(char *name)
{
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
	if(!fp)
		return;

	for(;;) {
		size_t sz = fread(buffer, 1, sizeof(buffer), fp);
		if(sz)
			fwrite(buffer, 1, sz, f);
		else if(feof(fp) || ferror(fp))
			break;
	}
	fclose(fp);
	fputs("</main>", f);

	fputs(html_footer, f);

	fclose(f);
}

void
build_rss(FILE *f, Recipe **recipes, int len)
{
	int i;
	time_t now;
	fputs("<?xml version='1.0' encoding='UTF-8' ?>\n", f);
	fputs("<rss version='2.0' xmlns:dc='http://purl.org/dc/elements/1.1/'>\n", f);
	fputs("<channel>\n", f);
	fputs("<title>" NAME "</title>\n", f);
	fputs("<link>" DOMAIN "</link>\n", f);
	fputs("<description>Grimgrains — a plantbased cooking blog</description>\n", f);
	/* Date */
	fputs("<lastBuildDate>", f);
	fpRFC2822(f, time(&now));
	fputs("</lastBuildDate>\n", f);
	/* Image */
	fputs("<image>\n", f);
	fputs("  <url>" DOMAIN "media/services/rss.jpg</url>\n", f);
	fputs("  <title>Grimgrains — a plantbased cooking blog</title>\n", f);
	fputs("  <link>" DOMAIN "</link>\n", f);
	fputs("</image>\n", f);
	for(i = 0; i < len; ++i) {
		Recipe *r = recipes[i];
		char filename[256];
		scsw(slca(scpy(r->name, filename)), ' ', '_');
		fputs("<item>\n", f);
		fprintf(f, "  <title>%s</title>\n", r->name);
		fprintf(f, "  <link>" DOMAIN "site/%s.html</link>\n", filename);
		fprintf(f, "  <guid isPermaLink='false'>%s</guid>\n", filename);
		fputs("  <pubDate>", f);
		fpRFC2822(f, intdate(r->date));
		fputs("</pubDate>\n", f);
		fputs("  <dc:creator><![CDATA[Rekka Bellum]]></dc:creator>\n", f);
		fputs("  <description>\n", f);
		fputs("<![CDATA[", f);
		fprintf(f, "<img src='" DOMAIN "media/recipes/%s.jpg' width='600'/>\n", filename);
		fprintf(f, "<div>%s</div>", r->description);
		fprintf(f, "<p><a href='" DOMAIN "site/%s.html'>Continue reading</a></p>", filename);
		fputs("]]>\n", f);
		fputs("  </description>\n", f);
		fputs("</item>\n", f);
	}
	fputs("</channel>", f);
	fputs("</rss>", f);
	fclose(f);
}

int
main(void)
{
#include "ingredients.c"
#include "recipes.c"

	int ingredients_len = sizeof ingredients / sizeof ingredients[0];
	int recipes_len = sizeof recipes / sizeof recipes[0];

	printf("Found Ingredients: %d, Recipes: %d\n", ingredients_len, recipes_len);

	for(int i = 0; i < ingredients_len; ++i) {
		build_ingredient(recipes, recipes_len, ingredients[i]);
	}
	printf("Built %d ingredients\n", ingredients_len);

	for(int i = 0; i < recipes_len; ++i) {
		build_recipe(recipes[i]);
	}
	printf("Built %d recipes\n", recipes_len);

	build_home(ingredients, ingredients_len, recipes_len);
	printf("Built home\n");

	build_inc("about");
	build_inc("nutrition");
	build_inc("tools");
	build_inc("meals");
	build_rss(fopen("../links/rss.xml", "w"), recipes, recipes_len);

	return (0);
}
