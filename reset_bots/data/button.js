
function inSideElement (element, x, y) {
  const rect = element.getBoundingClientRect()
  if (x > rect.x && x < rect.right) {
    if (y > rect.y && y < rect.bottom) {
      return true
    }
  }
  return false
}

export class ClickButton {
  constructor (element, onClick) {
    this.element = element
    this.onClick = onClick

    this.pointerId = undefined

    this.upHandler = (event) => this.onPointerUp(event)
    this.element.addEventListener('pointerdown', (event) => this.onPointerDown(event), false)

    this.element.oncontextmenu = function (event) {
      event.preventDefault()
      event.stopPropagation()
    }
  }

  onPointerDown (event) {
    event.preventDefault()
    event.target.setPointerCapture(event.pointerId)
    this.pointerId = event.pointerId

    this.element.addEventListener('pointerup', this.upHandler, false)
    this.element.addEventListener('pointercancel', this.upHandler, false)
  }

  onPointerUp (event) {
    if (this.pointerId !== event.pointerId) {
      console.log('returning early !')
      return
    }
    this.pointerId = undefined

    event.preventDefault()
    console.log('UP!!!')

    event.stopPropagation()
    // this.element.releasePointerCapture(event.pointerId)
    this.element.removeEventListener('pointerup', this.upHandler, false)
    this.element.removeEventListener('pointercancel', this.upHandler, false)

    if (inSideElement(this.element, event.pageX, event.pageY)) {
      this.onClick(event)
    }
  }
}

export class HoldButton {
  constructor (element, onDown = undefined, onUp = undefined, onMove = undefined) {
    this.element = element
    this.pointerId = undefined

    this.onDown = onDown
    this.onUp = onUp
    this.onMove = onMove

    this.upHandler = (event) => this.onPointerUp(event)
    this.moveHandler = (event) => this.onPointerMove(event)

    this.element.addEventListener('pointerdown', (event) => this.onPointerDown(event), false)

    this.element.oncontextmenu = function (event) {
      event.preventDefault()
      event.stopPropagation()
    }
  }

  onPointerDown (event) {
    event.preventDefault()
    event.stopPropagation()
    console.log('DOWN')

    event.target.setPointerCapture(event.pointerId)
    this.pointerId = event.pointerId

    this.element.addEventListener('pointerup', this.upHandler, false)
    this.element.addEventListener('pointercancel', this.upHandler, false)
    this.element.addEventListener('pointermove', this.moveHandler, false)

    if (typeof this.onDown === 'function') {
      this.onDown(event)
    }
  }

  onPointerUp (event) {
    if (this.pointerId !== event.pointerId) {
      console.log('returning early !')
      return
    }
    this.pointerId = undefined

    event.preventDefault()
    console.log('UP!!!')

    event.stopPropagation()
    // this.element.releasePointerCapture(event.pointerId)
    this.element.removeEventListener('pointerup', this.upHandler, false)
    this.element.removeEventListener('pointercancel', this.upHandler, false)

    this.element.removeEventListener('pointermove', this.moveHandler, false)

    if (typeof this.onUp === 'function') {
      this.onUp(event)
    }
  }

  onPointerMove (event) {
    if (this.pointerId !== event.pointerId) {
      return
    }
    event.preventDefault()
    event.stopPropagation()
    console.log('MOVE!!!')

    if (typeof this.onMove === 'function') {
      this.onUp(event)
    }
  }
}

export class RadioButtons {
  constructor (elements, onSelect, onDeSelect) {
    // this.elements = elements
    this.buttons = []
    this.onSelect = onSelect
    this.onDeSelect = onDeSelect

    this.currentSelected = undefined

    for (const element of elements) {
      const button = new ClickButton(element, (event) => this.onClick(event))
      this.buttons.push(button)
    }
  }

  onClick (event) {
    if (typeof this.currentSelected !== 'undefined') {
      this.onDeSelect(this.currentSelected)
    }
    this.onSelect(event.target)
    this.currentSelected = event.target
  }
}
