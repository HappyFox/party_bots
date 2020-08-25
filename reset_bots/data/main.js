/* global getComputedStyle */

import { ClickButton, HoldButton, RadioButtons } from './button.js'
import { BotProxy } from './bot_proxy.js'

const allElements = document.querySelectorAll('.all')
const bobElements = document.querySelectorAll('.bob')

const pressedColor = getComputedStyle(document.documentElement).getPropertyValue('--control-toggled')

const bots = {
  bob: new BotProxy('bob')
}

function onModeSelect (target) {
  target.style.backgroundColor = pressedColor
  target.style.borderColor = pressedColor

  const face = target.textContent
  const state = target.getAttribute('state')
  const name = target.getAttribute('bot')

  if (name in bots) {
    bots[name].setState(state)
  }
  // bot.setFace(face)
}

function onModeDeSelect (target) {
  target.style.backgroundColor = null
  target.style.borderColor = null
}

const allButtons = new RadioButtons(allElements, onModeSelect, onModeDeSelect)
const bobButtons = new RadioButtons(bobElements, onModeSelect, onModeDeSelect)
