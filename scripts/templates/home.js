function HomeTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  this.answer = function (q) {
    let ingredients = find_ingredients(q.tables.recipes)

    ingredients['coffee'] = 1

    let sorted_ingredients = sort_ingredients(ingredients)

    let html = `
    ${make_ingredients(sorted_ingredients, q.tables.ingredients)}
    <h1>Recipes</h1>
    ${make_recipes(q.tables.recipes)}
    `
    return {
      title: `GrimGrains — Home`,
      view: {
        core: {
          content: html,
          related: ''
        }
      }
    }
  }

  function find_ingredients (recipes) {
    let h = {}
    for (id in recipes) {
      let recipe = recipes[id]
      for (id in recipe.INGR) {
        let category = recipe.INGR[id]
        for (name in category) {
          h[name] = h[name] ? h[name] + 1 : 1
        }
      }
    }
    return h
  }

  function sort_ingredients (ingredients) {
    let a = []
    for (name in ingredients) {
      let value = ingredients[name]
      a.push([name, value])
    }
    a.sort(function (a, b) {
      return a[1] - b[1]
    })
    return a
  }

  function make_ingredients (ingredients, table) {
    let html = ''
    for (id in ingredients) {
      let name = ingredients[id][0]
      html += `
      <li class='ingredient${!table[name] ? ' missing' : ''}'>
        <a href='#${name.to_url()}' onclick="Ø('query').bang('${name}')">
          <img src='media/ingredients/${name.to_path()}.png'/>
        </a>
        <t class='name'>${name.capitalize()}</t>
      </li>`
    }
    return `<ul class='ingredients'>${html}</ul>`
  }

  function count_ingredients (recipe) {
    let ingredients = {}
    for (cat in recipe.INGR) {
      for (id in recipe.INGR[cat]) {
        ingredients[id] = 1
      }
    }
    return Object.keys(ingredients).length
  }

  function make_recipes (recipes) {
    let html = ''

    // Sort by tag

    let categorized = {}

    for (name in recipes) {
      let recipe = recipes[name]
      if (!categorized[recipe.TAGS[0]]) { categorized[recipe.TAGS[0]] = [] }
      recipe.name = name
      categorized[recipe.TAGS[0]].push(recipe)
    }

    for (cat in categorized) {
      let recipes = categorized[cat]
      html += `<h3>${cat.capitalize()}</h3>`
      html += "<ul style='margin-bottom:15px'>"
      for (id in recipes) {
        let recipe = recipes[id]
        html += `<li><a href="#${recipe.name.to_url()}" onclick="Ø('query').bang('${recipe.name.capitalize()}')">${recipe.name.capitalize()}</a></li>`
      }
      html += '</ul>'
    }
    return `<columns id='recipes'>${html}</columns>`
  }
}
