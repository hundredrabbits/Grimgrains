
/*
FILE *myfile = fopen("../site/somefile.txt", "w");

fprintf(myfile, "hello!\n");

fclose(myfile);

*/

int ingredients_len = sizeof ingredients / sizeof ingredients[0];
int recipes_len = sizeof recipes / sizeof recipes[0];

printf("Ingredients: %d Recipes: %d\n",ingredients_len,recipes_len);