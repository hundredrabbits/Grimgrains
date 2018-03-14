function HomeTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  this.answer = function(q)
  {
    var ingredients = find_ingredients(q.tables.recipes)

    ingredients['coffee'] = 1

    var sorted_ingredients = sort_ingredients(ingredients);  

    return {
      title:`GrimGrains — Home`,
      view:{
        core: {
          content: make_ingredients(sorted_ingredients,q.tables.ingredients),
          related: ""
        }
      }
    }
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

  function sort_ingredients(ingredients)
  {
    var a = [];
    for(name in ingredients){
      var value = ingredients[name];
      a.push([name,value])  
    }
    a.sort(function(a, b) {
      return a[1] - b[1];
    });
    return a;
  }

  function make_ingredients(ingredients,table)
  {
    var html = "";
    for(id in ingredients){
      var name = ingredients[id][0];
      html += `
      <ln class='ingredient ${!table[name] ? "missing" : ""}'>
        <a href='#${name.to_url()}' onclick="Ø('query').bang('${name}')">
          <img src='media/ingredients/${name.to_path()}.png'/>
        </a>
        <t class='name'>${name.capitalize()}</t>
      </ln>`
    }
    return `<list class='ingredients'>${html}<hr/></list>`
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