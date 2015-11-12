var Opener = require('./build/Release/nnb');

var NNB = function (options) {
    options = options || {};
    var required = {
        concurrency: 1,
        path: 1,
        host: 1
    };
    var defaults = {
        port: 80
    };
    var self = this;

    Object.keys(required).forEach(function (param) {
        if (!options[param]) {
            throw new Error(param + " is a mandatory parameter");
        }
    });

    Object.keys(defaults).forEach(function (paramName) {
        self[paramName] = defaults[paramName];
    });

    Object.keys(options).forEach(function (paramName) {
        self[paramName] = options[paramName];
    });

};

NNB.prototype.go = function () {
    return Opener.open(this.concurrency, this.host, this.path, this.port);
};


module.exports = NNB;