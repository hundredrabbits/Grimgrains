function RecipeTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    return {
      header:{
        search: q.name.capitalize()
      },
      core: {
        content: make_content(q),
        related: make_related(q)
      }
    }
  }

  function make_content(q)
  {
    var recipe = q.result
    var html = "";

    html += `
    <h1>${q.name.capitalize()}</h1>
    <h2>${recipe.SERV} — ${recipe.TIME} minutes</h2>
    <p>${new Runic(recipe.DESC)}</p>
    <h2>Ingredients</h2>
    ${make_ingredients(recipe.INGR)}`;

    return html
  }

  function make_related(q)
  {
    var html = "";
    var recipe = q.result
    var recipes = find_related(recipe,q.tables.recipes)

    for(id in recipes){
      var name = recipes[id][0];
      html += `<ln><a href='#${name.to_url()}'>${name.capitalize()}</a>(${recipes[id][1]})</ln>`
    }
    return html    
  }

  function find_related(target,recipes)
  {
    var a = [];
    for(id in recipes){
      var recipe = recipes[id];
      var index = similarity(target.TAGS,recipe.TAGS)
      a.push([id,index])
    }
    a.sort(function(a, b) {
      return a[1] - b[1];
    });
    return a.reverse();
  }

  function make_ingredients(categories)
  {
    var html = "";
    for(id in categories){
      var elements = categories[id];
      html += categories.length > 1 ? `<h3>${id.capitalize()}</h3>` : ''
      html += `<list class='ingredients'>`
      for(id in elements){
        var element = elements[id];
        html += `
        <ln class='ingredient'>
          <a href='#${id.to_url()}'>
            <img src='media/ingredients/${id.to_path()}.png'/>
          </a>
          <t class='name'>${id.capitalize()}</t>
          <t class='quantity'>${element}</t>
        </ln>`
      }
      html += `<hr /></list>`
    }
    return html
  }

  function similarity(a,b)
  {
    var score = 0
    for(a_id in a){
      var tag_a = a[a_id];
      for(b_id in b){
        var tag_b = b[b_id];
        score += tag_a.toLowerCase() == tag_b.toLowerCase() ? 1 : 0
      }
    }
    return score
  }
}