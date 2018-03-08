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
    <h1>${q.name}</h1>
    <h2>${recipe.DATE}</h2>
    <h3>${recipe.SERV} — ${recipe.TIME} minutes</h3>
    <p>${new Runic(recipe.DESC)}</p>
    <h4>Ingredients</h4>
    <list>${print_sub_list(recipe.INGR)}</list>`;

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