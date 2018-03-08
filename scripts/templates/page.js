function PageTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    if(q.name == "HOME"){
      this.label = `page:${q.name}`
      return this.signal("home").answer(q)
    }
    if(!q.type){
      this.label = `page:${q.name}`
      return this.signal("search").answer(q)
    }

    var page = q.result

    return {
      core: {
        content: `<p>${page.BREF}</p>${new Runic(page.LONG).toString()}`
      }
    }
  }

  function list(items)
  {
    var html = ""
    for(id in items){
      html += `<ln>${id} -> ${items[id]}</ln>`
      // html += list(items[id])
    }
    return html;
  }
}