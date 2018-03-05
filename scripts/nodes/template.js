function TemplateNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.parser

  this.cache = null;

  this.receive = function(q)
  {
    var dom = {
      header:{
        search:"test"
      },
      footer:"hello"
    }
    this.send({main:dom})
    this.request()
  }
}