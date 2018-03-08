
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
    Ø("template").create({x:5,y:2},TemplateNode),
    Ø("recipe").create({x:2,y:8},RecipeTemplate),
    Ø("ingredient").create({x:5,y:8},IngredientTemplate),
    Ø("page").create({x:8,y:8},PageTemplate),
  ])

  Ø("view").mesh({x:32,y:0},[

    Ø("body").create({x:2,y:2},DomNode),

    Ø("header").create({x:2,y:8},DomNode),
    Ø("logo").create({x:2,y:14},DomNode),
    Ø("search").create({x:5,y:14},DomNode),
    Ø("menu").create({x:8,y:14},DomNode),

    Ø("core").create({x:11,y:8},DomNode),
    Ø("content").create({x:11,y:14},DomNode),
    Ø("related").create({x:14,y:14},DomNode),

    Ø("footer").create({x:8,y:8},DomNode),
  ])

  // Model
  Ø("router").syphon("database")
  Ø("database").syphon(["recipes","ingredients","pages"])

  // Assoc
  Ø("template").syphon(["recipe","ingredient","page"])

  Ø("template").connect("body")
  Ø("body").bind(["header","core","footer"])
  Ø("core").bind(["content","related"])
  Ø("header").bind(["logo","search","menu"])

  Ø("query").connect("router")
  Ø("router").connect("template")

  Ø("query").bang()
}
