'use strict'

function HomeTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  this.answer = function (q) {
    const ingredients = find_ingredients(q.tables.recipes)

    translate_ingredients(q.tables.ingredients)
    // translate_recipes(q.tables.recipes)

    ingredients.coffee = 1

    const sorted_ingredients = sort_ingredients(ingredients)

    const html = `
    <h1>Ingredients <a class='jump' id='jump-ingredients' href='javascript:Š("recipes")'>recipes</a></h1>
    ${make_ingredients(sorted_ingredients, q.tables.ingredients)}
    <h1 id='recipes_header'>Recipes <a class='jump' id='jump-recipes' href='javascript:Š("ingredients")'>ingredients</a></h1>
    ${make_recipes(q.tables.recipes)}`

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




  function translate_recipes (recipes) {

    let txt = ''

    for(const name of Object.keys(recipes)){
      const recipe = recipes[name]
      const snake_name = name.toLowerCase().replace(/ /g,'_').trim()

      txt += `// ${name.toLowerCase()}\n`
      txt += `Recipe ${snake_name} = create_recipe("${name.toLowerCase()}", "${recipe.TAGS[0]}", "${recipe.SERV}", ${recipe.DATE.replace(/-/g,'')}, ${recipe.TIME});\n`
      txt += `set_description(&${snake_name}, "${recipe.DESC.reduce((acc,item) => { return `${acc}${item.substr(2).to_markup2().trim()}<br /><br />`},'')}");\n`

      for(const part in recipe.INST){
        if(!recipe.INGR[part]){ console.warn(snake_name,part) }
        const part_name = snake_name+'_'+part.toLowerCase().replace(/ /g,'_').trim()
        txt += `RecipePart ${part_name} = create_part("${part.toLowerCase()}");\n`
        // instructions
        for(const inst of recipe.INST[part]){
          txt += `add_instruction(&${part_name}, "${inst.substr(2).to_markup2()}");\n`
        }
        // instructions
        if(recipe.INGR[part]){
          for(const ingr in recipe.INGR[part]){
            const ingr_name = ingr.toLowerCase().replace(/ /g,'_').trim()
            txt += `add_serving(&${part_name}, &${ingr_name}, "${recipe.INGR[part][ingr]}");\n`
          }
        }
        txt += `add_part(&${snake_name}, &${part_name});\n`
      }
      txt += '\n'
    }

    console.log(txt)
  }




  function translate_ingredients (ingredients) {

    let txt = ''
    let each_ingr = ''

    console.log(ingredients)

    for(const name in ingredients){
      const snake_name = name.toLowerCase().replace(/ /g,'_').trim()    
      each_ingr += `&${snake_name}, `  
      let desc = ingredients[name].BREF ? ingredients[name].BREF.to_markup2() : 'Missing description.'
      desc += ingredients[name].LONG ? ingredients[name].LONG.reduce((acc,item) => { return `${acc}${item.substr(2).to_markup2().trim()}<br /><br />`},'') : ''

      if(ingredients[name].PARENT){
        const parent_snake = ingredients[name].PARENT.toLowerCase().replace(/ /g,'_').trim()  
        txt += `Ingredient ${snake_name} = create_child_ingredient(&${parent_snake}, "${name.toLowerCase()}", "${desc}");\n`
      }
      else{
        txt += `Ingredient ${snake_name} = create_ingredient("${name.toLowerCase()}", "${desc}");\n`
      }
      txt += `\n`
    }

    txt += `Ingredient *ingredients[] = {${each_ingr}};`

    console.log(txt)
  }








  function make_recipes (recipes) {
    let html = ''

    // Sort by tag

    const categorized = {}

    for (const name in recipes) {
      const recipe = recipes[name]
      if(!recipe.TAGS){ console.warn(name,"broken"); continue; }
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
