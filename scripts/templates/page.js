function PageTemplate(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.render

  // Create the recipe body

  this.answer = function(q)
  {
    if(this.signal(q.name.toLowerCase())){
      this.label = `page:${q.name}`
      return this.signal(q.name.toLowerCase()).answer(q)      
    }
    if(!q.type){
      this.label = `page:${q.name}`
      return this.signal("search").answer(q)
    }

    var page = q.result
    return {
      title:`GrimGrains — ${q.name.capitalize()}`,
      view:{
        core: {
          content: `<h1>${q.name.capitalize()}</h1><p>${page.BREF.to_markup()}</p>${new Runic(page.LONG).toString()}`
        }
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