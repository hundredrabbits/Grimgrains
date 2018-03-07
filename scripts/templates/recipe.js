function RecipeTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    var recipe = q.result
    var html = "";

    html += `
    <h1>${q.name}</h1>
    <h2>${recipe.DATE}</h2>
    <h3>${recipe.SERV} — ${recipe.TIME} minutes</h3>
    <p>${recipe.DESC}</p>
    <h4>Ingredients</h4>
    <list>${print_sub_list(recipe.INGR)}</list>`;

    return html
  }

  function print_sub_list(categories)
  {
    var html = "";
    for(id in categories){
      var elements = categories[id];
      html += `<ln class='category'>${id}</ln>`
      for(id in elements){
        var element = elements[id];
        html += `<ln><a href='#${id.to_url()}'>${id.capitalize()}</a></ln>`
      }
    }
    return html
  }
}