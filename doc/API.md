<a name="ProcessObserve"></a>

## ProcessObserve
**Kind**: global class  
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
