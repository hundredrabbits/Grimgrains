function DatabaseNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.cache

  this.cache = null;

  this.answer = function(q)
  {
    if(this.cache){ return cache; }

    this.cache = this.request();

    return this.cache;
  }
}