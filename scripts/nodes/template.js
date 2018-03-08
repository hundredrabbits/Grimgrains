function TemplateNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.parser

  this.cache = null;

  this.receive = function(q)
  {
    // Select the right signal
    var assoc = this.signal(q.type.slice(0, -1));
    
    this.send({body:assoc.answer(q)})

    // // Install Dom
    this.signal("body").request()
  }
}