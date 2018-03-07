function IngredientTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(t)
  {
    var ingredient = t.result;
    var html = "";

    console.log(t)

    html += `
    <h1>${t.name}</h1>
    <p>${ingredient.DESC}</p>
    <p>${ingredient.TAGS}</p>
    <h4>Related</h4>
    <list>${related(t.name,ingredient.TAGS[0],sort(t.tables.ingredients))}</list>`;

    return html
  }

  function sort(o)
  {
    return Object.keys(o).sort().reduce((r, k) => (r[k] = o[k], r), {});
  }

  function related(name,tag,ingredients)
  {
    var html = ""
    for(id in ingredients){
      var ingredient = ingredients[id]
      if(!ingredient.TAGS || ingredient.TAGS.indexOf(tag) < 0 || id == name){ continue; }
      html += `<ln>${id.capitalize()}</ln>`
    }
    return html;
  }
}

String.prototype.capitalize = function()
{
  return this.charAt(0).toUpperCase() + this.slice(1).toLowerCase();
}
