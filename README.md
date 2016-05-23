# process-observe

Native bindings to observation APIs like `sysctl` (BSDs), `proc/{pid}/` (Linux) and Windows equivalents in the context of process observation.

[![Build Status](https://travis-ci.org/eljefedelrodeodeljefe/process-observe.svg?branch=master)](https://travis-ci.org/eljefedelrodeodeljefe/process-observe) [![Build status](https://ci.appveyor.com/api/projects/status/59q34ua3i457k27x?svg=true)](https://ci.appveyor.com/project/eljefederodeodeljefe/process-observe) [![js-standard-style](https://img.shields.io/badge/code%20style-standard-brightgreen.svg?style=flat)](http://standardjs.com/) [![Join the chat at https://gitter.im/eljefedelrodeodeljefe/process-observe](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/eljefedelrodeodeljefe/process-observe?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[![NPM](https://nodei.co/npm-dl/process-observe.png?months=6&height=2)](https://nodei.co/npm/process-observe/)

## TL;DR

> In some cases you have orphaned process or just want to watch certain processes. To work with them from node you need either handles to them or need to observe them first. This module aids those purposes.

```js
// inspect your current process (here assuming that you have just spawned
// a new process), here with async API
observe.children(process.pid, (err, result) => {
	if (err)
		console.log(err)

	console.log(result)
	// -> {pids: [5841], count: 1}
})
```
## Implementation Status<a name="status"></a>
| Method | implemented |
| --- | --- |
| .info(pid [, cb]) | - |
| .children(pid [, cb]) | **yes** |



### Overview

### Technical Overview

**Rquirements:**
* Node 4.0.0+

## Platform

This module is currently tested on:

| Platform | 0.12 | 3.0 | 4.0 | 5.0 | 6.0 |
| --- | --- | --- | --- | ---| ---|---|
| Mac OS X | - | - | **yes** | **yes**| **yes** |
| BSDs | - | - | - | - | - |
| Linux | - | - | **yes** | **yes**  | **yes** |
| Windows | - | - | - | - | - |

## Roadmap

Please see [list of the implemented methods](#status) for now.


## API
<a name="ProcessObserve"></a>

## ProcessObserve
**Kind**: global class  

* [ProcessObserve](#ProcessObserve)
    * [.children(pid, [cb])](#ProcessObserve+children) ⇒ <code>Object</code>
    * [.info(pid, [cb])](#ProcessObserve+info) ⇒ <code>Object</code>

<a name="ProcessObserve+children"></a>

### processObserve.children(pid, [cb]) ⇒ <code>Object</code>
Query a process associated with a specified `pid` for the number of and a
list of pids of childnre it has.

**Kind**: instance method of <code>[ProcessObserve](#ProcessObserve)</code>  
**Returns**: <code>Object</code> - returns an object with [0] the list of pids and [1] their count  

| Param | Type | Description |
| --- | --- | --- |
| pid | <code>Number</code> | `pid` to query |
| [cb] | <code>function</code> | optional callback. If provided sync function will be called on next tick. |

**Example**  
```js
const observe = require('process-observer')
// query the OS pid for all its children (100+), here with sync API
observe.children(0)
// -> {pids: [1, 2, 50, ...], count: 121}

// inspect your current process (here assuming that you have just spawned
// a new process), here with async API
observe.children(process.pid, (err, result) => {
	if (err)
		console.log(err)
	console.log(result)
	// -> {pids: [5841], count: 1}
})
```
<a name="ProcessObserve+info"></a>

### processObserve.info(pid, [cb]) ⇒ <code>Object</code>
Query a process associated with a specified `pid` a wider process specific data
such as a its parent, children, hardware usage etc. (see list TBD)

**Kind**: instance method of <code>[ProcessObserve](#ProcessObserve)</code>  
**Returns**: <code>Object</code> - returns an object with process specific data  

| Param | Type | Description |
| --- | --- | --- |
| pid | <code>Number</code> | `pid` to query |
| [cb] | <code>function</code> | optional callback. If provided sync function will be called on next tick. |

**Example**  
```js
const observe = require('process-observer')
const currenProcess = process.pid
// sync
observe.info(currenProcess)
// -> { pid: 3804, parent_pid: 3803, children_count: 1, children: [ 3805 ]}

// async
observe.info(currenProcess, (err, result) => {
	if (err)
		console.log(err)
	console.log(result)
	// -> {pid: 3804, parent_pid: 3803, children_count: 1, children: [ 3805 ]}
})
```
## License

MIT
