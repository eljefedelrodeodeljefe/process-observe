const oberserve = require('../')
const test = require('tape')
const fork = require('child_process').fork

test('Children: get count of current process === 0, but expect the full object', (t) => {
  t.plan(1)
  t.deepEquals(oberserve.children(process.pid), {pids: [], count: 0}, 'Current process does not have any children ')
})

test('Children: After fork process has a children of a(!) discrete PID', (t) => {
  t.plan(4)
  t.deepEquals(oberserve.children(process.pid), {pids: [], count: 0}, 'Current process does not have any children')

  var n = fork(__dirname + '/fixtures/spawn.js')

  n.on('message', function(m) {
    const res = oberserve.children(process.pid)
    t.deepEquals(res.count, 1, 'Current process has one child')
    t.equal(res.pids.length, 1, 'Resulting array is of length 1')
    t.equal(res.pids[0], m, 'The on pid we have is the same that the child sends us')
  })
  n.send(process.pid)

})

test('Info: get an object of infos', (t) => {
  t.plan(4)
  const data = oberserve.info(process.pid)
  t.ok(data.pid, 'has pid result')
  t.ok(data.parent_pid, 'has ppid result')
  t.ok(data.children_count, 'has children_count result')
  t.ok(data.children, 'has children array result')
})
