'use strict'

function RecipeTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function (t) {
    let name = t.name
    if (t.result && t.result.TITLE) { name = t.result.TITLE }
    return {
      title: `GrimGrains — ${name.capitalize()}`,
      view: {
        header: {
          search: name.capitalize()
        },
        core: {
          content: make_content(t),
          related: make_related(t)
        }
      }
    }
  }

  function make_content (q) {
    const recipe = q.result
    const name = recipe.TITLE || q.name
    let html = ''

    html += `
    <h1 class='name'>${name.capitalize()}</h1>
    <h2 class='serving'>${recipe.SERV} — ${recipe.TIME} minutes</h2>
    <hr />

    <img class='photo' src='media/recipes/${q.name.to_path()}.jpg'/>

    <columns>${runic.run(recipe.DESC)}</columns>
    ${make_ingredients(recipe.INGR)}
    ${make_warnings(recipe, q.tables.ingredients)}
    ${make_instructions(recipe)}`

    return html
  }

  function make_instructions (recipe) {
    let html = ''

    html += '<h2>Instructions</h2>'

    let count = 1
    for (const cat in recipe.INST) {
      html += `<h3>Step ${count}: ${cat.capitalize()}</h3>`
      const category = recipe.INST[cat].map(convertTemperatures)
      html += runic.run(category).toString()
      count += 1
    }

    return `<div id='instructions'>${html}</div>`
  }

  function make_warnings (recipe, all_ingredients) {
    let html = ''

    for (const cat in recipe.INGR) {
      for (const id in recipe.INGR[cat]) {
        if (all_ingredients[id] && all_ingredients[id].WARN) {
          const warn = all_ingredients[id].WARN
          html += `
            <section id='warn'>
              ${runic.run(warn)}
            </section>
          `
        }
      }
    }

    return html
  }

  function formatTemperature (temperature) {
    const celcius = (parseInt(temperature) - 32) / 1.8
    return `${temperature}°F / ${parseInt(celcius / 10) * 10}°C`
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
    const recipe = q.result
    const recipes = find_related(q.name, recipe, q.tables.recipes)

    let count = 0
    for (id in recipes) {
      const name = recipes[id][0]
      const recipe = q.tables.recipes[name]
      html += `
      <li class='recipe'>
        <a href='#${name.to_url()}'>
          <div class='photo' style='background-image:url(media/recipes/${name.to_path()}.jpg)'></div>
          <span class='name'>${name.capitalize()}</span>
        </a>
        <span class='details'><b>${recipe.TIME} minutes</b><br />${count_ingredients(recipe)} ingredients<br />${Object.keys(recipe.INST).length} step${Object.keys(recipe.INST).length > 1 ? 's' : ''}</span>
      </li>`
      if (count > 1) { break }
      count += 1
    }
    return `${html}<hr/>`
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

  function find_related (name, target, recipes) {
    const a = []
    for (id in recipes) {
      const recipe = recipes[id]
      const index = similarity(target.TAGS, recipe.TAGS)
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
      const elements = categories[id]

      html += '<ul class=\'ingredients\'>'
      html += Object.keys(categories).length > 1 ? `<h3>${id.capitalize()}</h3>` : ''
      for (name in elements) {
        const element = elements[name]
        html += `
        <li class='ingredient'>
          <a href='#${name.to_url()}'>
            <img src='media/ingredients/${name.to_path()}.png'/>
            <span class='name'>${name.capitalize()}</span>
          </a>
          <span class='quantity'>${element}</span>
        </li>`
      }
      html += '<hr /></ul>'
    }
    return html
  }

  function similarity (a, b) {
    let score = 0
    for (const a_id in a) {
      const tag_a = a[a_id]
      for (const b_id in b) {
        const tag_b = b[b_id]
        score += tag_a.toLowerCase() == tag_b.toLowerCase() ? 1 : 0
      }
    }
    return score
  }
}
