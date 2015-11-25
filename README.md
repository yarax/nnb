# Native Node.js module for stress testing

As you know Node.js has only one thread that doesn't allow to reach high performance in stress HTTP requesting.
Multi request uses C++ Posix threads to reach hight concurency.
The only limit for concurency is your system, check it `ulimit -n`

### Usage

```
var MR = require('multi-request');
var mr = new MR({
            host: 'google.com',
            path: '/',
            port: 80,
            concurrency: 100
        });
        nnb.go(function (err, result) {
            console.log(result);
        });
``` 

Output:
```
[ 'HTTP/1.0 302 Found\r\nCache-Control: private\r\nContent-Type: text/html; charset=UTF-8\r\nLocation: http://www.google.de/?gfe_rd=cr&ei=M3dVVr7lIsWG8QeA3ZuIBw\r\nContent-Length: 258\r\nDate: Wed, 25 Nov 2015 08:54:11 GMT\r\nServer: GFE/2.0\r\n\r\n<HTML><HEAD><meta http-equiv="content-type" content="text/html;charset=utf-8">\n<TITLE>302 Moved</TITLE></HEAD><BODY>\n<H1>302 Moved</H1>\nThe document has moved\n<A HREF="http://www.google.de/?gfe_rd=cr&amp;ei=M3dVVr7lIsWG8QeA3ZuIBw">here</A>.\r\n</BODY></HTML>\r\n',
....
]
```

Result is array of strings, given by server for each request with headers



