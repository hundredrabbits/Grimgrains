function BangNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.bang
}

function ValueNode(id,rect,param)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.value
  this.label = param
  this.param = param

  this.answer = function()
  {
    return this.param
  }
}

function PrintNode(id,rect)
{
  Node.call(this,id,rect);
  this.glyph = NODE_GLYPHS.render

  this.receive = function(q)
  {
    this.label = q;
  }
}

function AddNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.equal
  this.label = "add"

  this.add = function()
  {
    var sum = 0;
    var values = this.request();
    for(id in values){
      sum += values[id];
    }
    return sum
  }

  this.receive = function(q)
  {
    this.send(this.add())
  }

  this.answer = function()
  {
    return this.add()
  }
}

function graph()
{
  Ø("query").cast({x:2,y:4},QueryNode)
  Ø("print").cast({x:26,y:4},PrintNode)

  Ø("model").mesh({x:6,y:0},[
    Ø("router").cast({x:2,y:2},RouterNode),
    Ø("database").cast({x:2,y:8},DatabaseNode),
    Ø("recipes").cast({x:2,y:14},DictionaryNode),
  ])

  Ø("view").mesh({x:14,y:0},[
    Ø("template").cast({x:2,y:2},TemplateNode),
    Ø("body").cast({x:2,y:8},ElementNode),
  ])

  Ø("router").syphon("database")
  Ø("database").syphon("recipes")

  Ø("template").syphon("body")

  Ø("query").connect("router")
  Ø("router").connect("template")
  Ø("template").connect("print")

  Ø("query").bang()
}
