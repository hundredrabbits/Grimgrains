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

function Š (target) {
  const elem = document.getElementById('jump-' + target)
  if (!elem) { console.error('Undefined jump target', target); return }
  elem.scrollIntoView()
  elem.focus()
}
