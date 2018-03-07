
function graph()
{
  Ø("query").create({x:2,y:4},QueryNode)

  Ø("model").mesh({x:6,y:0},[
    Ø("router").create({x:5,y:2},RouterNode),
    Ø("database").create({x:5,y:9},DatabaseNode),
    Ø("recipes").create({x:2,y:17},IndentalNode),
    Ø("ingredients").create({x:5,y:17},IndentalNode),
    Ø("pages").create({x:8,y:17},IndentalNode),
  ])

  Ø("view").mesh({x:19,y:0},[
    Ø("template").create({x:2,y:2},TemplateNode),

    Ø("recipe").create({x:2,y:7},RecipeTemplate),
    Ø("ingredient").create({x:5,y:7},IngredientTemplate),
    Ø("page").create({x:8,y:7},PageTemplate),

    Ø("main").create({x:12,y:7},DomNode),

    Ø("header").create({x:2,y:12},DomNode),
    Ø("logo").create({x:2,y:17},DomNode),
    Ø("search").create({x:6,y:17},DomNode),
    Ø("menu").create({x:10,y:17},DomNode),

    Ø("body").create({x:14,y:12},DomNode),
    Ø("related").create({x:14,y:17},DomNode),

    Ø("footer").create({x:10,y:12},DomNode),
  ])

  Ø("router").syphon("database")
  Ø("database").syphon(["recipes","ingredients","pages"])
  
  Ø("template").syphon(["recipe","ingredient","page"])

  Ø("template").bind("main")
  Ø("main").bind(["header","body","footer"])
  Ø("body").bind(["related"])
  Ø("header").bind(["logo","search","menu"])

  Ø("query").connect("router")
  Ø("router").connect("template")

  Ø("query").bang()
}
