function RouterNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.router
  this.label = "router"

  this.receive = function(q)
  {
    console.log(q)

    console.log(this.request("database"))
    var page = {type:"custom"}
    this.send(page)
  }
}