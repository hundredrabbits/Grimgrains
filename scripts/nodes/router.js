function RouterNode (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.router

  this.receive = function (q) {
    let target = q.indexOf(':') > -1 ? q.split(':')[0].replace(/\+/g, ' ') : q.replace(/\+/g, ' ')
    let params = q.indexOf(':') > -1 ? q.split(':')[1] : null
    let db = this.request('database').database
    let data = find(target.toUpperCase(), db)

    this.label = `${this.id}|${target}|${params}`

    console.log(this.id, `${data ? data.type : '?'}->${target}[${params}]`)

    this.send({
      name: target,
      type: data ? data.type : null,
      result: data ? data.result : null,
      params: params,
      tables: db
    })
  }

  function find (key, db) {
    if (parseInt(key) > 0) { return null }

    for (id in db) {
      let table = db[id]
      if (table[key]) {
        return { type: id, result: table[key] }
      }
    }
    return { type: null, result: null }
  }
}
