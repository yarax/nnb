var Opener = require('./build/Release/nnb');
var url = require('url');

var NNB = function (options) {
    options = options || {};
    var urlObj;
    var self = this;

    if (!options.concurrency) {
        throw new Error("Concurrency is a mandatory parameter");
    }

    if (options.url) {
        urlObj = url.parse(options.url);
        if (urlObj.protocol === 'http') {
            options.port = options.port || 80;
        } else {
            throw new Error('Unknown protocol');
        }


        options.host = urlObj.host;
        options.path = urlObj.path + (urlObj.query || '');
    }

    options.method = (options.method || 'GET').toUpperCase();
    options.headers = options.headers || '';
    options.headers = 'Host:' + options.host + '\n\n' + options.headers;

    Object.keys(options).forEach(function (paramName) {
        self[paramName] = options[paramName];
    });

};

NNB.prototype.go = function (callback) {
    return Opener.open(this.concurrency, this.method, this.host, this.path, this.port, this.headers, callback);
};


module.exports = NNB;