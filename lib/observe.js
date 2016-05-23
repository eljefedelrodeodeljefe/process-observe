'use strict'
const bindings = require('./bindings')
const Readable = require('stream').Readable

/**
 * @class
 */
class ProcessObserve extends Readable {
  constructor() {
    super()
  }
}

/**
 * Query a process associated with a specified `pid` for the number of and a
 * list of pids of childnre it has.
 * @example
 * const observe = require('process-observer')
 * // query the OS pid for all its children (100+), here with sync API
 * observe.children(0)
 * // -> {pids: [1, 2, 50, ...], count: 121}
 *
 * // inspect your current process (here assuming that you have just spawned
 * // a new process), here with async API
 * observe.children(process.pid, (err, result) => {
 *   if (err)
 *     console.log(err)
 *   console.log(result)
 *   // -> {pids: [5841], count: 1}
 * })
 *
 * @param  {Number}   pid `pid` to query
 * @param  {Function} [cb] optional callback. If provided sync function will be called on next tick.
 * @return {Object}       returns an object with [0] the list of pids and [1] their count
 */
ProcessObserve.prototype.children = (pid, cb) => {
  if (typeof pid !== 'number') {
    const err = new TypeError('First argument must be a PID (number).')
    if (arguments.length === 1) {
      throw err
    } else {
      return cb(err)
    }
  }

  if (arguments.length === 2) {
    process.nextTick(() => {
      return cb(null, bindings.children(pid))
    })
  } else {
    return bindings.children(pid)
  }
};

/**
 * Query a process associated with a specified `pid` a wider process specific data
 * such as a its parent, children, hardware usage etc. (see list TBD)
 * @example
 * const observe = require('process-observer')
 * const currenProcess = process.pid
 * // sync
 * observe.info(currenProcess)
 * // -> { pid: 3804, parent_pid: 3803, children_count: 1, children: [ 3805 ]}
 *
 * // async
 * observe.info(currenProcess, (err, result) => {
 *   if (err)
 *     console.log(err)
 *   console.log(result)
 *    // -> {pid: 3804, parent_pid: 3803, children_count: 1, children: [ 3805 ]}
 * })
 *
 * @param  {Number}   pid `pid` to query
 * @param  {Function} [cb] optional callback. If provided sync function will be called on next tick.
 * @return {Object}       returns an object with process specific data
 */
ProcessObserve.prototype.info = (pid, cb) => {
  if (typeof pid !== 'number') {
    const err = new TypeError('First argument must be a PID (number).')
    if (arguments.length === 1) {
      throw err
    } else {
      return cb(err)
    }
  }

  if (arguments.length === 2) {
    process.nextTick(() => {
      return cb(null, bindings.info(pid))
    })
  } else {
    return bindings.info(pid)
  }
};

module.exports = new ProcessObserve()
