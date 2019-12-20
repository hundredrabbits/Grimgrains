
Recipe sweet_sour_lentils = create_recipe("sweet and sour lentils","dinner","2 servings",20190602,30);
set_description(&sweet_sour_lentils, "In my galley I have a few recipes that I consider staples, that I'm always in the mood to eat. I usually rotate these throughout the week, adding maybe a new recipe or two to change things up. This sweet and sour lentils recipe is one of these, a favourite of ours.");
RecipePart sweet_sour_lentils_sauce = create_part("sauce");
add_instruction(&sweet_sour_lentils_sauce, "Mix all liquid ingredients together in a bowl, add {_1 tbsp_} of {{peanut butter}} and stir until dissolved.");
add_instruction(&sweet_sour_lentils_sauce, "Stir in {_1 tbsp_} of {{arrowroot starch}} (to help thicken sauce).");
add_instruction(&sweet_sour_lentils_sauce, "Add {_2 tsp_} of {{chili pepper flakes}}, {_3_} minced {{garlic cloves}} and roughly {_1 inch_} of minced {{ginger root}}.");
add_serving(&sweet_sour_lentils_sauce, &soy_sauce, "2 tbsp");
add_serving(&sweet_sour_lentils_sauce, &japanese_rice_vinegar, "2 tbsp");
add_serving(&sweet_sour_lentils_sauce, &chili_pepper_flakes, "2 tsp");
add_serving(&sweet_sour_lentils_sauce, &peanut_butter, "1 tbsp");
add_serving(&sweet_sour_lentils_sauce, &maple_syrup, "2 tbsp");
add_serving(&sweet_sour_lentils_sauce, &sesame_oil, "1 tsp");
add_serving(&sweet_sour_lentils_sauce, &garlic, "3 cloves, minced");
add_serving(&sweet_sour_lentils_sauce, &ginger_root, "1 root minced");
add_serving(&sweet_sour_lentils_sauce, &arrowroot_starch, "1 tbsp");
add_part(&sweet_sour_lentils, &sweet_sour_lentils_sauce);
RecipePart sweet_sour_lentils_main = create_part("main");
add_instruction(&sweet_sour_lentils_main, "Rinse lentils. Transfer rinsed lentils to a pot and add {_1 1/2 cups_} of {{vegetable bouillon}}.");
add_instruction(&sweet_sour_lentils_main, "Bring water to a rapid simmer, then reduce heat to medium.");
add_instruction(&sweet_sour_lentils_main, "Simmer uncovered. After {#10 minutes#} add {_1_} cubed {{carrot}} and {_2 inches_} of cubed {{daikon}}.");
add_instruction(&sweet_sour_lentils_main, "Let mixture simmer for an additional {#10-20 minutes#}, add extra water as needed.");
add_instruction(&sweet_sour_lentils_main, "If lentils are tender, they are ready. Strain lentils and return to pot with {_1/4 tsp_} of {{salt}}.");
add_instruction(&sweet_sour_lentils_main, "Pour sauce onto lentils, mix well. Serve into two bowls with some {{chives}}. Eat wrapped in salad or cabbage leaves, or with {{crackers}}.");
add_serving(&sweet_sour_lentils_main, &brown_lentils, "1/2 cup");
add_serving(&sweet_sour_lentils_main, &vegetable_bouillon, "1 1/2 cups");
add_serving(&sweet_sour_lentils_main, &carrots, "1, cubed");
add_serving(&sweet_sour_lentils_main, &daikon, "2 cubed");
add_serving(&sweet_sour_lentils_main, &chives, "3 stalks");
add_serving(&sweet_sour_lentils_main, &salt, "1/4 tsp");
add_part(&sweet_sour_lentils, &sweet_sour_lentils_main);




Recipe *recipes[] = {&sweet_sour_lentils};