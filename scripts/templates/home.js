function HomeTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    return {
      core: {
        content: make_content(q)
      }
    }
  }

  function make_content(q)
  {
    return "Hello"
  }
}