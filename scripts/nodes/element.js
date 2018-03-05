function ElementNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.element
  this.el = document.createElement("yu");
  this.el.id = this.id;

  this.answer = function(q)
  {
    return this.el
  }
}