function Runic (raw) {
  this.raw = raw

  this.runes = {
    '&': { glyph: '&', tag: 'p', class: '' },
    '~': { glyph: '~', tag: 'ul', sub: 'li', class: 'parent', stash: true },
    '-': { glyph: '-', tag: 'ul', sub: 'li', class: '', stash: true },
    '!': { glyph: '!', tag: 'table', sub: 'tr', wrap: 'th', class: 'outline', stash: true },
    '|': { glyph: '|', tag: 'table', sub: 'tr', wrap: 'td', class: 'outline', stash: true },
    '•': { glyph: '•', tag: 'ul', sub: 'li', class: 'bullet', stash: true },
    '#': { glyph: '#', tag: 'code', sub: 'ln', class: '', stash: true },
    '%': { glyph: '%' },
    '?': { glyph: '?', tag: 'note', class: '' },
    ':': { glyph: ':', tag: 'info', class: '' },
    '*': { glyph: '*', tag: 'h2', class: '' },
    '=': { glyph: '=', tag: 'h3', class: '' },
    '+': { glyph: '+', tag: 'hs', class: '' },
    '>': { glyph: '>', tag: '', class: '' },
    $: { glyph: '>', tag: '', class: '' }
  }

  this.stash = {
    rune: '',
    all: [],
    add: function (rune, item) {
      this.rune = this.copy(rune)
      this.all.push({ rune: rune, item: item })
    },
    pop: function () {
      const copy = this.copy(this.all)
      this.all = []
      return copy
    },
    is_pop: function (rune) {
      return this.all.length > 0 && rune.tag != this.rune.tag
    },
    length: function () {
      return this.all.length
    },
    copy: function (data) {
      return JSON.parse(JSON.stringify(data))
    }
  }

  this.media = function (val) {
    const service = val.split(' ')[0]
    const id = val.split(' ')[1]

    if (service == 'itchio') {
      return `<iframe frameborder="0" src="https://itch.io/embed/${id}?link_color=000000" width="600" height="167"></iframe>`
    }
    if (service == 'bandcamp') {
      return `<iframe style="border: 0; width: 600px; height: 274px;" src="https://bandcamp.com/EmbeddedPlayer/album=${id}/size=large/bgcol=ffffff/linkcol=333333/artwork=small/transparent=true/" seamless></iframe>`
    }
    return `<img src='media/${val}'/>`
  }

  this.parse = function (raw = this.raw) {
    if (!raw) { return '' }

    let html = ''
    const lines = !Array.isArray(raw) ? raw.split('\n') : raw

    for (id in lines) {
      const char = lines[id].substr(0, 1).trim().toString()
      const rune = this.runes[char]
      const trail = lines[id].substr(1, 1)
      if (char == '$') { html += '<p>' + Ø('operation').request(lines[id].substr(2)).to_markup() + '</p>'; continue }
      if (char == '%') { html += this.media(lines[id].substr(2)); continue }
      const line = lines[id].substr(2).to_markup()
      if (!line || line.trim() == '') { continue }
      if (!rune) { console.log(`Unknown rune:${char} : ${line}`) }
      if (trail != ' ') { console.warn('Runic', 'Non-rune[' + trail + '] at:' + id + '(' + line + ')'); continue }

      if (this.stash.is_pop(rune)) { html += this.render_stash() }
      if (rune.stash === true) { this.stash.add(rune, line); continue }
      html += this.render(line, rune)
    }
    if (this.stash.length() > 0) { html += this.render_stash() }
    return html
  }

  this.render_stash = function () {
    const rune = this.stash.rune
    const stash = this.stash.pop()

    let html = ''
    for (id in stash) {
      const rune = stash[id].rune
      const line = stash[id].item
      html += rune.wrap ? `<${rune.sub}><${rune.wrap}>${line.replace(/\|/g, `</${rune.wrap}><${rune.wrap}>`).trim()}</${rune.wrap}></${rune.sub}>` : `<${rune.sub}>${line}</${rune.sub}>`
    }
    return `<${rune.tag} class='${rune.class}'>${html}</${rune.tag}>`
  }

  this.render = function (line = '', rune = null) {
    if (rune && rune.tag == 'img') { return `<img src='media/${line}'/>` }
    if (rune && rune.tag == 'table') { return 'HEY' }

    return rune ? (rune.tag ? '<' + rune.tag + " class='" + rune.class + "'>" + line + '</' + rune.tag + '>' : line) : ''
  }

  this.html = function () {
    return this.parse(raw)
  }

  this.toString = function () {
    return this.html()
  }
}

String.prototype.capitalize = function () {
  return this.charAt(0).toUpperCase() + this.slice(1).toLowerCase()
}

String.prototype.to_url = function () {
  return this.toLowerCase().replace(/ /g, '+').replace(/[^0-9a-z\+]/gi, '').trim()
}

String.prototype.to_path = function () {
  return this.toLowerCase().replace(/ /g, '.').replace(/[^0-9a-z\.]/gi, '').trim()
}

String.prototype.to_markup = function () {
  html = this
  html = html.replace(/{_/g, '<i>').replace(/_}/g, '</i>')
  html = html.replace(/{\*/g, '<b>').replace(/\*}/g, '</b>')
  html = html.replace(/{\#/g, "<code class='inline'>").replace(/\#}/g, '</code>')

  const parts = html.split('{{')
  for (id in parts) {
    const part = parts[id]
    if (part.indexOf('}}') == -1) { continue }
    const content = part.split('}}')[0]
    if (content.substr(0, 1) == '$') { html = html.replace(`{{${content}}}`, Ø('operation').request(content.replace('$', ''))); continue }
    // if(content.substr(0,1) == "%"){ html = html.replace(`{{${content}}}`, this.media(content)); continue; }
    const target = content.indexOf('|') > -1 ? content.split('|')[1] : content
    const name = content.indexOf('|') > -1 ? content.split('|')[0] : content
    const external = (target.indexOf('https:') > -1 || target.indexOf('http:') > -1 || target.indexOf('dat:') > -1)
    html = html.replace(`{{${content}}}`, external ? `<a href='${target}' class='external' target='_blank'>${name}</a>` : `<a class='local' href="#${target.to_url()}" onclick="Ø('query').bang('${target}')">${name}</a>`)
  }
  return html
}
