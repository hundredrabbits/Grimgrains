# Grimgrains

This repository is the sources for the Grimgrains website. The website is written in C, and requires `gcc` to build. To rebuild the site, go to `src` and run the build script:

```
./build.sh
```

### Macros Helpers

```
#define INGREDIENT(ING_NAME, ING_DESC) Ingredient ING_NAME = create_ingredient(#ING_NAME, ING_DESC);
#define CHILD_INGREDIENT(PARENT_NAME, ING_NAME, ING_DESC) Ingredient ING_NAME = create_child_ingredient(&PARENT_NAME, #ING_NAME, ING_DESC);//Ingredient beans = create_ingredient("beans", "Missing description.");
INGREDIENT(beans, "MissingDescription");
CHILD_INGREDIENT(beans, coffee, "Coffee is life");
```
# Extras

- The website source code of GrimGrains is licensed under [MIT](https://github.com/hundredrabbits/grimgrains/blob/master/LICENSE) and the **images, text and assets** are licensed under [BY-NC-SA 4.0](https://github.com/hundredrabbits/grimgrains/blob/master/LICENSE.by-nc-sa-4.0.md). View individual licenses for details.
- Support this project through [Patreon](https://patreon.com/100).
