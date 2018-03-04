function RouterNode(id,rect)
{
  Node.call(this,id,rect);

  this.glyph = NODE_GLYPHS.router
  this.label = "router"

  this.receive = function(q)
  {
    var db = this.request("database").database;
    
    console.log(q,find(q,db))

    var page = {type:"custom"}
    this.send(page)
  }

  function find(key,db)
  {
    for(id in db){
      var table = db[id].hash
      console.log(table)
      if(table[key]){
        return id
      }
    }
    return null
  }
}