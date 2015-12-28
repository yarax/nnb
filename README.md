# Native Node.js Benchmark module for stress testing

As you know Node.js has only one thread that doesn't allow to reach high performance in stress HTTP requesting.
Nnb uses C++ Posix threads to reach high concurency.
The limit for concurrency is on your system, check it `ulimit -n`

### Usage

```
var Nnb = require('nnb');
var nnb = new MR({
            host: 'google.com',
            path: '/',
            port: 80,
            method: 'GET',
            concurrency: 50,
            headers: {
                'user-agent': 'Mozilla/5.0'
            },
            headersOnly: true
        });
        nnb.go(function (err, result) {
            console.log(result);
        });
``` 

Output:
```
[ { time: 80,
      body: '',
      headers: 'HTTP/1.1 200 OK\r\nDate: Mon, 28 Dec 2015 10:37:35 GMT\r\nConnection: close\r\n\r\n' },
....
]
```

`headersOnly` flag is used to avoid getting bytes further from socket right after getting all headers.
In most cases that's enough to understand servers respond time and correctness.

Result is array of strings, given by server for each request



