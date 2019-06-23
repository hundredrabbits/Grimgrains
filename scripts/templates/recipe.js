function RecipeTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function (t) {
    return {
      title: `GrimGrains — ${t.name.capitalize()}`,
      view: {
        header: {
          search: t.name.capitalize()
        },
        core: {
          content: make_content(t),
          related: make_related(t)
        }
      }
    }
  }

  function make_content (q) {
    let recipe = q.result
    let html = ''

    html += `
    <h1 class='name'>${q.name.capitalize()}</h1>
    <h2 class='serving'>${recipe.SERV} — ${recipe.TIME} minutes</h2>
    <hr />

    <img class='photo' src='media/recipes/${q.name.to_path()}.jpg'/>

    <columns>${new Runic(recipe.DESC)}</columns>
    ${make_ingredients(recipe.INGR)}
    ${make_instructions(recipe)}`

    return html
  }

  function make_instructions (recipe) {
    let html = ''

    html += `<h2>Instructions</h2>`

    let count = 1
    for (cat in recipe.INST) {
      html += `<h3>Step ${count}: ${cat.capitalize()}</h3>`
      let category = recipe.INST[cat].map(convertTemperatures)
      html += new Runic(category).toString()
      count += 1
    }

    return `<div id='instructions'>${html}</div>`
  }

  function formatTemperature (temperature) {
    const celcius = (parseInt(temperature) - 32) / 1.8
    return `${temperature}°F(${parseInt(celcius / 10) * 10}°C)`
  }

  function convertTemperatures (content) {
    const parts = content.match(/[^{\}]+(?=})/g)
    for (const id in parts) {
      const part = parts[id]
      if (part.substr(0, 1) === '#' && part.substr(-2, 2) === 'F#') {
        content = content.replace(`{${part}}`, `{#${formatTemperature(part.substr(1, part.length - 3))}#}`)
      }
    }
    return content
  }

  function make_related (q) {
    let html = ''
    let recipe = q.result
    let recipes = find_related(q.name, recipe, q.tables.recipes)

    let count = 0
    for (id in recipes) {
      let name = recipes[id][0]
      let recipe = q.tables.recipes[name]
      html += `
      <li class='recipe'>
        <a class='photo' onclick="Ø('query').bang('${name}')" href='#${name.to_url()}' style='background-image:url(media/recipes/${name.to_path()}.jpg)'></a>
        <t class='name'>${name.capitalize()}</t>
        <t class='details'><b>${recipe.TIME} minutes</b><br />${count_ingredients(recipe)} ingredients<br />${Object.keys(recipe.INST).length} step${Object.keys(recipe.INST).length > 1 ? 's' : ''}</t>
      </li>`
      if (count > 1) { break }
      count += 1
    }
    return `${html}<hr/>`
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

  function find_related (name, target, recipes) {
    let a = []
    for (id in recipes) {
      let recipe = recipes[id]
      let index = similarity(target.TAGS, recipe.TAGS)
      if (id.toLowerCase() != name.toLowerCase()) {
        a.push([id, index])
      }
    }
    a.sort(function (a, b) {
      return a[1] - b[1]
    })
    return a.reverse()
  }

  function make_ingredients (categories) {
    let html = ''
    for (id in categories) {
      let elements = categories[id]

      html += `<ul class='ingredients'>`
      html += Object.keys(categories).length > 1 ? `<h3>${id.capitalize()}</h3>` : ''
      for (name in elements) {
        let element = elements[name]
        html += `
        <li class='ingredient'>
          <a onclick="Ø('query').bang('${name}')" href='#${name.to_url()}'>
            <img src='media/ingredients/${name.to_path()}.png'/>
          </a>
          <t class='name'>${name.capitalize()}</t>
          <t class='quantity'>${element}</t>
        </li>`
      }
      html += `<hr /></ul>`
    }
    return html
  }

  function similarity (a, b) {
    let score = 0
    for (a_id in a) {
      let tag_a = a[a_id]
      for (b_id in b) {
        let tag_b = b[b_id]
        score += tag_a.toLowerCase() == tag_b.toLowerCase() ? 1 : 0
      }
    }
    return score
  }
}
