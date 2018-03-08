
function graph()
{
  Ø("query").create({x:2,y:4},QueryNode)

  Ø("model").mesh({x:6,y:0},[
    Ø("router").create({x:5,y:2},RouterNode),
    Ø("database").create({x:5,y:8},DatabaseNode),
    Ø("recipes").create({x:2,y:14},IndentalNode),
    Ø("ingredients").create({x:5,y:14},IndentalNode),
    Ø("pages").create({x:8,y:14},IndentalNode),
  ])

  Ø("assoc").mesh({x:19,y:0},[
    Ø("parser").create({x:5,y:2},ParserNode),
    Ø("recipe").create({x:2,y:8},RecipeTemplate),
    Ø("ingredient").create({x:5,y:8},IngredientTemplate),
    Ø("page").create({x:8,y:8},PageTemplate),
  ])

  Ø("view").mesh({x:32,y:0},[
    Ø("template").create({x:2,y:2},TemplateNode),

    Ø("main").create({x:14,y:7},DomNode),

    Ø("header").create({x:2,y:12},DomNode),
    Ø("logo").create({x:2,y:17},DomNode),
    Ø("search").create({x:6,y:17},DomNode),
    Ø("menu").create({x:10,y:17},DomNode),

    Ø("body").create({x:14,y:12},DomNode),
    Ø("core").create({x:14,y:17},DomNode),
    Ø("suggestions").create({x:17,y:17},DomNode),

    Ø("footer").create({x:10,y:12},DomNode),
  ])

  // Model
  Ø("router").syphon("database")
  Ø("database").syphon(["recipes","ingredients","pages"])

  // Assoc
  Ø("parser").syphon(["recipe","ingredient","page"])
  Ø("parser").connect("template")

  Ø("template").bind("main")
  Ø("main").bind(["header","body","footer"])
  Ø("body").bind(["core","suggestions"])
  Ø("header").bind(["logo","search","menu"])

  Ø("query").connect("router")
  Ø("router").connect("parser")

  Ø("query").bang()
}
