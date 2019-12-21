# Grimgrains

This repository is the sources for the Grimgrains website. The website is written in C, and requires `gcc` to be built. To rebuild the site, go to `src` and run the build script.

```
./build.sh
```

### Old coffee notes

```
Devine & I are daily coffee drinkers, we sometimes have a second cup in the afternoon, but typically we limit ourselves to a single cup in the morning. We drink it <b>black</b>, but enjoy the occasional flat white as a treat.<br /><br />We grind new beans each morning with a portable manual <a href='http://www.amzn.to/2p0pO2p' target='_blank'>coffee grinder</a>. We mainly use a <a href='http://www.ikea.com/us/en/catalog/products/30149839/' target='_blank'>stainless steel moka pot</a> for espresso.<br /><br />To make a perfect espresso with the moka pot, here are a few things to remember:<br /><br />The grind should be only slightly smaller than filter coffee.<br /><br />Warm up the water in the bottom part before screwing the top section.<br /><br />Take apart and clean the moka pot after each use.<br /><br />To finish, here is a list of the coffee places we've tried (and loved) in our travels:<br /><br /><a href='https://pradocafe.co' target='_blank'>Prado</a> — Vancouver, CA<br /><br /><a href='http://www.cafeneve.com' target='_blank'>Cafe Neve</a> — Montreal, CA<br /><br /><a href='https://sextantcoffee.com' target='_blank'>Sextant Coffee Roasters</a> — San Francisco, CA, US<br /><br /><a href='https://www.facebook.com/BigSurCafeOrganico/' target='_blank'>Big Sur</a> — La Paz, Mexico<br /><br /><a href='https://www.facebook.com/Fat-Camel-Israeli-Cafe-111529865551331/' target='_blank'>Fat Camel</a> — Whangarei, NZ<br /><br />
```

### Macros Helpers

```
#define INGREDIENT(ING_NAME, ING_DESC) Ingredient ING_NAME = create_ingredient(#ING_NAME, ING_DESC);
#define CHILD_INGREDIENT(PARENT_NAME, ING_NAME, ING_DESC) Ingredient ING_NAME = create_child_ingredient(&PARENT_NAME, #ING_NAME, ING_DESC);//Ingredient beans = create_ingredient("beans", "Missing description.");
INGREDIENT(beans, "MissingDescription");
CHILD_INGREDIENT(beans, coffee, "Coffee is life");
```