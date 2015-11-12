var Nnb = require('../');
var assert = require('assert');

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

    it('Request to google.com with concurrency 2', function () {
        this.timeout(0);
        var nnb = new Nnb({
            host: 'google.com',
            path: '/',
            concurrency: 2
        });
        var result = nnb.go();
        assert.equal(!!result, true);
        assert.equal(!!result[0], true);
        assert.equal(!!result[1], true);
    });


});
