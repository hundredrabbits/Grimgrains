function Š (target) {
  const elem = document.getElementById('jump-' + target)
  if (!elem) { console.error('Undefined jump target', target); return }
  elem.scrollIntoView()
  elem.focus()
}
