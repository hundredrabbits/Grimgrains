function RouterNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.router
  this.label = "router"

  this.receive = function(q)
  {
    var q = q.toUpperCase();
    var db = this.request("database").database;

    var type = find(q,db)

    this.label = `Router(${type})`
    this.send({
      name:q,
      type:type,
      result:db[type][q],
      tables:db
    })
  }

  function find(key,db)
  {
    for(id in db){
      var table = db[id]
      if(table[key]){
        return id
      }
    }
    return null
  }
}