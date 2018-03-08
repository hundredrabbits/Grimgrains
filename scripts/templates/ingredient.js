function IngredientTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(t)
  {
    var ingredient = t.result;

    return {
      header:{
        search: t.name.capitalize()
      },
      core: {
        content: ingredient.DESC,
        related:{
          related_recipes:related_recipes(t.name,t.tables.recipes),
          related_ingredients:related_ingredients(t.name,ingredient.TAGS[0],sort(t.tables.ingredients))
        }
      }
    }
  }

  function sort(o)
  {
    return Object.keys(o).sort().reduce((r, k) => (r[k] = o[k], r), {});
  }

  function related_recipes(name,recipes)
  {
    var a = []
    for(id in recipes){
      var recipe = recipes[id]
      for(i in recipe.INGR){
        var ingredients = recipe.INGR[i];
        for(n in ingredients){
          if(n.indexOf(name) < 0){ continue; }
          a.push(id)
        }
      }
    }
    return a;
  }

  function related_ingredients(name,tag,ingredients)
  {
    var a = []
    for(id in ingredients){
      var ingredient = ingredients[id]
      if(!ingredient.TAGS || ingredient.TAGS.indexOf(tag) < 0 || id == name){ continue; }
      a.push(id)
    }
    return a;
  }
}
