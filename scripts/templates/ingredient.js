function IngredientTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(t)
  {
    var ingredient = t.result;

    return {
      title:`GrimGrains — ${t.name.capitalize()}`,
      view: {
        core: {
          content: make_ingredient(t.name,ingredient,t.tables.recipes),
          related:make_related(related_recipes(t.name,t.tables.recipes))
        }
      }
    }
  }

  function make_ingredient(name,ingredient,recipes)
  {
    console.log(ingredient)
    var html = ""

    html += `<h1>${ingredient.TYPE ? ingredient.TYPE.capitalize()+"/" : ''}${name.capitalize()}</h1>`
    html += ingredient.BREF ? `<p class='bref'>${ingredient.BREF.to_markup()}</p>` : ''
    html += ingredient.LONG ? `${new Runic(ingredient.LONG)}` : ''
    html += `${make_similar(name,recipes)}`
    return html
  }

  function make_similar(name,recipes)
  {
    var html = ""
    var ingredients = find_ingredients(recipes);
    var similar_ingredients = find_similar_ingredients(name,ingredients)

    for(id in similar_ingredients){
      if(similar_ingredients[id][1] < 1){ break;}
      var name = similar_ingredients[id][0];
      html += `
      <ln class='ingredient'>
        <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
        </a>
        <t class='name'>${name.capitalize()}</t>
      </ln>`
    }
    return similar_ingredients.length > 1 ? `<h2>Related Ingredients</h2><list class='ingredients'>${html}<hr /></list>` : ''
  }

  function find_ingredients(recipes)
  {
    var h = {}
    for(id in recipes){
      var recipe = recipes[id];
      for(id in recipe.INGR){
        var category = recipe.INGR[id];
        for(name in category){
          h[name] = h[name] ? h[name]+1 : 1
        }
      }
    }
    return h
  }

  function find_similar_ingredients(name,ingredients)
  {
    var a = [];

    for(id in ingredients){
      var words = id.split(" ")
      var index = similarity(name.split(" "),words);
      if(index > 0){
        a.push([id,index])  
      }
    }

    a.sort(function(a, b) {
      return a[1] - b[1];
    });
    
    return a.reverse();
  }

  function similarity(a,b)
  {
    var score = 0;
    for(a_id in a){
      var word_a = a[a_id];
      for(b_id in b){
        var word_b = b[b_id];
        score += word_a == word_b ? 1 : 0
      }
    }
    return score
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
