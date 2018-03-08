function TemplateNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.parser

  this.cache = null;

  this.receive = function(q)
  {
    console.log(q)
    // var template = this.signal(q.type.slice(0, -1));
    // var related_template = this.signal("related");

    // console.log(related_template)
    // var dom = {
    //   header:{
    //     search:q.name
    //   },
    //   body:{
    //     core: template.answer(q),
    //     related: "qq"
    //   },
    //   footer:"hello"
    // }
    // this.send({main:dom})

    // // Install Dom
    // this.signal("main").request()
  }
}