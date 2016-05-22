
function onmessage(m) {
  process.removeListener('message', onmessage);
}

process.on('message', onmessage);
process.send(process.pid)
