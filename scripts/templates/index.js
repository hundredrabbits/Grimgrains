function IndexTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    console.log("!!")
    return {
      title:`GrimGrains — Index`,
      view:{
        core: {
          content: `Hello`
        }
      }
    }
  }
}