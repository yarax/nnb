var NativeExtension = require('bindings')('NativeExtension');
console.log(NativeExtension.open("yarax.ru", 80));
module.exports = NativeExtension;
