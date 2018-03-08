function PageTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    var recipe = t.result;

    return {
      header:{
        search: t.name.capitalize()
      },
      core: {
        content: recipe,
        related:{
          related_recipes:related_recipes(t.name,t.tables.recipes),
          related_ingredients:related_ingredients(t.name,ingredient.TAGS[0],sort(t.tables.ingredients))
        }
      }
    }
    console.log(q.result)

    var recipe = q.result
    var html = "";

    html += `
    <h1>${q.name}</h1>
    <h2>${recipe.date}</h2>
    <h3>${recipe.serv} — ${recipe.time} minutes</h3>
    <p>${recipe.desc}</p>
    <h4>Ingredients</h4>
    <list>${list(recipe.ingr)}</list>`;

    return html
  }

  function list(items)
  {
    var html = ""
    for(id in items){
      html += `<ln>${id} -> ${items[id]}</ln>`
      // html += list(items[id])
    }
    return html;
  }
}