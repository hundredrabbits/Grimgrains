'use strict'

function HomeTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  this.answer = function (q) {
    const ingredients = find_ingredients(q.tables.recipes)

    ingredients.coffee = 1

    const sorted_ingredients = sort_ingredients(ingredients)

    const html = `
    <h1>Ingredients <a class='jump' id='jump-ingredients' href='javascript:Š("recipes")'>recipes</a></h1>
    ${make_ingredients(sorted_ingredients, q.tables.ingredients)}
    <h1 id='recipes_header'>Recipes <a class='jump' id='jump-recipes' href='javascript:Š("ingredients")'>ingredients</a></h1>
    ${make_recipes(q.tables.recipes)}
    `
    return {
      title: 'GrimGrains — Home',
      view: {
        core: {
          content: html,
          related: ''
        }
      }
    }
  }

  function find_ingredients (recipes) {
    const h = {}
    for (id in recipes) {
      const recipe = recipes[id]
      for (id in recipe.INGR) {
        const category = recipe.INGR[id]
        for (name in category) {
          h[name] = h[name] ? h[name] + 1 : 1
        }
      }
    }
    return h
  }

  function sort_ingredients (ingredients) {
    const a = []
    for (name in ingredients) {
      const value = ingredients[name]
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
      const name = ingredients[id][0]
      html += `
      <li class='ingredient${!table[name] ? ' missing' : ''}'>
        <a href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
          <span class='name'>${name.capitalize()}</span>
        </a>
      </li>`
    }
    return `<ul class='ingredients'>${html}</ul>`
  }

  function count_ingredients (recipe) {
    const ingredients = {}
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

    const categorized = {}

    for (const name in recipes) {
      const recipe = recipes[name]
      if (!categorized[recipe.TAGS[0]]) { categorized[recipe.TAGS[0]] = [] }
      recipe.name = name
      categorized[recipe.TAGS[0]].push(recipe)
    }

    for (const cat in categorized) {
      const recipes = categorized[cat]
      html += `<h3>${cat.capitalize()}</h3>`
      html += "<ul style='margin-bottom:15px'>"
      for (id in recipes) {
        const recipe = recipes[id]
        if (recipe.HIDE) { continue }
        html += `<li><a href="#${recipe.name.to_url()}" onclick="Ø('query').bang('${recipe.name.capitalize()}')">${recipe.name.capitalize()}</a></li>`
      }
      html += '</ul>'
    }
    return `<columns id='recipes'>${html}</columns>`
  }
}
