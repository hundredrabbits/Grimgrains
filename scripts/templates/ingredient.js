function IngredientTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(t)
  {
    var ingredient = t.result;

    return {
      core: {
        content: make_ingredient(t.name,ingredient),
        related:make_related(related_recipes(t.name,t.tables.recipes))
      }
    }
  }

  function make_ingredient(name,ingredient)
  {
    var html = ""

    html += `<h1>${name.capitalize()}</h1>`
    return html
  }

  function make_related(recipes)
  {
    var html = ""

    var count = 0;
    for(id in recipes){
      var recipe = recipes[id]
      var name = id;
      html += `
      <ln class='recipe'>
        <a onclick="Ø('query').bang('${name}')" class='photo' href='#${name}' style='background-image:url(media/recipes/${name.to_path()}.jpg)'></a>
        <t class='name'>${name.capitalize()}</t>
        <t class='details'><b>${recipe.TIME} minutes</b><br />${count_ingredients(recipe)} ingredients<br />${recipe.INST.length} steps</t>
      </ln>`
      if(count > 1){ break; }
      count += 1
    }
    return `${html}<hr/>`    
  }

  function sort(o)
  {
    return Object.keys(o).sort().reduce((r, k) => (r[k] = o[k], r), {});
  }

  function related_recipes(name,recipes)
  {
    var h = {}
    for(id in recipes){
      var recipe = recipes[id]
      for(i in recipe.INGR){
        var ingredients = recipe.INGR[i];
        for(n in ingredients){
          if(n.indexOf(name) < 0){ continue; }
          h[id] = recipes[id]
        }
      }
    }
    return h;
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

  function count_ingredients(recipe)
  {
    var ingredients = {}
    for(cat in recipe.INGR){
      for(id in recipe.INGR[cat]){
        ingredients[id] = 1
      }
    }
    return Object.keys(ingredients).length
  }
}
