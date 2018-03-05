
function graph()
{
  Ø("query").cast({x:2,y:4},QueryNode)

  Ø("model").mesh({x:6,y:0},[
    Ø("router").cast({x:5,y:2},RouterNode),
    Ø("database").cast({x:5,y:8},DatabaseNode),
    Ø("recipes").cast({x:2,y:14},DictionaryNode),
    Ø("ingredients").cast({x:5,y:14},DictionaryNode),
    Ø("pages").cast({x:8,y:14},DictionaryNode),
  ])

  Ø("view").mesh({x:18,y:0},[
    Ø("template").cast({x:2,y:2},TemplateNode),
    Ø("main").cast({x:8,y:2},DomNode),

    Ø("header").cast({x:14,y:2},DomNode),
    Ø("logo").cast({x:20,y:2},DomNode),
    Ø("search").cast({x:20,y:6},DomNode),
    Ø("menu").cast({x:20,y:10},DomNode),

    Ø("body").cast({x:14,y:10},DomNode),
    Ø("related").cast({x:20,y:14},DomNode),

    Ø("footer").cast({x:14,y:6},DomNode),
  ])

  Ø("router").syphon("database")
  Ø("database").syphon(["recipes","ingredients","pages"])

  Ø("main").connect(["header","body","footer"])
  Ø("body").connect(["related"])
  Ø("header").connect(["logo","search","menu"])

  Ø("query").connect("router")
  Ø("router").connect("template")
  Ø("template").connect("main")

  Ø("query").bang()
}
