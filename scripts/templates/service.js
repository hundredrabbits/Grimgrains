'use strict'

function ServiceTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  this.answer = function (q) {
    const recipe_ingredients = find_ingredients(q.tables.recipes)

    const html = `
    ${make_pageless(recipe_ingredients, q.tables.ingredients)}
    ${make_unused(recipe_ingredients, q.tables.ingredients)}
    `
    return {
      title: 'GrimGrains — Service Panel',
      view: {
        core: {
          content: html,
          related: ''
        }
      }
    }
  }

  function find_ingredients (recipes) {
    const h = []
    for (id in recipes) {
      const recipe = recipes[id]
      for (id in recipe.INGR) {
        const category = recipe.INGR[id]
        for (name in category) {
          if (!h.includes(name.toLowerCase())) { h.push(name.toLowerCase()) }
        }
      }
    }
    return h
  }

  function make_pageless (used, pages) {
    const pageless = find_pageless(used, pages)
    let html = ''
    for (id in pageless) {
      html += `
      <li class='ingredient missing'>
        <a href='#${pageless[id].to_url()}' onclick="Ø('query').bang('${pageless[id]}')">
          <img src='media/ingredients/${pageless[id].to_path()}.png'/>
          <span class='name'>${pageless[id].capitalize()}</span>
        </a>
      </li>`
    }
    if (html == '') { return '<h2>No Ingredients Without Pages!</h2>' }
    return `<h2>Ingredients Without Pages</h2><ul class='ingredients'>${html}</ul>`
  }

  function find_pageless (used, pages) {
    const pageless = []

    for (id in used) {
      const name = used[id].toUpperCase()
      if (!pages[name]) { pageless.push(name) }
    }

    return pageless
  }

  function make_unused (used, pages) {
    const unused = find_unused(used, pages)
    let html = ''

    for (id in unused) {
      html += `
      <li class='ingredient'>
        <a href='#${unused[id].to_url()}' onclick="Ø('query').bang('${unused[id]}')">
          <img src='media/ingredients/${unused[id].to_path()}.png'/>
          <span class='name'>${unused[id].capitalize()}</span>
        </a>
      </li>`
    }
    if (html == '') { return '<h2>No Unused Ingredients!</h2>' }
    return `<h2>Unused Ingredients</h2><ul class='ingredients'>${html}</ul>`
  }

  function find_unused (used, pages) {
    const unused = []

    for (name in pages) {
      if (!used.includes(name.toLowerCase())) { unused.push(name) }
    }

    return unused
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

    for (name in recipes) {
      const recipe = recipes[name]
      if (!categorized[recipe.TAGS[0]]) { categorized[recipe.TAGS[0]] = [] }
      recipe.name = name
      categorized[recipe.TAGS[0]].push(recipe)
    }

    for (cat in categorized) {
      const recipes = categorized[cat]
      html += `<h3>${cat.capitalize()}</h3>`
      html += "<ul style='margin-bottom:15px'>"
      for (id in recipes) {
        const recipe = recipes[id]
        html += `<li><a href="#${recipe.name.to_url()}" onclick="Ø('query').bang('${recipe.name.capitalize()}')">${recipe.name.capitalize()}</a></li>`
      }
      html += '</ul>'
    }
    return `<columns id='recipes'>${html}</columns>`
  }
}
