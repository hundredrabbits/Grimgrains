'use strict'

function IngredientTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function (t) {
    const ingredient = t.result

    return {
      title: `GrimGrains — ${t.name.capitalize()}`,
      view: {
        core: {
          content: make_ingredient(t.name, ingredient, t.tables.recipes, t.tables.ingredients),
          related: make_related(related_recipes(t.name, t.tables.recipes))
        }
      }
    }
  }

  function make_ingredient (name, ingredient, recipes, all_ingredients) {
    let html = ''

    html += `<h1>${name.capitalize()}</h1>`
    html += ingredient.BREF ? `<p class='bref'>${ingredient.BREF.to_markup()}</p>` : ''
    html += ingredient.LONG ? `${new Runic(ingredient.LONG)}` : ''
    html += ingredient.WARN ? `<section id='warn'>${new Runic(ingredient.WARN)}</section>` : ''
    html += `${make_parents(ingredient)}`
    html += `${make_children(name, all_ingredients)}`
    html += `${make_similar(name, recipes, all_ingredients)}`
    return html
  }

  function make_parents (ingredient) {
    let html = ''
    if (!ingredient.PARENT) { return html }

    html += "<h2>Parent Ingredients</h2><ul class='ingredients'>"
    const parents = ingredient.PARENT.split(',')

    for (id in parents) {
      const name = parents[id].trim()
      html += `
      <li class='ingredient'>
        <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
          <span class='name'>${name.capitalize()}</span>
        </a>
      </li>`
    }

    html += '</ul>'
    return html
  }

  function make_children (ingredient, all_ingredients) {
    let html = ''
    const child_ingredients = find_child_ingredients(ingredient, all_ingredients)
    if (child_ingredients.length == 0) { return html }

    html += "<h2>Child Ingredients</h2><ul class='ingredients'>"

    for (id in child_ingredients) {
      const name = child_ingredients[id]
      html += `
      <li class='ingredient'>
        <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
          <span class='name'>${name.capitalize()}</span>
        </a>
      </li>`
    }

    html += '</ul>'
    return html
  }

  function make_similar (search_name, recipes, all_ingredients) {
    let html = ''
    const ingredients = find_ingredients(recipes)
    const similar_ingredients = find_similar_ingredients(search_name, ingredients, all_ingredients)

    for (id in similar_ingredients) {
      if (similar_ingredients[id][1] < 1) { break }
      const name = similar_ingredients[id][0]
      if (name.toLowerCase() == search_name.toLowerCase()) { continue }
      html += `
      <li class='ingredient'>
        <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
          <span class='name'>${name.capitalize()}</span>
        </a>
      </li>`
    }
    return similar_ingredients.length >= 1 ? `<h2>Related Ingredients</h2><ul class='ingredients'>${html}<hr /></ul>` : ''
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

  function find_similar_ingredients (name, ingredients, all_ingredients) {
    const a = []

    const children = find_child_ingredients(name, all_ingredients)

    for (id in ingredients) {
      if (children.includes(id.toLowerCase())) { continue }
      const words = id.toLowerCase().split(' ')
      const index = similarity(name.toLowerCase().split(' '), words)
      if (index > 0) {
        a.push([id, index])
      }
    }

    a.sort(function (a, b) {
      return a[1] - b[1]
    })

    return a.reverse()
  }

  function find_child_ingredients (search_name, all_ingredients) {
    const a = []

    for (name in all_ingredients) {
      const ingr = all_ingredients[name]
      if (!ingr.PARENT) { continue }
      const parents = ingr.PARENT.split(',').map(function (name) { return name.trim().toLowerCase() })
      if (parents.includes(search_name.toLowerCase())) {
        a.push(name.toLowerCase())
      }
    }

    return a
  }

  function similarity (a, b) {
    let score = 0
    for (const a_id in a) {
      const word_a = a[a_id]
      for (const b_id in b) {
        const word_b = b[b_id]
        score += word_a == word_b ? 1 : 0
      }
    }
    return score
  }

  function make_related (recipes) {
    let html = ''

    let count = 0
    for (id in recipes) {
      const recipe = recipes[id]
      const name = id
      html += `
      <li class='recipe'>
        <a onclick="Ø('query').bang('${name}')" class='photo' href='#${name.to_url()}' style='background-image:url(media/recipes/${name.to_path()}.jpg)'></a>
        <span class='name'>${name.capitalize()}</span>
        <span class='details'><b>${recipe.TIME} minutes</b><br />${count_ingredients(recipe)} ingredients<br />${recipe.INST.length} steps</span>
      </li>`
      if (count > 1) { break }
      count += 1
    }
    return `${html}<hr/>`
  }

  function sort (o) {
    return Object.keys(o).sort().reduce((r, k) => (r[k] = o[k], r), {})
  }

  function related_recipes (name, recipes) {
    const h = {}
    for (id in recipes) {
      const recipe = recipes[id]
      for (const i in recipe.INGR) {
        const ingredients = recipe.INGR[i]
        for (const n in ingredients) {
          if (n.indexOf(name.toUpperCase()) < 0) { continue }
          h[id] = recipes[id]
        }
      }
    }
    return h
  }

  function related_ingredients (name, tag, ingredients) {
    const a = []
    for (const id in ingredients) {
      const ingredient = ingredients[id]
      if (!ingredient.TAGS || ingredient.TAGS.indexOf(tag) < 0 || id == name) { continue }
      a.push(id)
    }
    return a
  }

  function count_ingredients (recipe) {
    const ingredients = {}
    for (const cat in recipe.INGR) {
      for (const id in recipe.INGR[cat]) {
        ingredients[id] = 1
      }
    }
    return Object.keys(ingredients).length
  }
}
