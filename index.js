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
        if (urlObj.protocol === 'http:') {
            options.port = options.port || 80;
        } else {
            throw new Error('Unknown protocol');
        }


        options.host = urlObj.host;
        options.path = urlObj.path + (urlObj.query || '');
    }

    options.method = (options.method || 'GET').toUpperCase();
    options.headers = options.headers || '';
    var headers = options.headers || {};
    headers['Host'] = options.host;
    if (options.data) {
        headers['Content-Length'] = options.data.length * 2;
    }

    options.headers = Object.keys(headers).reduce(function (prev, headerName) {
        return prev + headerName + ': ' + headers[headerName] + '\r\n';
    }, '') + '\r\n';

    //options.headers = 'Host:' + options.host + '\r\n' + (options.headers ? options.headers + '\r\n' : '') + '\r\n';
    options.headersOnly = options.headersOnly ? 1 : 0;
    options.data = options.data || '';

    Object.keys(options).forEach(function (paramName) {
        self[paramName] = options[paramName];
    });

};

NNB.prototype.go = function (callback) {
    return Opener.open(this.concurrency, this.method, this.host, this.path, this.port, this.headers, this.data, this.headersOnly, callback);
};


module.exports = NNB;
