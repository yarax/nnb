var NativeExtension = require('./build/Release/NativeExtension');

var res = NativeExtension.open(10, "google.com", 80);

console.log(res);
console.log(res.length);

