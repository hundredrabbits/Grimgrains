function DatabaseNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.entry

  this.cache = null;

  this.answer = function(q)
  {
    if(this.cache){ return cache; }
    
    console.log("!",q)
  }
}