var NativeExtension = require('./build/Release/NativeExtension');
console.log(NativeExtension.open("google.com", 80));
module.exports = NativeExtension;
