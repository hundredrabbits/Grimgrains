function SearchTemplate (id, rect) {
  Node.call(this, id, rect)

  this.glyph = NODE_GLYPHS.render

  // Create the search body

  this.answer = function (q) {
    let html = ''

    return {
      title: `GrimGrains — Search`,
      view: {
        header: {
          search: 'search'
        },
        core: {
          content: make_content(q),
          related: ''
        }
      }
    }
  }

  function make_content (q) {
    let html = ''

    const index = Object.keys(q.tables.ingredients).concat(Object.keys(q.tables.recipes))
    const similar = findSimilar(q.name.toUpperCase(), index)

    html += `
    <h1 class='name'>Could not find "${q.name}"</h1>
    <h2 class='serving'>404</h2>
    <hr />
    <p>Did you mean <a onclick="Ø('query').bang('${similar[0].word.toLowerCase()}')" href='#${similar[0].word.toLowerCase().to_url()}'>${similar[0].word.toLowerCase()}</a>, <a onclick="Ø('query').bang('${similar[1].word.toLowerCase()}')" href='#${similar[1].word.toLowerCase().to_url()}'>${similar[1].word.toLowerCase()}</a> or <a onclick="Ø('query').bang('${similar[2].word.toLowerCase()}')" href='#${similar[2].word.toLowerCase().to_url()}'>${similar[2].word.toLowerCase()}</a>.</p>`

    return html
  }

  function findSimilar (target, list) {
    const similar = []
    for (const key in list) {
      const word = list[key]
      similar.push({ word: word, value: similarity(target, word) })
    }
    return similar.sort(function (a, b) {
      return a.value - b.value
    }).reverse()
  }

  function similarity (a, b) {
    let val = 0
    for (let i = 0; i < a.length; ++i) { val += b.indexOf(a.substr(i)) > -1 ? 1 : 0 }
    for (let i = 0; i < b.length; ++i) { val += a.indexOf(b.substr(i)) > -1 ? 1 : 0 }
    a = a.split('').sort().join('')
    b = b.split('').sort().join('')
    for (let i = 0; i < a.length; ++i) { val += b.indexOf(a.substr(i)) > -1 ? 1 : 0 }
    for (let i = 0; i < b.length; ++i) { val += a.indexOf(b.substr(i)) > -1 ? 1 : 0 }
    return val
  }
}
