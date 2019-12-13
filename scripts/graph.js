
function graph () {
  Ø('query').create({ x: 2, y: 4 }, QueryNode)

  Ø('model').mesh({ x: 6, y: 0 }, [
    Ø('router').create({ x: 5, y: 2 }, RouterNode),
    Ø('database').create({ x: 5, y: 8 }, DatabaseNode),
    Ø('recipes').create({ x: 2, y: 14 }, IndentalNode),
    Ø('ingredients').create({ x: 5, y: 14 }, IndentalNode),
    Ø('pages').create({ x: 8, y: 14 }, IndentalNode)
  ])

  Ø('assoc').mesh({ x: 19, y: 0 }, [
    Ø('template').create({ x: 5, y: 2 }, TemplateNode),
    Ø('page').create({ x: 2, y: 8 }, PageTemplate),
    Ø('search').create({ x: 5, y: 14 }, SearchTemplate),
    Ø('home').create({ x: 2, y: 14 }, HomeTemplate),
    Ø('recipe').create({ x: 5, y: 8 }, RecipeTemplate),
    Ø('ingredient').create({ x: 8, y: 8 }, IngredientTemplate),
    Ø('service').create({ x: 8, y: 14 }, ServiceTemplate)
  ])

  Ø('client').mesh({ x: 32, y: 0 }, [
    Ø('document').create({ x: 2, y: 2 }, DocumentNode),
    Ø('view').create({ x: 2, y: 6 }, DomNode),
    Ø('header').create({ x: 2, y: 11 }, DomNode),
    Ø('logo').create({ x: 2, y: 16 }, DomNode, 'wr', `
      <a href='index.html'><img src='media/interface/logo.png' alt='Grimgrains'/></a>
      <ul>
        <li><a class="local home" href="#home">Home</a></li>
        <li><a class="local about" href="#about">About</a></li>
        <li><a class="local tools" href="#tools">Tools</a></li>
        <li><a class="local gallery" href="#gallery">Gallery</a></li>
        <li><a class="local nutrition" href="#nutrition">Nutrition</a></li>
        <li class='right'><a href='http://twitter.com/grimgrains' target='_blank'>Twitter</a></li>
      </ul>`.to_markup()),
    Ø('core').create({ x: 10, y: 11 }, DomNode),
    Ø('content').create({ x: 10, y: 16 }, DomNode),
    Ø('related').create({ x: 14, y: 16 }, DomNode, 'ul'),
    Ø('footer').create({ x: 6, y: 11 }, DomNode, 'wr', '<a href="#about">Grimgrains</a> © 2014—2019<br/><a href="http://100r.co/" target="_blank">Hundred Rabbits</a>')
  ])

  // Model
  Ø('router').syphon('database')
  Ø('database').syphon(['recipes', 'ingredients', 'pages'])

  // Assoc
  Ø('template').syphon(['recipe', 'ingredient', 'page'])
  Ø('page').syphon(['home', 'search', 'service'])

  Ø('template').connect(['view', 'document'])
  Ø('view').bind(['header', 'core', 'footer'])
  Ø('core').bind(['content', 'related'])
  Ø('header').bind(['logo'])

  Ø('query').connect('router')
  Ø('router').connect('template')

  Ø('query').bang()
}
