var Nnb = require('../');
var assert = require('assert');
var http = require('http');

describe('nnb', function() {
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

    it('Request to google.com with concurrency 10', function (done) {
        this.timeout(0);
        var nnb = new Nnb({
            host: 'google.com',
            path: '/',
	    port: 80,
            concurrency: 10
        });
        nnb.go(function (err, result) {
            console.log(arguments);
            assert.equal(typeof result[0], 'string');
            assert.equal(typeof result[1], 'string');
            done();
        });
        
    });

    it('10 requests to google from async js', function (done) {
        this.timeout(200000);
        var i=0;
        var n = 0;
        var call = function() {
            var options = {
              hostname: 'google.com',
              port: 80,
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
    });


});
