function IndexTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    return {
      title:`GrimGrains — Index`,
      view:{
        core: {
          content: make_list(q.tables.recipes)
        }
      }
    }
  }

  function make_list(recipes)
  {
    var html = ""

    // Sort by tag

    var categorized = {}

    for(name in recipes){
      var recipe = recipes[name]
      if(!categorized[recipe.TAGS[0]]){ categorized[recipe.TAGS[0]] = []; }
      recipe.name = name
      categorized[recipe.TAGS[0]].push(recipe)
    }

    for(cat in categorized){
      var recipes = categorized[cat]
      html += `<h3>${cat.capitalize()}</h3>`
      html += "<list style='margin-bottom:15px'>"
      for(id in recipes){
        var recipe = recipes[id]
        html += `<ln>${recipe.name.capitalize()}</ln>`

      }
      html += "</list>"
    }
    return `<columns id='index'>${html}</columns>`;
  }
}