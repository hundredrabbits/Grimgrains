
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
    Ø("page").create({x:2,y:8},PageTemplate),
    Ø("search").create({x:5,y:14},SearchTemplate),
    Ø("home").create({x:2,y:14},HomeTemplate),
    Ø("recipe").create({x:5,y:8},RecipeTemplate),
    Ø("ingredient").create({x:8,y:8},IngredientTemplate),
  ])

  Ø("client").mesh({x:32,y:0},[
    Ø("view").create({x:2,y:2},DomNode),
    Ø("header").create({x:2,y:8},DomNode),
    Ø("logo").create({x:2,y:14},DomNode,"wr","<a href='#home'><img src='media/interface/logo.png'/></a>"),
    Ø("menu").create({x:6,y:14},DomNode,"list"),
    Ø("core").create({x:10,y:8},DomNode),
    Ø("content").create({x:10,y:14},DomNode),
    Ø("related").create({x:14,y:14},DomNode,"list"),
    Ø("footer").create({x:6,y:8},DomNode),
  ])

  // Model
  Ø("router").syphon("database")
  Ø("database").syphon(["recipes","ingredients","pages"])

  // Assoc
  Ø("template").syphon(["recipe","ingredient","page"])
  Ø("page").syphon(["home","search"])

  Ø("template").connect("view")
  Ø("view").bind(["header","core","footer"])
  Ø("core").bind(["content","related"])
  Ø("header").bind(["logo","menu"])

  Ø("query").connect("router")
  Ø("router").connect("template")

  Ø("query").bang()
}
