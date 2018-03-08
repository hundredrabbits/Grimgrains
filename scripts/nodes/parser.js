function ParserNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.router

  this.receive = function(q)
  {
    this.label = `parser=${q.name}`
    var assoc = this.signal(q.type.slice(0, -1));

    this.send(assoc)
  }
}