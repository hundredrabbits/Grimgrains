'use strict'

function IndentalNode (id, rect, type) {
  Node.call(this, id, rect)

  this.type = type
  this.glyph = NODE_GLYPHS.database

  this.answer = function (q) {
    if (!DATABASE[this.id]) {
      console.warn(`Missing /database/${this.id}.js`)
      return null
    }
    if (this.cache) {
      return this.cache
    }

    this.label = this.type ? `${this.id}=${this.type.name}` : `${this.id}`
    this.cache = parse(DATABASE[this.id], this.type)
    return this.cache
  }

  function parse (data, type) {
    return build(data.split('\n').map(liner), type)

    function build (lines, type) {
      // Assoc lines
      const stack = {}
      let target = lines[0]
      for (id in lines) {
        const line = lines[id]
        if (line.skip) { continue }
        target = stack[line.indent - 2]
        if (target) { target.children.push(line) }
        stack[line.indent] = line
      }

      // Format
      const h = {}
      for (id in lines) {
        const line = lines[id]
        if (line.skip || line.indent > 0) { continue }
        const key = line.content.toUpperCase()
        h[key] = type ? new type(key, format(line)) : format(line)
      }
      return h
    }

    function format (line) {
      const a = []
      const h = {}
      for (id in line.children) {
        const child = line.children[id]
        if (child.key) { h[child.key.toUpperCase()] = child.value } else if (child.children.length == 0 && child.content) { a.push(child.content) } else { h[child.content.toUpperCase()] = format(child) }
      }
      return a.length > 0 ? a : h
    }

    function liner (line) {
      return {
        indent: line.search(/\S|$/),
        content: line.trim(),
        skip: line == '' || line.substr(0, 1) == '~',
        key: line.indexOf(' : ') > -1 ? line.split(' : ')[0].trim() : null,
        value: line.indexOf(' : ') > -1 ? line.split(' : ')[1].trim() : null,
        children: []
      }
    }
  }
}

const DATABASE = {}
