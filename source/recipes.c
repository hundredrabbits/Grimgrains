
Recipe rice_treat = create_recipe("sesame puffed rice treat","12 servings",20161207,60);

add_recipe_description(&rice_treat, "<p>A simple and fun dessert, using two of my favorite ingredients, {{kinako}} and {{black sesame seeds}}. Both of these ingredients lend themselves well to sweets, with a complex nutty taste.</p><p>Kinako is roasted soybean flour. It is often used to coat confectionaries in Japan. It's a healthy topping and flavouring which contains B vitamins and protein.</p>");

add_recipe_instruction(&rice_treat, "Bring a pan to medium heat, and add {_1/3 cup_} of {{black sesame seeds}}. Toast until fragrant, you'll know they're ready because they'll start popping and crackling. Remove from heat, and let cool. ");
add_recipe_instruction(&rice_treat, "Grind the toasted sesame seeds with a mortar and pestle, or your pulverizer of choice.");
add_recipe_instruction(&rice_treat, "Heat a {_1/4 cup_} of {{coconut oil}} in a pot at medium heat until fully melted, then stir in {_1/2 cup_} of {{brown rice syrup}} and the ground sesame seeds. Cook for {#5 minutes#}, stirring all the while.");
add_recipe_instruction(&rice_treat, "Remove from heat, stir in {_1/4 tsp_} of {{salt}}, then add {_4 cups_} of {{puffed rice}} cereal and mix well. Transfer to a greased square baking dish (8x8 pyrex dish is fine) and pack it down using a spatula. You can also overlay a sheet of parchment paper and to push down with your hands. ");
add_recipe_instruction(&rice_treat, "Refrigerate for {#1 hour#}.");
add_recipe_instruction(&rice_treat, "Sprinkle {_1/4 cup_} of {{kinako flour}} over the rice crispy treats and cut into squares.");

add_recipe_serving(&rice_treat, &black_sesame_seeds, "1/3 cup");
add_recipe_serving(&rice_treat, &coconut_oil, "1/4 cup");
add_recipe_serving(&rice_treat, &brown_rice_syrup, "1/2 cup");
add_recipe_serving(&rice_treat, &puffed_rice, ": 4 cups");
add_recipe_serving(&rice_treat, &kinako, "1/4 cup");
add_recipe_serving(&rice_treat, &salt, "1/4 tsp");


// SESAME PUFFED RICE TREAT
//   DATE : 2016-12-07
//   TAGS
//     dessert
//   TIME : 60
//   SERV : 12 servings
//   DESC
//     & A simple and fun dessert, using two of my favorite ingredients, {{kinako}} and {{black sesame seeds}}. Both of these ingredients lend themselves well to sweets, with a complex nutty taste.
//     & Kinako is roasted soybean flour. It is often used to coat confectionaries in Japan. It's a healthy topping and flavouring which contains B vitamins and protein.
//   INST
//     Preparation 
//       - Bring a pan to medium heat, and add {_1/3 cup_} of {{black sesame seeds}}. Toast until fragrant, you'll know they're ready because they'll start popping and crackling. Remove from heat, and let cool. 
//       - Grind the toasted sesame seeds with a mortar and pestle, or your pulverizer of choice.
//       - Heat a {_1/4 cup_} of {{coconut oil}} in a pot at medium heat until fully melted, then stir in {_1/2 cup_} of {{brown rice syrup}} and the ground sesame seeds. Cook for {#5 minutes#}, stirring all the while.
//       - Remove from heat, stir in {_1/4 tsp_} of {{salt}}, then add {_4 cups_} of {{puffed rice}} cereal and mix well. Transfer to a greased square baking dish (8x8 pyrex dish is fine) and pack it down using a spatula. You can also overlay a sheet of parchment paper and to push down with your hands. 
//       - Refrigerate for {#1 hour#}.
//       - Sprinkle {_1/4 cup_} of {{kinako flour}} over the rice crispy treats and cut into squares.
//   INGR
//     Main
//       Black sesame seeds : 1/3 cup
//       Coconut oil : 1/4 cup
//       Brown Rice Syrup : 1/2 cup
//       Puffed Rice : 4 cups
//       Kinako : 1/4 cup
//       Salt : 1/4 tsp



// add_serving(&cake,&pepper,"1tbs");

// Recipe *recipes[] = {&rice_treat};