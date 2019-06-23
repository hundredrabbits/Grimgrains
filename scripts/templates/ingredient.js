function IngredientTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function (t) {
    let ingredient = t.result

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
    html += `${make_parents(ingredient)}`
    html += `${make_children(name, all_ingredients)}`
    html += `${make_similar(name, recipes, all_ingredients)}`
    return html
  }
  
  function make_parents (ingredient) {
    let html = ''
    if (!ingredient.PARENT) {return html}

    html += "<h2>Parent Ingredients</h2><ul class='ingredients'>"
    let parents = ingredient.PARENT.split(",")

    for (id in parents) {
      let name = parents[id].trim()
      html += `
      <li class='ingredient'>
        <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
        </a>
        <t class='name'>${name.capitalize()}</t>
      </li>`
    }

    html += "</ul>"
    return html
  }
  
  function make_children (ingredient, all_ingredients) {
    let html = ''
    let child_ingredients = find_child_ingredients(ingredient, all_ingredients)
    if (child_ingredients.length == 0) {return html}
    
    html += "<h2>Child Ingredients</h2><ul class='ingredients'>"

    for (id in child_ingredients) {
      let name = child_ingredients[id]
      html += `
      <li class='ingredient'>
        <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
        </a>
        <t class='name'>${name.capitalize()}</t>
      </li>`
    }
    
    html += "</ul>"
    return html
  }

  function make_similar (search_name, recipes, all_ingredients) {
    let html = ''
    let ingredients = find_ingredients(recipes)
    let similar_ingredients = find_similar_ingredients(search_name, ingredients, all_ingredients)

    for (id in similar_ingredients) {
      if (similar_ingredients[id][1] < 1) { break }
      let name = similar_ingredients[id][0]
      if (name.toLowerCase() == search_name.toLowerCase()) { continue }
      html += `
      <li class='ingredient'>
        <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
          <img src='media/ingredients/${name.to_path()}.png'/>
        </a>
        <t class='name'>${name.capitalize()}</t>
      </li>`
    }
    return similar_ingredients.length >= 1 ? `<h2>Related Ingredients</h2><ul class='ingredients'>${html}<hr /></ul>` : ''
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

  function find_similar_ingredients (name, ingredients, all_ingredients) {
    let a = []
    
    let children = find_child_ingredients(name, all_ingredients)

    for (id in ingredients) {
      if (children.includes(id.toLowerCase())) {continue}
      let words = id.toLowerCase().split(' ')
      let index = similarity(name.toLowerCase().split(' '), words)
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
    let a = []
    
    for (name in all_ingredients) {
      let ingr = all_ingredients[name]
      if (!ingr.PARENT) { continue }
      let parents = ingr.PARENT.split(",").map(function (name) {return name.trim().toLowerCase()})
      if (parents.includes(search_name.toLowerCase())) {
        a.push(name.toLowerCase())
      }
    }

    return a
  }

  function similarity (a, b) {
    let score = 0
    for (a_id in a) {
      let word_a = a[a_id]
      for (b_id in b) {
        let word_b = b[b_id]
        score += word_a == word_b ? 1 : 0
      }
    }
    return score
  }

  function make_related (recipes) {
    let html = ''

    let count = 0
    for (id in recipes) {
      let recipe = recipes[id]
      let name = id
      html += `
      <li class='recipe'>
        <a onclick="Ø('query').bang('${name}')" class='photo' href='#${name.to_url()}' style='background-image:url(media/recipes/${name.to_path()}.jpg)'></a>
        <t class='name'>${name.capitalize()}</t>
        <t class='details'><b>${recipe.TIME} minutes</b><br />${count_ingredients(recipe)} ingredients<br />${recipe.INST.length} steps</t>
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
    let h = {}
    for (id in recipes) {
      let recipe = recipes[id]
      for (i in recipe.INGR) {
        let ingredients = recipe.INGR[i]
        for (n in ingredients) {
          if (n.indexOf(name.toUpperCase()) < 0) { continue }
          h[id] = recipes[id]
        }
      }
    }
    return h
  }

  function related_ingredients (name, tag, ingredients) {
    let a = []
    for (id in ingredients) {
      let ingredient = ingredients[id]
      if (!ingredient.TAGS || ingredient.TAGS.indexOf(tag) < 0 || id == name) { continue }
      a.push(id)
    }
    return a
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
}
