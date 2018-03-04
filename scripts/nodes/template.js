function TemplateNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.builder

  this.receive = function(page)
  {
    console.log("page",page)
  }
}