function DatabaseNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.builder

  this.cache = null;

  this.answer = function(q)
  {
    this.cache = this.cache ? this.cache : this.request();
    return this.request(this.cache);
  }
}