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
