
Recipe sweet_sour_lentils = create_recipe("sweet and sour lentils","2 servings",20190602,30);
add_recipe_description(&sweet_sour_lentils, "In my galley I have a few recipes that I consider staples, that I'm always in the mood to eat. I usually rotate these throughout the week, adding maybe a new recipe or two to change things up. This sweet and sour lentils recipe is one of these, a favourite of ours.");

RecipePart sweet_sour_lentils_sauce = create_recipe_part(&sweet_sour_lentils,"sauce");
add_part_instruction(&sweet_sour_lentils_sauce,"Mix all liquid ingredients together in a bowl, add {_1 tbsp_} of {{peanut butter}} and stir until dissolved.");
add_part_instruction(&sweet_sour_lentils_sauce,"Stir in {_1 tbsp_} of {{arrowroot starch}} (to help thicken sauce).");
add_part_instruction(&sweet_sour_lentils_sauce,"Add {_2 tsp_} of {{chili pepper flakes}}, {_3_} minced {{garlic cloves}} and roughly {_1 inch_} of minced {{ginger root}}.");
add_part_serving(&sweet_sour_lentils_sauce, &soy_sauce, "2 tbsp");
add_part_serving(&sweet_sour_lentils_sauce, &japanese_rice_vinegar, "2 tbsp");
add_part_serving(&sweet_sour_lentils_sauce, &chili_pepper_flakes, "2 tsp");
add_part_serving(&sweet_sour_lentils_sauce, &peanut_butter, "1 tbsp");
add_part_serving(&sweet_sour_lentils_sauce, &maple_syrup, "2 tbsp");
add_part_serving(&sweet_sour_lentils_sauce, &sesame_oil, "1 tsp");
add_part_serving(&sweet_sour_lentils_sauce, &garlic, "3 cloves, minced");
add_part_serving(&sweet_sour_lentils_sauce, &ginger_root, "1 root minced");
add_part_serving(&sweet_sour_lentils_sauce, &arrowroot_starch, "1 tbsp");

// RecipePart sweet_sour_lentils_main = create_recipe_part(&sweet_sour_lentils,"main");
// add_part_instruction(&sweet_sour_lentils_main, "Rinse lentils. Transfer rinsed lentils to a pot and add {_1 1/2 cups_} of {{vegetable bouillon}}.");
// add_part_instruction(&sweet_sour_lentils_main, "Bring water to a rapid simmer, then reduce heat to medium.");
// add_part_instruction(&sweet_sour_lentils_main, "Simmer uncovered. After {#10 minutes#} add {_1_} cubed {{carrot}} and {_2 inches_} of cubed {{daikon}}.");
// add_part_instruction(&sweet_sour_lentils_main, "Let mixture simmer for an additional {#10-20 minutes#}, add extra water as needed.");
// add_part_instruction(&sweet_sour_lentils_main, "If lentils are tender, they are ready. Strain lentils and return to pot with {_1/4 tsp_} of {{salt}}.");
// add_part_instruction(&sweet_sour_lentils_main, "Pour sauce onto lentils, mix well. Serve into two bowls with some {{chives}}. Eat wrapped in salad or cabbage leaves, or with {{crackers}}.");
// add_part_serving(&sweet_sour_lentils_main, &brown_lentils, "1/2 cup");
// add_part_serving(&sweet_sour_lentils_main, &vegetable_bouillon, "1 1/2 cups");
// add_part_serving(&sweet_sour_lentils_main, &carrots, "1, cubed");
// add_part_serving(&sweet_sour_lentils_main, &daikon, "2 cubed");
// add_part_serving(&sweet_sour_lentils_main, &chives, "3 stalks");
// add_part_serving(&sweet_sour_lentils_main, &salt, "1/4 tsp");

/*

SWEET AND SOUR LENTILS
  DATE : 2019-06-02
  TAGS
    dinner
  TIME : 30
  SERV : 2 servings
  DESC
    & In my galley I have a few recipes that I consider staples, that I'm always in the mood to eat. I usually rotate these throughout the week, adding maybe a new recipe or two to change things up. This sweet and sour lentils recipe is one of these, a favourite of ours.
    & You can eat the lentils as is, or scoop it up with crackers. Devine & I enjoy eating it wrapped in salad or cabbage leaves.
    & We've cooked sweet and sour lentils often on long passages, it's a simple one-pot meal. It's also my go-to recipe during pot lucks - even people who don't like lentils will enjoy it (I've never had any complaints, not yet).
    & {*Substitutions*}
    & It's a versatile recipe, so if I don't have any carrots or daikon I'll sometimes put brocoli or fresh green peas instead. For a heartier meal, adding sweet potatoes is also delicious. Sometimes, if I'm out of peanut butter I'll use tahini instead, the flavour is very similar and doesn't change much in the recipe (also nice for those with peanut allergies). This recipe only works with whole lentils, because there's still a bite to them, halved lentils will soften too much and the resulting texture won't be as pleasant.
    & Enjoy this humble, but delicious recipe.
    % recipes/sweet.and.sour.lentils.2.jpg
  INST
    SAUCE
      - Mix all liquid ingredients together in a bowl, add {_1 tbsp_} of {{peanut butter}} and stir until dissolved.
      - Stir in {_1 tbsp_} of {{arrowroot starch}} (to help thicken sauce).
      - Add {_2 tsp_} of {{chili pepper flakes}}, {_3_} minced {{garlic cloves}} and roughly {_1 inch_} of minced {{ginger root}}.
    LENTILS
      - Rinse lentils. Transfer rinsed lentils to a pot and add {_1 1/2 cups_} of {{vegetable bouillon}}.
      - Bring water to a rapid simmer, then reduce heat to medium.
      - Simmer uncovered. After {#10 minutes#} add {_1_} cubed {{carrot}} and {_2 inches_} of cubed {{daikon}}.
      - Let mixture simmer for an additional {#10-20 minutes#}, add extra water as needed.
      - If lentils are tender, they are ready. Strain lentils and return to pot with {_1/4 tsp_} of {{salt}}.
      - Pour sauce onto lentils, mix well. Serve into two bowls with some {{chives}}. Eat wrapped in salad or cabbage leaves, or with {{crackers}}.
  INGR
    Main
      Brown lentils : 1/2 cup
      Vegetable bouillon : 1 1/2 cups
      Carrots : 1, cubed
      Daikon : 2", cubed
      Chives : 3 stalks
      Salt : 1/4 tsp
    Sauce
      Soy sauce : 2 tbsp
      Japanese rice vinegar : 2 tbsp
      Chili pepper flakes : 2 tsp
      Peanut butter : 1 tbsp
      Maple syrup : 2 tbsp
      Sesame oil : 1 tsp
      Garlic : 3 cloves, minced
      Ginger root : 1", minced
      Arrowroot starch : 1 tbsp

*/

// Recipe *recipes[] = {&rice_treat};