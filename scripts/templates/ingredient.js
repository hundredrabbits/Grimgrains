function IngredientTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(t)
  {
    var ingredient = t.result;
    var html = "";

    html += `
    <h1>${t.name}</h1>
    <p>${ingredient.DESC}</p>
    <p>${ingredient.TAGS}</p>
    <h4>Related Recipes</h4>
    <list>${print_list(related_recipes(t.name,sort(t.tables.recipes)))}</list>
    <h4>Related Ingredients</h4>
    <list>${print_list(related_ingredients(t.name,ingredient.TAGS[0],sort(t.tables.ingredients)))}</list>`;

    return html
  }

  function print_list(elements)
  {
    var html = "";
    for(id in elements){
      var name = elements[id];
      html += `<ln><a href='#${name.to_url()}'>${name.capitalize()}</a></ln>`
    }
    return html
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

String.prototype.capitalize = function()
{
  return this.charAt(0).toUpperCase() + this.slice(1).toLowerCase();
}

String.prototype.to_url = function()
{
  return this.toLowerCase().replace(/ /g,"+").replace(/[^0-9a-z\+]/gi,"").trim();
}

String.prototype.to_path = function()
{
  return this.toLowerCase().replace(/ /g,".").replace(/[^0-9a-z\.]/gi,"").trim();
}