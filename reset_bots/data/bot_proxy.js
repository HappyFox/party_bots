/* global WebSocket */

export class BotProxy {
  constructor (name = undefined) {
    let addr = 'ws://' + window.location.hostname
    if (name) {
      addr = 'ws://' + name + '.local'
    }
    console.log('addr : ' + addr)
    this.websocket = new WebSocket(addr + ':81/')
  }

  setState (state) {
    if (this.websocket.readyState === WebSocket.OPEN) {
      this.websocket.send(JSON.stringify({ action: 'set_state', state: state}))
    }
  }

  setFace (face) {
    if (this.websocket.readyState === WebSocket.OPEN) {
      this.websocket.send(JSON.stringify({ action: 'face', face: face }))
    }
  }

  setColor (red, green, blue) {
    if (this.websocket.readyState === WebSocket.OPEN) {
      this.websocket.send(JSON.stringify({ action: 'setColor', red: red, green: green, blue: blue }))
    }
  }

  setLook (direction) {
    if (this.websocket.readyState === WebSocket.OPEN) {
      this.websocket.send(JSON.stringify({ action: 'look', direction: direction }))
    }
  }

  clearLook () {
    if (this.websocket.readyState === WebSocket.OPEN) {
      this.websocket.send(JSON.stringify({ action: 'clearLook' }))
    }
  }
}
