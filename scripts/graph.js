
function graph()
{
  Ø("query").cast({x:2,y:4},QueryNode)

  Ø("model").mesh({x:6,y:0},[
    Ø("router").cast({x:5,y:2},RouterNode),
    Ø("database").cast({x:5,y:9},DatabaseNode),
    Ø("recipes").cast({x:2,y:17},DictionaryNode),
    Ø("ingredients").cast({x:5,y:17},DictionaryNode),
    Ø("pages").cast({x:8,y:17},DictionaryNode),
  ])

  Ø("view").mesh({x:18,y:0},[
    Ø("template").cast({x:2,y:2},TemplateNode),
    Ø("main").cast({x:8,y:7},DomNode),

    Ø("header").cast({x:2,y:12},DomNode),
    Ø("logo").cast({x:2,y:17},DomNode),
    Ø("search").cast({x:6,y:17},DomNode),
    Ø("menu").cast({x:10,y:17},DomNode),

    Ø("body").cast({x:14,y:12},DomNode),
    Ø("related").cast({x:14,y:17},DomNode),

    Ø("footer").cast({x:10,y:12},DomNode),
  ])

  Ø("router").syphon("database")
  Ø("database").syphon(["recipes","ingredients","pages"])

  Ø("template").bind("main")
  Ø("main").bind(["header","body","footer"])
  Ø("body").bind(["related"])
  Ø("header").bind(["logo","search","menu"])

  Ø("query").connect("router")
  Ø("router").connect("template")

  Ø("query").bang()
}
