const libco = require('../')
const test = require('tape')

test('globals get registered', (t) => {
  t.plan(1)
  t.deepEquals(go(), 0, 'Go gets registered')
})
