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
    <img class='photo' src='media/recipes/${q.name.to_path()}.jpg'/>
    <h1>${q.name.capitalize()}</h1>
    <h2>${recipe.SERV} — ${recipe.TIME} minutes</h2>

    <columns>${new Runic(recipe.DESC)}</columns>
    <h2>Ingredients</h2>
    ${make_ingredients(recipe.INGR)}`;

    return html
  }

  function make_related(q)
  {
    var html = "";
    var recipe = q.result
    var recipes = find_related(q.name,recipe,q.tables.recipes)

    var count = 0;
    for(id in recipes){
      var name = recipes[id][0];
      html += `
      <ln class='recipe'>
        <a class='photo' onclick="Ø('query').bang('${name}')" href='#${name.to_url()}' style='background-image:url(media/recipes/${name.to_path()}.jpg)'></a>
        <t class='name'>${name.capitalize()}</t>
        <t class='details'><b>${recipe.TIME} minutes</b><br />${count_ingredients(recipe)} ingredients<br />${recipe.INST.length} steps</t>
      </ln>`
      if(count > 1){ break; }
      count += 1
    }
    return `${html}<hr/>`    
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

  function find_related(name,target,recipes)
  {
    var a = [];
    for(id in recipes){
      var recipe = recipes[id];
      var index = similarity(target.TAGS,recipe.TAGS)
      if(id != name){
        a.push([id,index])  
      }
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
      
      html += `<list class='ingredients'>`
      html += Object.keys(categories).length > 1 ? `<h3>${id.capitalize()}</h3>` : ""
      for(name in elements){
        var element = elements[name];
        html += `
        <ln class='ingredient'>
          <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
            <img src='media/ingredients/${name.to_path()}.png'/>
          </a>
          <t class='name'>${name.capitalize()}</t>
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