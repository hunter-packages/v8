// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Flags: --expose-wasm

load('test/mjsunit/wasm/wasm-module-builder.js');

function CreateDefaultBuilder() {
  const builder = new WasmModuleBuilder();

  const sig_index = kSig_i_dd;
  builder.addImport('mod', 'fun', sig_index);
  builder.addFunction('main', sig_index)
      .addBody([
        kExprGetLocal, 0,      // --
        kExprGetLocal, 1,      // --
        kExprCallFunction, 0,  // --
      ])                       // --
      .exportFunc();
  return builder;
}

function checkSuccessfulInstantiation(builder, ffi, handler) {
  // Test synchronous instantiation.
  const instance = builder.instantiate(ffi);
  if (handler) handler(instance);

  // Test asynchronous instantiation.
  assertPromiseResult(builder.asyncInstantiate(ffi), handler);
}

function checkFailingInstantiation(builder, ffi, error, message) {
  // Test synchronous instantiation.
  assertThrows(_ => builder.instantiate(ffi), error, message);

  // Test asynchronous instantiation.
  assertThrowsAsync(builder.asyncInstantiate(ffi), error, message);
}

(function testValidFFI() {
  print(arguments.callee.name);
  let ffi = {'mod': {fun: print}};
  checkSuccessfulInstantiation(CreateDefaultBuilder(), ffi, undefined);
})();

(function testInvalidFFIs() {
  print(arguments.callee.name);
  checkFailingInstantiation(
      CreateDefaultBuilder(), 17, TypeError,
      'WebAssembly Instantiation: Argument 1 must be an object');
  checkFailingInstantiation(
      CreateDefaultBuilder(), {}, TypeError,
      'WebAssembly Instantiation: Import #0 module="mod" error: module is not an object or function');
  checkFailingInstantiation(
      CreateDefaultBuilder(), {mod: {}}, WebAssembly.LinkError,
      'WebAssembly Instantiation: Import #0 module="mod" function="fun" error: function import requires a callable');
  checkFailingInstantiation(
      CreateDefaultBuilder(), {mod: {fun: {}}}, WebAssembly.LinkError,
      'WebAssembly Instantiation: Import #0 module="mod" function="fun" error: function import requires a callable');
  checkFailingInstantiation(
      CreateDefaultBuilder(), {mod: {fun: 0}}, WebAssembly.LinkError,
      'WebAssembly Instantiation: Import #0 module="mod" function="fun" error: function import requires a callable');
})();

(function testImportWithInvalidSignature() {
  print(arguments.callee.name);
  // "fun" should have signature "i_dd"
  let builder = new WasmModuleBuilder();

  let sig_index = kSig_i_dd;
  builder.addFunction('exp', kSig_i_i)
      .addBody([
        kExprGetLocal,
        0,
      ])  // --
      .exportFunc();

  let exported = builder.instantiate().exports.exp;
  checkFailingInstantiation(
      CreateDefaultBuilder(), {mod: {fun: exported}}, WebAssembly.LinkError,
      'WebAssembly Instantiation: Import #0 module="mod" function="fun" error: imported function does not match the expected type');
})();

(function regression870646() {
  print(arguments.callee.name);
  const ffi = {mod: {fun: function() {}}};
  Object.defineProperty(ffi, 'mod', {
    get: function() {
      throw new Error('my_exception');
    }
  });

  checkFailingInstantiation(CreateDefaultBuilder(), ffi, Error, 'my_exception');
})();

// "fun" matches signature "i_dd"
(function testImportWithValidSignature() {
  print(arguments.callee.name);
  let builder = new WasmModuleBuilder();

  builder.addFunction('exp', kSig_i_dd)
      .addBody([
        kExprI32Const,
        33,
      ])  // --
      .exportFunc();

  let exported = builder.instantiate().exports.exp;

  checkSuccessfulInstantiation(
      CreateDefaultBuilder(), {mod: {fun: exported}},
      instance => assertEquals(33, instance.exports.main()));
})();

(function I64InSignatureThrows() {
  print(arguments.callee.name);
  let builder = new WasmModuleBuilder();

  builder.addMemory(1, 1, true);
  builder.addFunction('function_with_invalid_signature', kSig_l_ll)
    .addBody([           // --
      kExprGetLocal, 0,  // --
      kExprGetLocal, 1,  // --
      kExprI64Sub])      // --
    .exportFunc()

  checkSuccessfulInstantiation(
      builder, undefined,
      instance => assertThrows(function() {
        instance.exports.function_with_invalid_signature(33, 88);
      }, TypeError, 'wasm function signature contains illegal type'));
})();

(function I64ParamsInSignatureThrows() {
  print(arguments.callee.name);
  let builder = new WasmModuleBuilder();

  builder.addMemory(1, 1, true);
  builder.addFunction('function_with_invalid_signature', kSig_i_l)
      .addBody([kExprGetLocal, 0, kExprI32ConvertI64])
      .exportFunc();

  checkSuccessfulInstantiation(
      builder, undefined,
      instance => assertThrows(
          _ => instance.exports.function_with_invalid_signature(12), TypeError,
          'wasm function signature contains illegal type'));

})();

(function I64JSImportThrows() {
  print(arguments.callee.name);
  let builder = new WasmModuleBuilder();
  let sig_index = builder.addType(kSig_i_i);
  let sig_i64_index = builder.addType(kSig_i_l);
  let index = builder.addImport('', 'func', sig_i64_index);
  builder.addFunction('main', sig_index)
      .addBody([
        kExprGetLocal, 0, kExprI64SConvertI32, kExprCallFunction, index  // --
      ])                                                                 // --
      .exportFunc();

  checkSuccessfulInstantiation(
      builder, {'': {func: _ => {}}},
      instance => assertThrows(
          instance.exports.main, TypeError,
          'wasm function signature contains illegal type'));

})();

(function ImportI64ParamWithF64ReturnThrows() {
  print(arguments.callee.name);
  // This tests that we generate correct code by using the correct return
  // register. See bug 6096.
  let builder = new WasmModuleBuilder();
  builder.addImport('', 'f', makeSig([kWasmI64], [kWasmF64]));
  builder.addFunction('main', kSig_v_v)
      .addBody([kExprI64Const, 0, kExprCallFunction, 0, kExprDrop])
      .exportFunc();

  checkSuccessfulInstantiation(
      builder, {'': {f: i => i}},
      instance => assertThrows(
          instance.exports.main, TypeError,
          'wasm function signature contains illegal type'));

})();

(function ImportI64Return() {
  print(arguments.callee.name);
  // This tests that we generate correct code by using the correct return
  // register(s). See bug 6104.
  let builder = new WasmModuleBuilder();
  builder.addImport('', 'f', makeSig([], [kWasmI64]));
  builder.addFunction('main', kSig_v_v)
      .addBody([kExprCallFunction, 0, kExprDrop])
      .exportFunc();

  checkSuccessfulInstantiation(
      builder, {'': {f: _ => 1}},
      instance => assertThrows(
          instance.exports.main, TypeError,
          'wasm function signature contains illegal type'));

})();

(function ImportSymbolToNumberThrows() {
  print(arguments.callee.name);
  let builder = new WasmModuleBuilder();
  let index = builder.addImport('', 'f', kSig_i_v);
  builder.addFunction('main', kSig_i_v)
      .addBody([kExprCallFunction, 0])
      .exportFunc();

  checkSuccessfulInstantiation(
      builder, {'': {f: _ => Symbol()}},
      instance => assertThrows(
          instance.exports.main, TypeError,
          'Cannot convert a Symbol value to a number'));
})();
