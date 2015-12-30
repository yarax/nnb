var Nnb = require('../');
var assert = require('assert');
var http = require('http');
//var SegfaultHandler = require('segfault-handler');
//SegfaultHandler.registerHandler("crash.log");

var port = 3678;

describe('nnb', function() {

    it('Before', function (done) {
        var serv = http.createServer(function (req, res) {
            res.end('Method: ' + req.method);
        }).listen(port, function () {
            console.log('Server is up on', port);
            done();
        });
    });

    it('Call without required fields', function () {
        assert.throws(
            function() {
                var nnb = new Nnb();
            },
            Error
        );
        assert.throws(
            function() {
                var nnb = new Nnb({
                    host: 1
                });
            },
            Error
        );
    });

    it('Request with headers only', function (done) {
        this.timeout(0);
        var rn = 10;
        var nnb = new Nnb({
            host: 'localhost',
            path: '/',
            port: port,
            concurrency: rn,
            method: 'POST',
	        data: 'a=1&b=2',
            headers: {
                'Content-type' : 'text/plain'
            },
            headersOnly: true
        });
        var ts1 = Date.now();
        nnb.go(function (err, result) {
            assert(result.length, rn);
            assert(typeof result[0].time, 'number');
            assert(typeof result[0].headers, 'string');
            assert.equal(result[0].body, '');

            /*var ts = Date.now() - ts1;
            var rps = rn/(ts/1000);
            console.log("RPS", rps, result);*/
            done();
        });
        
    });

    it('Request with body', function (done) {
        this.timeout(0);
        var rn = 10;
        var nnb = new Nnb({
            host: 'localhost',
            path: '/',
            port: port,
            concurrency: rn,
            method: 'POST',
            data: 'a=1&b=2',
            headers: {
                'Content-type' : 'text/plain'
            },
            headersOnly: false
        });
        var ts1 = Date.now();
        nnb.go(function (err, result) {
            assert(result.length, rn);
            assert(typeof result[0].time, 'number');
            assert(typeof result[0].headers, 'string');
            assert.equal(result[0].body, 'Method: POST');

            /*var ts = Date.now() - ts1;
             var rps = rn/(ts/1000);
             console.log("RPS", rps, result);*/
            done();
        });

    });

    /*it('10 requests to google from async js', function (done) {
        this.timeout(200000);
        var i=0;
        var n = 0;
        var call = function() {
            var options = {
              hostname: 'localhost',
              port: 8082,
              path: '/',
              method: 'GET'
            };

            var req = http.request(options, function(res) {
                i++;
                if (i < n) call();
                else done();
            });

            req.on('error', done);

            req.end();
        }
        call();
    });*/


});
