'use strict'

const runicLibrary = {
  '>': { },
  '*': { tag: 'h3' },
  '+': { tag: 'h4' },
  '?': {
    tag: 'div',
    class: 'notice',
    fn: (html, host) => {
      return html.to_markup(host)
    }
  },
  '-': {
    tag: 'li',
    wrapper: 'ul',
    wrapperClass: 'bullet',
    fn: (html, host) => {
      return html.to_markup(host)
    }
  },
  '#': {
    tag: 'li',
    wrapper: 'code',
    fn: (html, host) => {
      return html.to_markup(host)
    }
  },
  '&': {
    tag: 'p',
    fn: (html, host) => {
      return html.to_markup(host)
    }
  },
  ';': {
    fn: (html, host) => {
      console.log(html)
      return ''
    }
  },
  '%': {
    fn: (html, host) => {
      const parts = html.split(' ')
      const service = parts[0]
      const id = parts[1]
      if (service === 'itchio') { return `<iframe frameborder="0" src="https://itch.io/embed/${id}?link_color=000000" width="600" height="167"></iframe>` }
      if (service === 'bandcamp') { return `<iframe style="border: 0; width: 600px; height: 274px;" src="https://bandcamp.com/EmbeddedPlayer/album=${id}/size=large/bgcol=ffffff/linkcol=333333/artwork=small/transparent=true/" seamless></iframe>` }
      if (service === 'youtube') { return `<iframe width="100%" height="380" src="https://www.youtube.com/embed/${id}?rel=0" style="max-width:700px" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>` }
      if (service === 'custom') { return `<iframe src='${id}' style='width:100%;height:350px;'></iframe>` }
      return `<img src='media/${service}' loading='lazy' class='${id}'/>`
    }
  }
}
