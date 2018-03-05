function DomNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.dom

  this.el = document.createElement("yu")
  this.is_installed = false;

  this.receive = function(page)
  {
    if(!this.is_installed){
      this.install(this.request());
    }

    this.update(page);
  }

  this.install = function(elements)
  {
    for(id in elements){
      var el = elements[id];
      this.el.appendChild(el)
    }
    document.body.appendChild(this.el)
    this.is_installed = true;
  }

  this.update = function()
  {
    
  }
}