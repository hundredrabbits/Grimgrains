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
    html += `<ul class='ingredients' style='float:right'>${print_ingredient(name)}</ul>`
    html += ingredient.BREF ? `<p class='bref'>${ingredient.BREF.to_markup()}</p>` : ''
    html += ingredient.LONG ? `${runic.run(ingredient.LONG)}` : ''

    if (ingredient.PARENT) {
      const parent_ingr = all_ingredients[ingredient.PARENT.toUpperCase()]
      if (parent_ingr && parent_ingr.BREF) {
        html += `<p>${parent_ingr.BREF.to_markup()}</p>`
      }
      if (parent_ingr && parent_ingr.LONG) {
        html += `${runic.run(parent_ingr.LONG)}`
      }
    }

    html += ingredient.WARN ? `<section id='warn'>${runic.run(ingredient.WARN)}</section>` : ''

    const parents = ingredient && ingredient.PARENT ? ingredient.PARENT.split(',') : []
    const children = find_child_ingredients(name, all_ingredients)
    const related = parents.concat(children)
    html += related.length > 0 ? `<h2>Related Ingredients</h2><ul class='ingredients'>${related.reduce((acc, ingr) => { return acc + print_ingredient(ingr) }, '')}</ul>` : ''

    return html
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
        <a href='#${name.to_url()}'>
          <div class='photo' style='background-image:url(media/recipes/${name.to_path()}.jpg)'></div>
          <span class='name'>${name.capitalize()}</span>
        </a>
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

  function print_ingredient (name) {
    return `
      <li class='ingredient'>
        <a href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
          <span class='name'>${name.capitalize()}</span>
        </a>
      </li>`
  }
}
