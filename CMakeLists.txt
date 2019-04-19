cmake_minimum_required(VERSION 3.2)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/a621d12df1eb69907193794770fe761e07fe1938.zip"
    SHA1 "60eb7d6313e4448d61dc003797215b2d903d70aa"
)

project(v8 VERSION 7.4.98 LANGUAGES C CXX ASM)

# GN: see 'v8_use_snapshot'
option(V8_USE_SNAPSHOT "..." OFF)

# GN: see 'v8_use_external_startup_data'
option(V8_USE_EXTERNAL_STARTUP_DATA "..." OFF)

option(V8_BUILD_HELLOWORLD "..." ON)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

hunter_add_package(ICU)
find_package(ICU CONFIG REQUIRED)

include(hunter_check_toolchain_definition)

hunter_check_toolchain_definition(
    NAME __aarch64__
    DEFINED arm64_architecture
)

hunter_check_toolchain_definition(
    NAME __arm__
    DEFINED arm_architecture
)

hunter_check_toolchain_definition(
    NAME __amd64__
    DEFINED amd64_architecture
)

# https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2017#microsoft-specific-predefined-macros
hunter_check_toolchain_definition(
    NAME _M_AMD64
    DEFINED m_amd64_architecture
)

if(amd64_architecture OR m_amd64_architecture)
  set(x64_architecture YES)
else()
  set(x64_architecture NO)
endif()

hunter_check_toolchain_definition(
    NAME __i686__
    DEFINED x86_architecture
)

# FIXME {
set(mips_architecture FALSE)
set(mipsel_architecture FALSE)
set(mips64_architecture FALSE)
set(mips64el_architecture FALSE)
set(ppc_architecture FALSE)
set(ppc64_architecture FALSE)
set(s390_architecture FALSE)
set(s390x_architecture FALSE)

if(WIN32)
  set(is_posix FALSE)
else()
  set(is_posix TRUE)
endif()

set(is_fuchsia FALSE)
set(is_aix FALSE)

set(emit_builtins_as_inline_asm FALSE) # WIN32 + Clang

set(v8_enable_embedded_builtins FALSE)
# }

string(COMPARE EQUAL "${CMAKE_SYSTEM_NAME}" "Linux" is_linux)

# See BUILD.gn, v8_base, sources
set(
    v8_base_sources
    include/v8-inspector-protocol.h
    include/v8-inspector.h
    include/v8-platform.h
    include/v8-profiler.h
    include/v8-testing.h
    include/v8-util.h
    include/v8.h
    include/v8config.h
    src/accessors.cc
    src/accessors.h
    src/address-map.cc
    src/address-map.h
    src/allocation-site-scopes.h
    src/allocation.cc
    src/allocation.h
    src/api-arguments-inl.h
    src/api-arguments.h
    src/api-natives.cc
    src/api-natives.h
    src/api.cc
    src/api.h
    src/arguments.cc
    src/arguments.h
    src/asan.h
    src/asmjs/asm-js.cc
    src/asmjs/asm-js.h
    src/asmjs/asm-names.h
    src/asmjs/asm-parser.cc
    src/asmjs/asm-parser.h
    src/asmjs/asm-scanner.cc
    src/asmjs/asm-scanner.h
    src/asmjs/asm-types.cc
    src/asmjs/asm-types.h
    src/asmjs/switch-logic.cc
    src/asmjs/switch-logic.h
    src/assembler-arch.h
    src/assembler-inl.h
    src/assembler.cc
    src/assembler.h
    src/assert-scope.cc
    src/assert-scope.h
    src/ast/ast-function-literal-id-reindexer.cc
    src/ast/ast-function-literal-id-reindexer.h
    src/ast/ast-source-ranges.h
    src/ast/ast-traversal-visitor.h
    src/ast/ast-value-factory.cc
    src/ast/ast-value-factory.h
    src/ast/ast.cc
    src/ast/ast.h
    src/ast/compile-time-value.cc
    src/ast/compile-time-value.h
    src/ast/context-slot-cache.cc
    src/ast/context-slot-cache.h
    src/ast/modules.cc
    src/ast/modules.h
    src/ast/prettyprinter.cc
    src/ast/prettyprinter.h
    src/ast/scopes.cc
    src/ast/scopes.h
    src/ast/variables.cc
    src/ast/variables.h
    src/bailout-reason.cc
    src/bailout-reason.h
    src/basic-block-profiler.cc
    src/basic-block-profiler.h
    src/bignum-dtoa.cc
    src/bignum-dtoa.h
    src/bignum.cc
    src/bignum.h
    src/bit-vector.cc
    src/bit-vector.h
    src/bootstrapper.cc
    src/bootstrapper.h
    src/boxed-float.h
    src/builtins/builtins-api.cc
    src/builtins/builtins-array.cc
    src/builtins/builtins-arraybuffer.cc
    src/builtins/builtins-bigint.cc
    src/builtins/builtins-boolean.cc
    src/builtins/builtins-call.cc
    src/builtins/builtins-callsite.cc
    src/builtins/builtins-collections.cc
    src/builtins/builtins-console.cc
    src/builtins/builtins-constructor.h
    src/builtins/builtins-dataview.cc
    src/builtins/builtins-date.cc
    src/builtins/builtins-definitions.h
    src/builtins/builtins-descriptors.h
    src/builtins/builtins-error.cc
    src/builtins/builtins-function.cc
    src/builtins/builtins-global.cc
    src/builtins/builtins-internal.cc
    src/builtins/builtins-interpreter.cc
    src/builtins/builtins-intl.cc
    src/builtins/builtins-intl.h
    src/builtins/builtins-json.cc
    src/builtins/builtins-math.cc
    src/builtins/builtins-number.cc
    src/builtins/builtins-object.cc
    src/builtins/builtins-promise.cc
    src/builtins/builtins-reflect.cc
    src/builtins/builtins-regexp.cc
    src/builtins/builtins-sharedarraybuffer.cc
    src/builtins/builtins-string.cc
    src/builtins/builtins-symbol.cc
    src/builtins/builtins-typed-array.cc
    src/builtins/builtins-utils.h
    src/builtins/builtins.cc
    src/builtins/builtins.h
    src/builtins/constants-table-builder.cc
    src/builtins/constants-table-builder.h
    src/cached-powers.cc
    src/cached-powers.h
    src/callable.h
    src/cancelable-task.cc
    src/cancelable-task.h
    src/char-predicates-inl.h
    src/char-predicates.cc
    src/char-predicates.h
    src/checks.h
    src/code-events.h
    src/code-factory.cc
    src/code-factory.h
    src/code-reference.cc
    src/code-reference.h
    src/code-stub-assembler.cc
    src/code-stub-assembler.h
    src/code-stubs-utils.h
    src/code-stubs.cc
    src/code-stubs.h
    src/codegen.cc
    src/codegen.h
    src/collector.h
    src/compilation-cache.cc
    src/compilation-cache.h
    src/compilation-dependencies.cc
    src/compilation-dependencies.h
    src/compilation-statistics.cc
    src/compilation-statistics.h
    src/compiler-dispatcher/compiler-dispatcher-job.cc
    src/compiler-dispatcher/compiler-dispatcher-job.h
    src/compiler-dispatcher/compiler-dispatcher-tracer.cc
    src/compiler-dispatcher/compiler-dispatcher-tracer.h
    src/compiler-dispatcher/compiler-dispatcher.cc
    src/compiler-dispatcher/compiler-dispatcher.h
    src/compiler-dispatcher/optimizing-compile-dispatcher.cc
    src/compiler-dispatcher/optimizing-compile-dispatcher.h
    src/compiler-dispatcher/unoptimized-compile-job.cc
    src/compiler-dispatcher/unoptimized-compile-job.h
    src/compiler.cc
    src/compiler.h
    src/compiler/access-builder.cc
    src/compiler/access-builder.h
    src/compiler/access-info.cc
    src/compiler/access-info.h
    src/compiler/all-nodes.cc
    src/compiler/all-nodes.h
    src/compiler/allocation-builder.h
    src/compiler/basic-block-instrumentor.cc
    src/compiler/basic-block-instrumentor.h
    src/compiler/branch-elimination.cc
    src/compiler/branch-elimination.h
    src/compiler/bytecode-analysis.cc
    src/compiler/bytecode-analysis.h
    src/compiler/bytecode-graph-builder.cc
    src/compiler/bytecode-graph-builder.h
    src/compiler/bytecode-liveness-map.cc
    src/compiler/bytecode-liveness-map.h
    src/compiler/c-linkage.cc
    src/compiler/checkpoint-elimination.cc
    src/compiler/checkpoint-elimination.h
    src/compiler/code-assembler.cc
    src/compiler/code-assembler.h
    src/compiler/code-generator-impl.h
    src/compiler/code-generator.cc
    src/compiler/code-generator.h
    src/compiler/common-node-cache.cc
    src/compiler/common-node-cache.h
    src/compiler/common-operator-reducer.cc
    src/compiler/common-operator-reducer.h
    src/compiler/common-operator.cc
    src/compiler/common-operator.h
    src/compiler/compiler-source-position-table.cc
    src/compiler/compiler-source-position-table.h
    src/compiler/constant-folding-reducer.cc
    src/compiler/constant-folding-reducer.h
    src/compiler/control-equivalence.cc
    src/compiler/control-equivalence.h
    src/compiler/control-flow-optimizer.cc
    src/compiler/control-flow-optimizer.h
    src/compiler/dead-code-elimination.cc
    src/compiler/dead-code-elimination.h
    src/compiler/diamond.h
    src/compiler/effect-control-linearizer.cc
    src/compiler/effect-control-linearizer.h
    src/compiler/escape-analysis-reducer.cc
    src/compiler/escape-analysis-reducer.h
    src/compiler/escape-analysis.cc
    src/compiler/escape-analysis.h
    src/compiler/frame-elider.cc
    src/compiler/frame-elider.h
    src/compiler/frame-states.cc
    src/compiler/frame-states.h
    src/compiler/frame.cc
    src/compiler/frame.h
    src/compiler/functional-list.h
    src/compiler/gap-resolver.cc
    src/compiler/gap-resolver.h
    src/compiler/graph-assembler.cc
    src/compiler/graph-assembler.h
    src/compiler/graph-reducer.cc
    src/compiler/graph-reducer.h
    src/compiler/graph-trimmer.cc
    src/compiler/graph-trimmer.h
    src/compiler/graph-visualizer.cc
    src/compiler/graph-visualizer.h
    src/compiler/graph.cc
    src/compiler/graph.h
    src/compiler/instruction-codes.h
    src/compiler/instruction-scheduler.cc
    src/compiler/instruction-scheduler.h
    src/compiler/instruction-selector-impl.h
    src/compiler/instruction-selector.cc
    src/compiler/instruction-selector.h
    src/compiler/instruction.cc
    src/compiler/instruction.h
    src/compiler/int64-lowering.cc
    src/compiler/int64-lowering.h
    src/compiler/js-call-reducer.cc
    src/compiler/js-call-reducer.h
    src/compiler/js-context-specialization.cc
    src/compiler/js-context-specialization.h
    src/compiler/js-create-lowering.cc
    src/compiler/js-create-lowering.h
    src/compiler/js-generic-lowering.cc
    src/compiler/js-generic-lowering.h
    src/compiler/js-graph.cc
    src/compiler/js-graph.h
    src/compiler/js-heap-broker.cc
    src/compiler/js-heap-broker.h
    src/compiler/js-inlining-heuristic.cc
    src/compiler/js-inlining-heuristic.h
    src/compiler/js-inlining.cc
    src/compiler/js-inlining.h
    src/compiler/js-intrinsic-lowering.cc
    src/compiler/js-intrinsic-lowering.h
    src/compiler/js-native-context-specialization.cc
    src/compiler/js-native-context-specialization.h
    src/compiler/js-operator.cc
    src/compiler/js-operator.h
    src/compiler/js-type-hint-lowering.cc
    src/compiler/js-type-hint-lowering.h
    src/compiler/js-typed-lowering.cc
    src/compiler/js-typed-lowering.h
    src/compiler/jump-threading.cc
    src/compiler/jump-threading.h
    src/compiler/linkage.cc
    src/compiler/linkage.h
    src/compiler/live-range-separator.cc
    src/compiler/live-range-separator.h
    src/compiler/load-elimination.cc
    src/compiler/load-elimination.h
    src/compiler/loop-analysis.cc
    src/compiler/loop-analysis.h
    src/compiler/loop-peeling.cc
    src/compiler/loop-peeling.h
    src/compiler/loop-variable-optimizer.cc
    src/compiler/loop-variable-optimizer.h
    src/compiler/machine-graph-verifier.cc
    src/compiler/machine-graph-verifier.h
    src/compiler/machine-graph.cc
    src/compiler/machine-graph.h
    src/compiler/machine-operator-reducer.cc
    src/compiler/machine-operator-reducer.h
    src/compiler/machine-operator.cc
    src/compiler/machine-operator.h
    src/compiler/memory-optimizer.cc
    src/compiler/memory-optimizer.h
    src/compiler/move-optimizer.cc
    src/compiler/move-optimizer.h
    src/compiler/node-aux-data.h
    src/compiler/node-cache.cc
    src/compiler/node-cache.h
    src/compiler/node-marker.cc
    src/compiler/node-marker.h
    src/compiler/node-matchers.cc
    src/compiler/node-matchers.h
    src/compiler/node-origin-table.cc
    src/compiler/node-origin-table.h
    src/compiler/node-properties.cc
    src/compiler/node-properties.h
    src/compiler/node.cc
    src/compiler/node.h
    src/compiler/opcodes.cc
    src/compiler/opcodes.h
    src/compiler/operation-typer.cc
    src/compiler/operation-typer.h
    src/compiler/operator-properties.cc
    src/compiler/operator-properties.h
    src/compiler/operator.cc
    src/compiler/operator.h
    src/compiler/osr.cc
    src/compiler/osr.h
    src/compiler/persistent-map.h
    src/compiler/pipeline-statistics.cc
    src/compiler/pipeline-statistics.h
    src/compiler/pipeline.cc
    src/compiler/pipeline.h
    src/compiler/property-access-builder.cc
    src/compiler/property-access-builder.h
    src/compiler/raw-machine-assembler.cc
    src/compiler/raw-machine-assembler.h
    src/compiler/redundancy-elimination.cc
    src/compiler/redundancy-elimination.h
    src/compiler/register-allocator-verifier.cc
    src/compiler/register-allocator-verifier.h
    src/compiler/register-allocator.cc
    src/compiler/register-allocator.h
    src/compiler/representation-change.cc
    src/compiler/representation-change.h
    src/compiler/schedule.cc
    src/compiler/schedule.h
    src/compiler/scheduler.cc
    src/compiler/scheduler.h
    src/compiler/select-lowering.cc
    src/compiler/select-lowering.h
    src/compiler/simd-scalar-lowering.cc
    src/compiler/simd-scalar-lowering.h
    src/compiler/simplified-lowering.cc
    src/compiler/simplified-lowering.h
    src/compiler/simplified-operator-reducer.cc
    src/compiler/simplified-operator-reducer.h
    src/compiler/simplified-operator.cc
    src/compiler/simplified-operator.h
    src/compiler/state-values-utils.cc
    src/compiler/state-values-utils.h
    src/compiler/store-store-elimination.cc
    src/compiler/store-store-elimination.h
    src/compiler/type-cache.cc
    src/compiler/type-cache.h
    src/compiler/type-narrowing-reducer.cc
    src/compiler/type-narrowing-reducer.h
    src/compiler/typed-optimization.cc
    src/compiler/typed-optimization.h
    src/compiler/typer.cc
    src/compiler/typer.h
    src/compiler/types.cc
    src/compiler/types.h
    src/compiler/unwinding-info-writer.h
    src/compiler/value-numbering-reducer.cc
    src/compiler/value-numbering-reducer.h
    src/compiler/verifier.cc
    src/compiler/verifier.h
    src/compiler/wasm-compiler.cc
    src/compiler/wasm-compiler.h
    src/compiler/zone-stats.cc
    src/compiler/zone-stats.h
    src/contexts-inl.h
    src/contexts.cc
    src/contexts.h
    src/conversions-inl.h
    src/conversions.cc
    src/conversions.h
    src/counters-inl.h
    src/counters.cc
    src/counters.h
    src/date.cc
    src/date.h
    src/dateparser-inl.h
    src/dateparser.cc
    src/dateparser.h
    src/debug/debug-coverage.cc
    src/debug/debug-coverage.h
    src/debug/debug-evaluate.cc
    src/debug/debug-evaluate.h
    src/debug/debug-frames.cc
    src/debug/debug-frames.h
    src/debug/debug-interface.h
    src/debug/debug-scope-iterator.cc
    src/debug/debug-scope-iterator.h
    src/debug/debug-scopes.cc
    src/debug/debug-scopes.h
    src/debug/debug-stack-trace-iterator.cc
    src/debug/debug-stack-trace-iterator.h
    src/debug/debug-type-profile.cc
    src/debug/debug-type-profile.h
    src/debug/debug.cc
    src/debug/debug.h
    src/debug/interface-types.h
    src/debug/liveedit.cc
    src/debug/liveedit.h
    src/deoptimize-reason.cc
    src/deoptimize-reason.h
    src/deoptimizer.cc
    src/deoptimizer.h
    src/detachable-vector.h
    src/disasm.h
    src/disassembler.cc
    src/disassembler.h
    src/diy-fp.cc
    src/diy-fp.h
    src/double.h
    src/dtoa.cc
    src/dtoa.h
    src/eh-frame.cc
    src/eh-frame.h
    src/elements-kind.cc
    src/elements-kind.h
    src/elements.cc
    src/elements.h
    src/execution.cc
    src/execution.h
    src/extensions/externalize-string-extension.cc
    src/extensions/externalize-string-extension.h
    src/extensions/free-buffer-extension.cc
    src/extensions/free-buffer-extension.h
    src/extensions/gc-extension.cc
    src/extensions/gc-extension.h
    src/extensions/ignition-statistics-extension.cc
    src/extensions/ignition-statistics-extension.h
    src/extensions/statistics-extension.cc
    src/extensions/statistics-extension.h
    src/extensions/trigger-failure-extension.cc
    src/extensions/trigger-failure-extension.h
    src/external-reference-table.cc
    src/external-reference-table.h
    src/external-reference.cc
    src/external-reference.h
    src/fast-dtoa.cc
    src/fast-dtoa.h
    src/feedback-vector-inl.h
    src/feedback-vector.cc
    src/feedback-vector.h
    src/field-index-inl.h
    src/field-index.h
    src/field-type.cc
    src/field-type.h
    src/fixed-dtoa.cc
    src/fixed-dtoa.h
    src/flag-definitions.h
    src/flags.cc
    src/flags.h
    src/frame-constants.h
    src/frames-inl.h
    src/frames.cc
    src/frames.h
    src/futex-emulation.cc
    src/futex-emulation.h
    src/gdb-jit.cc
    src/gdb-jit.h
    src/global-handles.cc
    src/global-handles.h
    src/globals.h
    src/handler-table.cc
    src/handler-table.h
    src/handles-inl.h
    src/handles.cc
    src/handles.h
    src/heap-symbols.h
    src/heap/array-buffer-collector.cc
    src/heap/array-buffer-collector.h
    src/heap/array-buffer-tracker-inl.h
    src/heap/array-buffer-tracker.cc
    src/heap/array-buffer-tracker.h
    src/heap/barrier.h
    src/heap/code-stats.cc
    src/heap/code-stats.h
    src/heap/concurrent-marking.cc
    src/heap/concurrent-marking.h
    src/heap/embedder-tracing.cc
    src/heap/embedder-tracing.h
    src/heap/factory-inl.h
    src/heap/factory.cc
    src/heap/factory.h
    src/heap/gc-idle-time-handler.cc
    src/heap/gc-idle-time-handler.h
    src/heap/gc-tracer.cc
    src/heap/gc-tracer.h
    src/heap/heap-controller.cc
    src/heap/heap-controller.h
    src/heap/heap-inl.h
    src/heap/heap.cc
    src/heap/heap.h
    src/heap/incremental-marking-inl.h
    src/heap/incremental-marking-job.cc
    src/heap/incremental-marking-job.h
    src/heap/incremental-marking.cc
    src/heap/incremental-marking.h
    src/heap/invalidated-slots-inl.h
    src/heap/invalidated-slots.cc
    src/heap/invalidated-slots.h
    src/heap/item-parallel-job.cc
    src/heap/item-parallel-job.h
    src/heap/local-allocator.h
    src/heap/mark-compact-inl.h
    src/heap/mark-compact.cc
    src/heap/mark-compact.h
    src/heap/marking.cc
    src/heap/marking.h
    src/heap/memory-reducer.cc
    src/heap/memory-reducer.h
    src/heap/object-stats.cc
    src/heap/object-stats.h
    src/heap/objects-visiting-inl.h
    src/heap/objects-visiting.cc
    src/heap/objects-visiting.h
    src/heap/remembered-set.h
    src/heap/scavenge-job.cc
    src/heap/scavenge-job.h
    src/heap/scavenger-inl.h
    src/heap/scavenger.cc
    src/heap/scavenger.h
    src/heap/slot-set.h
    src/heap/spaces-inl.h
    src/heap/spaces.cc
    src/heap/spaces.h
    src/heap/store-buffer.cc
    src/heap/store-buffer.h
    src/heap/stress-marking-observer.cc
    src/heap/stress-marking-observer.h
    src/heap/stress-scavenge-observer.cc
    src/heap/stress-scavenge-observer.h
    src/heap/sweeper.cc
    src/heap/sweeper.h
    src/heap/worklist.h
    src/ic/call-optimization.cc
    src/ic/call-optimization.h
    src/ic/handler-configuration-inl.h
    src/ic/handler-configuration.cc
    src/ic/handler-configuration.h
    src/ic/ic-inl.h
    src/ic/ic-stats.cc
    src/ic/ic-stats.h
    src/ic/ic.cc
    src/ic/ic.h
    src/ic/stub-cache.cc
    src/ic/stub-cache.h
    src/icu_util.cc
    src/icu_util.h
    src/identity-map.cc
    src/identity-map.h
    src/instruction-stream.cc
    src/instruction-stream.h
    src/interface-descriptors.cc
    src/interface-descriptors.h
    src/interpreter/block-coverage-builder.h
    src/interpreter/bytecode-array-accessor.cc
    src/interpreter/bytecode-array-accessor.h
    src/interpreter/bytecode-array-builder.cc
    src/interpreter/bytecode-array-builder.h
    src/interpreter/bytecode-array-iterator.cc
    src/interpreter/bytecode-array-iterator.h
    src/interpreter/bytecode-array-random-iterator.cc
    src/interpreter/bytecode-array-random-iterator.h
    src/interpreter/bytecode-array-writer.cc
    src/interpreter/bytecode-array-writer.h
    src/interpreter/bytecode-decoder.cc
    src/interpreter/bytecode-decoder.h
    src/interpreter/bytecode-flags.cc
    src/interpreter/bytecode-flags.h
    src/interpreter/bytecode-generator.cc
    src/interpreter/bytecode-generator.h
    src/interpreter/bytecode-jump-table.h
    src/interpreter/bytecode-label.cc
    src/interpreter/bytecode-label.h
    src/interpreter/bytecode-node.cc
    src/interpreter/bytecode-node.h
    src/interpreter/bytecode-operands.cc
    src/interpreter/bytecode-operands.h
    src/interpreter/bytecode-register-allocator.h
    src/interpreter/bytecode-register-optimizer.cc
    src/interpreter/bytecode-register-optimizer.h
    src/interpreter/bytecode-register.cc
    src/interpreter/bytecode-register.h
    src/interpreter/bytecode-source-info.cc
    src/interpreter/bytecode-source-info.h
    src/interpreter/bytecode-traits.h
    src/interpreter/bytecodes.cc
    src/interpreter/bytecodes.h
    src/interpreter/constant-array-builder.cc
    src/interpreter/constant-array-builder.h
    src/interpreter/control-flow-builders.cc
    src/interpreter/control-flow-builders.h
    src/interpreter/handler-table-builder.cc
    src/interpreter/handler-table-builder.h
    src/interpreter/interpreter-generator.h
    src/interpreter/interpreter-intrinsics.cc
    src/interpreter/interpreter-intrinsics.h
    src/interpreter/interpreter.cc
    src/interpreter/interpreter.h
    src/intl.cc
    src/intl.h
    src/isolate-inl.h
    src/isolate.cc
    src/isolate.h
    src/json-parser.cc
    src/json-parser.h
    src/json-stringifier.cc
    src/json-stringifier.h
    src/keys.cc
    src/keys.h
    src/label.h
    src/layout-descriptor-inl.h
    src/layout-descriptor.cc
    src/layout-descriptor.h
    src/locked-queue-inl.h
    src/locked-queue.h
    src/log-inl.h
    src/log-utils.cc
    src/log-utils.h
    src/log.cc
    src/log.h
    src/lookup-cache-inl.h
    src/lookup-cache.cc
    src/lookup-cache.h
    src/lookup.cc
    src/lookup.h
    src/lsan.h
    src/machine-type.cc
    src/machine-type.h
    src/macro-assembler-inl.h
    src/macro-assembler.h
    src/map-updater.cc
    src/map-updater.h
    src/messages.cc
    src/messages.h
    src/msan.h
    src/objects-body-descriptors-inl.h
    src/objects-body-descriptors.h
    src/objects-debug.cc
    src/objects-inl.h
    src/objects-printer.cc
    src/objects.cc
    src/objects.h
    src/objects/api-callbacks-inl.h
    src/objects/api-callbacks.h
    src/objects/arguments-inl.h
    src/objects/arguments.h
    src/objects/bigint.cc
    src/objects/bigint.h
    src/objects/code-inl.h
    src/objects/code.h
    src/objects/compilation-cache-inl.h
    src/objects/compilation-cache.h
    src/objects/debug-objects-inl.h
    src/objects/debug-objects.cc
    src/objects/debug-objects.h
    src/objects/descriptor-array.h
    src/objects/dictionary.h
    src/objects/fixed-array-inl.h
    src/objects/fixed-array.h
    src/objects/frame-array-inl.h
    src/objects/frame-array.h
    src/objects/hash-table-inl.h
    src/objects/hash-table.h
    src/objects/intl-objects.cc
    src/objects/intl-objects.h
    src/objects/js-array-inl.h
    src/objects/js-array.h
    src/objects/js-collection-inl.h
    src/objects/js-collection.h
    src/objects/js-locale-inl.h
    src/objects/js-locale.cc
    src/objects/js-locale.h
    src/objects/js-promise-inl.h
    src/objects/js-promise.h
    src/objects/js-regexp-inl.h
    src/objects/js-regexp-string-iterator-inl.h
    src/objects/js-regexp-string-iterator.h
    src/objects/js-regexp.h
    src/objects/literal-objects-inl.h
    src/objects/literal-objects.cc
    src/objects/literal-objects.h
    src/objects/managed.cc
    src/objects/managed.h
    src/objects/map-inl.h
    src/objects/map.h
    src/objects/maybe-object-inl.h
    src/objects/maybe-object.h
    src/objects/microtask-inl.h
    src/objects/microtask.h
    src/objects/module-inl.h
    src/objects/module.cc
    src/objects/module.h
    src/objects/name-inl.h
    src/objects/name.h
    src/objects/object-macros-undef.h
    src/objects/object-macros.h
    src/objects/ordered-hash-table-inl.h
    src/objects/ordered-hash-table.cc
    src/objects/ordered-hash-table.h
    src/objects/promise-inl.h
    src/objects/promise.h
    src/objects/property-descriptor-object-inl.h
    src/objects/property-descriptor-object.h
    src/objects/regexp-match-info.h
    src/objects/scope-info.cc
    src/objects/scope-info.h
    src/objects/script-inl.h
    src/objects/script.h
    src/objects/shared-function-info-inl.h
    src/objects/shared-function-info.h
    src/objects/string-inl.h
    src/objects/string-table.h
    src/objects/string.h
    src/objects/template-objects.cc
    src/objects/template-objects.h
    src/objects/templates-inl.h
    src/objects/templates.h
    src/optimized-compilation-info.cc
    src/optimized-compilation-info.h
    src/ostreams.cc
    src/ostreams.h
    src/parsing/duplicate-finder.h
    src/parsing/expression-classifier.h
    src/parsing/expression-scope-reparenter.cc
    src/parsing/expression-scope-reparenter.h
    src/parsing/func-name-inferrer.cc
    src/parsing/func-name-inferrer.h
    src/parsing/parse-info.cc
    src/parsing/parse-info.h
    src/parsing/parser-base.h
    src/parsing/parser.cc
    src/parsing/parser.h
    src/parsing/parsing.cc
    src/parsing/parsing.h
    src/parsing/pattern-rewriter.cc
    src/parsing/preparse-data.cc
    src/parsing/preparse-data.h
    src/parsing/preparsed-scope-data.cc
    src/parsing/preparsed-scope-data.h
    src/parsing/preparser.cc
    src/parsing/preparser.h
    src/parsing/rewriter.cc
    src/parsing/rewriter.h
    src/parsing/scanner-character-streams.cc
    src/parsing/scanner-character-streams.h
    src/parsing/scanner.cc
    src/parsing/scanner.h
    src/parsing/token.cc
    src/parsing/token.h
    src/pending-compilation-error-handler.cc
    src/pending-compilation-error-handler.h
    src/perf-jit.cc
    src/perf-jit.h
    src/profiler/allocation-tracker.cc
    src/profiler/allocation-tracker.h
    src/profiler/circular-queue-inl.h
    src/profiler/circular-queue.h
    src/profiler/cpu-profiler-inl.h
    src/profiler/cpu-profiler.cc
    src/profiler/cpu-profiler.h
    src/profiler/heap-profiler.cc
    src/profiler/heap-profiler.h
    src/profiler/heap-snapshot-generator-inl.h
    src/profiler/heap-snapshot-generator.cc
    src/profiler/heap-snapshot-generator.h
    src/profiler/profile-generator-inl.h
    src/profiler/profile-generator.cc
    src/profiler/profile-generator.h
    src/profiler/profiler-listener.cc
    src/profiler/profiler-listener.h
    src/profiler/sampling-heap-profiler.cc
    src/profiler/sampling-heap-profiler.h
    src/profiler/strings-storage.cc
    src/profiler/strings-storage.h
    src/profiler/tick-sample.cc
    src/profiler/tick-sample.h
    src/profiler/tracing-cpu-profiler.cc
    src/profiler/tracing-cpu-profiler.h
    src/profiler/unbound-queue-inl.h
    src/profiler/unbound-queue.h
    src/property-descriptor.cc
    src/property-descriptor.h
    src/property-details.h
    src/property.cc
    src/property.h
    src/prototype.h
    src/regexp/bytecodes-irregexp.h
    src/regexp/interpreter-irregexp.cc
    src/regexp/interpreter-irregexp.h
    src/regexp/jsregexp-inl.h
    src/regexp/jsregexp.cc
    src/regexp/jsregexp.h
    src/regexp/regexp-ast.cc
    src/regexp/regexp-ast.h
    src/regexp/regexp-macro-assembler-irregexp-inl.h
    src/regexp/regexp-macro-assembler-irregexp.cc
    src/regexp/regexp-macro-assembler-irregexp.h
    src/regexp/regexp-macro-assembler-tracer.cc
    src/regexp/regexp-macro-assembler-tracer.h
    src/regexp/regexp-macro-assembler.cc
    src/regexp/regexp-macro-assembler.h
    src/regexp/regexp-parser.cc
    src/regexp/regexp-parser.h
    src/regexp/regexp-stack.cc
    src/regexp/regexp-stack.h
    src/regexp/regexp-utils.cc
    src/regexp/regexp-utils.h
    src/register-configuration.cc
    src/register-configuration.h
    src/reglist.h
    src/runtime-profiler.cc
    src/runtime-profiler.h
    src/runtime/runtime-array.cc
    src/runtime/runtime-atomics.cc
    src/runtime/runtime-bigint.cc
    src/runtime/runtime-classes.cc
    src/runtime/runtime-collections.cc
    src/runtime/runtime-compiler.cc
    src/runtime/runtime-date.cc
    src/runtime/runtime-debug.cc
    src/runtime/runtime-forin.cc
    src/runtime/runtime-function.cc
    src/runtime/runtime-futex.cc
    src/runtime/runtime-generator.cc
    src/runtime/runtime-internal.cc
    src/runtime/runtime-interpreter.cc
    src/runtime/runtime-intl.cc
    src/runtime/runtime-literals.cc
    src/runtime/runtime-liveedit.cc
    src/runtime/runtime-maths.cc
    src/runtime/runtime-module.cc
    src/runtime/runtime-numbers.cc
    src/runtime/runtime-object.cc
    src/runtime/runtime-operators.cc
    src/runtime/runtime-promise.cc
    src/runtime/runtime-proxy.cc
    src/runtime/runtime-regexp.cc
    src/runtime/runtime-scopes.cc
    src/runtime/runtime-strings.cc
    src/runtime/runtime-symbol.cc
    src/runtime/runtime-test.cc
    src/runtime/runtime-typedarray.cc
    src/runtime/runtime-utils.h
    src/runtime/runtime-wasm.cc
    src/runtime/runtime.cc
    src/runtime/runtime.h
    src/safepoint-table.cc
    src/safepoint-table.h
    src/setup-isolate.h
    src/signature.h
    src/simulator-base.cc
    src/simulator-base.h
    src/simulator.h
    src/snapshot/builtin-deserializer-allocator.cc
    src/snapshot/builtin-deserializer-allocator.h
    src/snapshot/builtin-deserializer.cc
    src/snapshot/builtin-deserializer.h
    src/snapshot/builtin-serializer-allocator.cc
    src/snapshot/builtin-serializer-allocator.h
    src/snapshot/builtin-serializer.cc
    src/snapshot/builtin-serializer.h
    src/snapshot/builtin-snapshot-utils.cc
    src/snapshot/builtin-snapshot-utils.h
    src/snapshot/code-serializer.cc
    src/snapshot/code-serializer.h
    src/snapshot/default-deserializer-allocator.cc
    src/snapshot/default-deserializer-allocator.h
    src/snapshot/default-serializer-allocator.cc
    src/snapshot/default-serializer-allocator.h
    src/snapshot/deserializer.cc
    src/snapshot/deserializer.h
    src/snapshot/macros.h
    src/snapshot/natives-common.cc
    src/snapshot/natives.h
    src/snapshot/object-deserializer.cc
    src/snapshot/object-deserializer.h
    src/snapshot/partial-deserializer.cc
    src/snapshot/partial-deserializer.h
    src/snapshot/partial-serializer.cc
    src/snapshot/partial-serializer.h
    src/snapshot/serializer-common.cc
    src/snapshot/serializer-common.h
    src/snapshot/serializer.cc
    src/snapshot/serializer.h
    src/snapshot/snapshot-common.cc
    src/snapshot/snapshot-source-sink.cc
    src/snapshot/snapshot-source-sink.h
    src/snapshot/snapshot.h
    src/snapshot/startup-deserializer.cc
    src/snapshot/startup-deserializer.h
    src/snapshot/startup-serializer.cc
    src/snapshot/startup-serializer.h
    src/source-position-table.cc
    src/source-position-table.h
    src/source-position.cc
    src/source-position.h
    src/splay-tree-inl.h
    src/splay-tree.h
    src/startup-data-util.cc
    src/startup-data-util.h
    src/string-builder.cc
    src/string-builder.h
    src/string-case.cc
    src/string-case.h
    src/string-hasher-inl.h
    src/string-hasher.h
    src/string-search.h
    src/string-stream.cc
    src/string-stream.h
    src/strtod.cc
    src/strtod.h
    src/third_party/utf8-decoder/utf8-decoder.h
    src/tracing/trace-event.cc
    src/tracing/trace-event.h
    src/tracing/traced-value.cc
    src/tracing/traced-value.h
    src/tracing/tracing-category-observer.cc
    src/tracing/tracing-category-observer.h
    src/transitions-inl.h
    src/transitions.cc
    src/transitions.h
    src/trap-handler/handler-inside.cc
    src/trap-handler/handler-outside.cc
    src/trap-handler/handler-shared.cc
    src/trap-handler/trap-handler-internal.h
    src/trap-handler/trap-handler.h
    src/turbo-assembler.cc
    src/turbo-assembler.h
    src/type-hints.cc
    src/type-hints.h
    src/unicode-cache-inl.h
    src/unicode-cache.h
    src/unicode-decoder.cc
    src/unicode-decoder.h
    src/unicode-inl.h
    src/unicode.cc
    src/unicode.h
    src/unoptimized-compilation-info.cc
    src/unoptimized-compilation-info.h
    src/uri.cc
    src/uri.h
    src/utils-inl.h
    src/utils.cc
    src/utils.h
    src/v8.cc
    src/v8.h
    src/v8memory.h
    src/v8threads.cc
    src/v8threads.h
    src/value-serializer.cc
    src/value-serializer.h
    src/vector-slot-pair.cc
    src/vector-slot-pair.h
    src/vector.h
    src/version.cc
    src/version.h
    src/visitors.cc
    src/visitors.h
    src/vm-state-inl.h
    src/vm-state.h
    src/wasm/baseline/liftoff-assembler-defs.h
    src/wasm/baseline/liftoff-assembler.cc
    src/wasm/baseline/liftoff-assembler.h
    src/wasm/baseline/liftoff-compiler.cc
    src/wasm/baseline/liftoff-compiler.h
    src/wasm/baseline/liftoff-register.h
    src/wasm/decoder.h
    src/wasm/function-body-decoder-impl.h
    src/wasm/function-body-decoder.cc
    src/wasm/function-body-decoder.h
    src/wasm/function-compiler.cc
    src/wasm/function-compiler.h
    src/wasm/jump-table-assembler.cc
    src/wasm/jump-table-assembler.h
    src/wasm/leb-helper.h
    src/wasm/local-decl-encoder.cc
    src/wasm/local-decl-encoder.h
    src/wasm/memory-tracing.cc
    src/wasm/memory-tracing.h
    src/wasm/module-compiler.cc
    src/wasm/module-compiler.h
    src/wasm/module-decoder.cc
    src/wasm/module-decoder.h
    src/wasm/signature-map.cc
    src/wasm/signature-map.h
    src/wasm/streaming-decoder.cc
    src/wasm/streaming-decoder.h
    src/wasm/value-type.h
    src/wasm/wasm-code-manager.cc
    src/wasm/wasm-code-manager.h
    src/wasm/wasm-constants.h
    src/wasm/wasm-debug.cc
    src/wasm/wasm-engine.cc
    src/wasm/wasm-engine.h
    src/wasm/wasm-external-refs.cc
    src/wasm/wasm-external-refs.h
    src/wasm/wasm-interpreter.cc
    src/wasm/wasm-interpreter.h
    src/wasm/wasm-js.cc
    src/wasm/wasm-js.h
    src/wasm/wasm-limits.h
    src/wasm/wasm-linkage.h
    src/wasm/wasm-memory.cc
    src/wasm/wasm-memory.h
    src/wasm/wasm-module-builder.cc
    src/wasm/wasm-module-builder.h
    src/wasm/wasm-module.cc
    src/wasm/wasm-module.h
    src/wasm/wasm-objects-inl.h
    src/wasm/wasm-objects.cc
    src/wasm/wasm-objects.h
    src/wasm/wasm-opcodes.cc
    src/wasm/wasm-opcodes.h
    src/wasm/wasm-result.cc
    src/wasm/wasm-result.h
    src/wasm/wasm-serialization.cc
    src/wasm/wasm-serialization.h
    src/wasm/wasm-text.cc
    src/wasm/wasm-text.h
    src/wasm/wasm-value.h
    src/zone/accounting-allocator.cc
    src/zone/accounting-allocator.h
    src/zone/zone-allocator.h
    src/zone/zone-chunk-list.h
    src/zone/zone-containers.h
    src/zone/zone-handle-set.h
    src/zone/zone-list-inl.h
    src/zone/zone-segment.cc
    src/zone/zone-segment.h
    src/zone/zone.cc
    src/zone/zone.h
)

if(x86_architecture)
  list(
      APPEND
      v8_base_sources
      src/compiler/backend/ia32/code-generator-ia32.cc
      src/compiler/backend/ia32/instruction-codes-ia32.h
      src/compiler/backend/ia32/instruction-scheduler-ia32.cc
      src/compiler/backend/ia32/instruction-selector-ia32.cc
      src/debug/ia32/debug-ia32.cc
      src/ia32/assembler-ia32-inl.h
      src/ia32/assembler-ia32.cc
      src/ia32/assembler-ia32.h
      src/ia32/constants-ia32.h
      src/ia32/cpu-ia32.cc
      src/ia32/deoptimizer-ia32.cc
      src/ia32/disasm-ia32.cc
      src/ia32/frame-constants-ia32.cc
      src/ia32/frame-constants-ia32.h
      src/ia32/interface-descriptors-ia32.cc
      src/ia32/macro-assembler-ia32.cc
      src/ia32/macro-assembler-ia32.h
      src/ia32/register-ia32.h
      src/ia32/sse-instr.h
      src/regexp/ia32/regexp-macro-assembler-ia32.cc
      src/regexp/ia32/regexp-macro-assembler-ia32.h
      src/wasm/baseline/ia32/liftoff-assembler-ia32.h
  )
elseif(x64_architecture)
  list(
      APPEND
      v8_base_sources
      src/compiler/x64/code-generator-x64.cc
      src/compiler/x64/instruction-codes-x64.h
      src/compiler/x64/instruction-scheduler-x64.cc
      src/compiler/x64/instruction-selector-x64.cc
      src/compiler/x64/unwinding-info-writer-x64.cc
      src/compiler/x64/unwinding-info-writer-x64.h
      src/debug/x64/debug-x64.cc
      src/regexp/x64/regexp-macro-assembler-x64.cc
      src/regexp/x64/regexp-macro-assembler-x64.h
      src/third_party/valgrind/valgrind.h
      src/wasm/baseline/x64/liftoff-assembler-x64.h
      src/x64/assembler-x64-inl.h
      src/x64/assembler-x64.cc
      src/x64/assembler-x64.h
      src/x64/code-stubs-x64.cc
      src/x64/codegen-x64.cc
      src/x64/constants-x64.h
      src/x64/cpu-x64.cc
      src/x64/deoptimizer-x64.cc
      src/x64/disasm-x64.cc
      src/x64/eh-frame-x64.cc
      src/x64/frame-constants-x64.cc
      src/x64/frame-constants-x64.h
      src/x64/interface-descriptors-x64.cc
      src/x64/macro-assembler-x64.cc
      src/x64/macro-assembler-x64.h
      src/x64/simulator-x64.cc
      src/x64/simulator-x64.h
      src/x64/sse-instr.h
  )

  if(is_linux)
    list(
        APPEND
        v8_base_sources
        src/trap-handler/handler-inside-linux.cc
        src/trap-handler/handler-outside-linux.cc
    )
  endif()

  if(WIN32)
    list(
        APPEND
        v8_base_sources
        src/trap-handler/handler-inside-win.cc
        src/trap-handler/handler-inside-win.h
        src/trap-handler/handler-outside-win.cc
    )
  endif()
elseif(arm_architecture)
  list(
      APPEND
      v8_base_sources
      src/arm/assembler-arm-inl.h
      src/arm/assembler-arm.cc
      src/arm/assembler-arm.h
      src/arm/constants-arm.cc
      src/arm/constants-arm.h
      src/arm/cpu-arm.cc
      src/arm/deoptimizer-arm.cc
      src/arm/disasm-arm.cc
      src/arm/eh-frame-arm.cc
      src/arm/frame-constants-arm.cc
      src/arm/frame-constants-arm.h
      src/arm/interface-descriptors-arm.cc
      src/arm/macro-assembler-arm.cc
      src/arm/macro-assembler-arm.h
      src/arm/register-arm.h
      src/arm/simulator-arm.cc
      src/arm/simulator-arm.h
      src/compiler/backend/arm/code-generator-arm.cc
      src/compiler/backend/arm/instruction-codes-arm.h
      src/compiler/backend/arm/instruction-scheduler-arm.cc
      src/compiler/backend/arm/instruction-selector-arm.cc
      src/compiler/backend/arm/unwinding-info-writer-arm.cc
      src/compiler/backend/arm/unwinding-info-writer-arm.h
      src/debug/arm/debug-arm.cc
      src/regexp/arm/regexp-macro-assembler-arm.cc
      src/regexp/arm/regexp-macro-assembler-arm.h
      src/wasm/baseline/arm/liftoff-assembler-arm.h
  )
elseif(arm64_architecture)
  list(
      APPEND
      v8_base_sources
      src/arm64/assembler-arm64-inl.h
      src/arm64/assembler-arm64.cc
      src/arm64/assembler-arm64.h
      src/arm64/constants-arm64.h
      src/arm64/cpu-arm64.cc
      src/arm64/decoder-arm64-inl.h
      src/arm64/decoder-arm64.cc
      src/arm64/decoder-arm64.h
      src/arm64/deoptimizer-arm64.cc
      src/arm64/disasm-arm64.cc
      src/arm64/disasm-arm64.h
      src/arm64/eh-frame-arm64.cc
      src/arm64/frame-constants-arm64.cc
      src/arm64/frame-constants-arm64.h
      # src/arm64/instructions-arm64-constants.cc # ??? See BUILD.gn
      src/arm64/instructions-arm64.cc
      src/arm64/instructions-arm64.h
      src/arm64/instrument-arm64.cc
      src/arm64/instrument-arm64.h
      src/arm64/interface-descriptors-arm64.cc
      src/arm64/macro-assembler-arm64-inl.h
      src/arm64/macro-assembler-arm64.cc
      src/arm64/macro-assembler-arm64.h
      src/arm64/register-arm64.cc
      src/arm64/register-arm64.h
      src/arm64/simulator-arm64.cc
      src/arm64/simulator-arm64.h
      src/arm64/simulator-logic-arm64.cc
      src/arm64/utils-arm64.cc
      src/arm64/utils-arm64.h
      src/compiler/backend/arm64/code-generator-arm64.cc
      src/compiler/backend/arm64/instruction-codes-arm64.h
      src/compiler/backend/arm64/instruction-scheduler-arm64.cc
      src/compiler/backend/arm64/instruction-selector-arm64.cc
      src/compiler/backend/arm64/unwinding-info-writer-arm64.cc
      src/compiler/backend/arm64/unwinding-info-writer-arm64.h
      src/debug/arm64/debug-arm64.cc
      src/regexp/arm64/regexp-macro-assembler-arm64.cc
      src/regexp/arm64/regexp-macro-assembler-arm64.h
      src/wasm/baseline/arm64/liftoff-assembler-arm64.h
  )
elseif(mips_architecture OR mipsel_architecture)
  list(
      APPEND
      v8_base_sources
      src/compiler/backend/mips/code-generator-mips.cc
      src/compiler/backend/mips/instruction-codes-mips.h
      src/compiler/backend/mips/instruction-scheduler-mips.cc
      src/compiler/backend/mips/instruction-selector-mips.cc
      src/debug/mips/debug-mips.cc
      src/mips/assembler-mips-inl.h
      src/mips/assembler-mips.cc
      src/mips/assembler-mips.h
      src/mips/constants-mips.cc
      src/mips/constants-mips.h
      src/mips/cpu-mips.cc
      src/mips/deoptimizer-mips.cc
      src/mips/disasm-mips.cc
      src/mips/frame-constants-mips.cc
      src/mips/frame-constants-mips.h
      src/mips/interface-descriptors-mips.cc
      src/mips/macro-assembler-mips.cc
      src/mips/macro-assembler-mips.h
      src/mips/register-mips.h
      src/mips/simulator-mips.cc
      src/mips/simulator-mips.h
      src/regexp/mips/regexp-macro-assembler-mips.cc
      src/regexp/mips/regexp-macro-assembler-mips.h
      src/wasm/baseline/mips/liftoff-assembler-mips.h
  )
elseif(mips64_architecture OR mips64el_architecture)
  list(
      APPEND
      v8_base_sources
      src/compiler/backend/mips64/code-generator-mips64.cc
      src/compiler/backend/mips64/instruction-codes-mips64.h
      src/compiler/backend/mips64/instruction-scheduler-mips64.cc
      src/compiler/backend/mips64/instruction-selector-mips64.cc
      src/debug/mips64/debug-mips64.cc
      src/mips64/assembler-mips64-inl.h
      src/mips64/assembler-mips64.cc
      src/mips64/assembler-mips64.h
      src/mips64/constants-mips64.cc
      src/mips64/constants-mips64.h
      src/mips64/cpu-mips64.cc
      src/mips64/deoptimizer-mips64.cc
      src/mips64/disasm-mips64.cc
      src/mips64/frame-constants-mips64.cc
      src/mips64/frame-constants-mips64.h
      src/mips64/interface-descriptors-mips64.cc
      src/mips64/macro-assembler-mips64.cc
      src/mips64/macro-assembler-mips64.h
      src/mips64/register-mips64.h
      src/mips64/simulator-mips64.cc
      src/mips64/simulator-mips64.h
      src/regexp/mips64/regexp-macro-assembler-mips64.cc
      src/regexp/mips64/regexp-macro-assembler-mips64.h
      src/wasm/baseline/mips64/liftoff-assembler-mips64.h
  )
elseif(ppc_architecture OR ppc64_architecture)
  list(
      APPEND
      v8_base_sources
      src/compiler/backend/ppc/code-generator-ppc.cc
      src/compiler/backend/ppc/instruction-codes-ppc.h
      src/compiler/backend/ppc/instruction-scheduler-ppc.cc
      src/compiler/backend/ppc/instruction-selector-ppc.cc
      src/debug/ppc/debug-ppc.cc
      src/ppc/assembler-ppc-inl.h
      src/ppc/assembler-ppc.cc
      src/ppc/assembler-ppc.h
      src/ppc/constants-ppc.cc
      src/ppc/constants-ppc.h
      src/ppc/cpu-ppc.cc
      src/ppc/deoptimizer-ppc.cc
      src/ppc/disasm-ppc.cc
      src/ppc/frame-constants-ppc.cc
      src/ppc/frame-constants-ppc.h
      src/ppc/interface-descriptors-ppc.cc
      src/ppc/macro-assembler-ppc.cc
      src/ppc/macro-assembler-ppc.h
      src/ppc/register-ppc.h
      src/ppc/simulator-ppc.cc
      src/ppc/simulator-ppc.h
      src/regexp/ppc/regexp-macro-assembler-ppc.cc
      src/regexp/ppc/regexp-macro-assembler-ppc.h
      src/wasm/baseline/ppc/liftoff-assembler-ppc.h
  )
elseif(s390_architecture OR s390x_architecture)
  list(
      APPEND
      v8_base_sources
      src/compiler/backend/s390/code-generator-s390.cc
      src/compiler/backend/s390/instruction-codes-s390.h
      src/compiler/backend/s390/instruction-scheduler-s390.cc
      src/compiler/backend/s390/instruction-selector-s390.cc
      src/debug/s390/debug-s390.cc
      src/regexp/s390/regexp-macro-assembler-s390.cc
      src/regexp/s390/regexp-macro-assembler-s390.h
      src/s390/assembler-s390-inl.h
      src/s390/assembler-s390.cc
      src/s390/assembler-s390.h
      src/s390/constants-s390.cc
      src/s390/constants-s390.h
      src/s390/cpu-s390.cc
      src/s390/deoptimizer-s390.cc
      src/s390/disasm-s390.cc
      src/s390/frame-constants-s390.cc
      src/s390/frame-constants-s390.h
      src/s390/interface-descriptors-s390.cc
      src/s390/macro-assembler-s390.cc
      src/s390/macro-assembler-s390.h
      src/s390/register-s390.h
      src/s390/simulator-s390.cc
      src/s390/simulator-s390.h
      src/wasm/baseline/s390/liftoff-assembler-s390.h
  )
endif()

if(V8_USE_SNAPSHOT AND V8_USE_EXTERNAL_STARTUP_DATA)
  # See BUILD.gn, v8_external_snapshot, sources
  # Not a separate target because of the cyclic dependencies
  list(
      APPEND
      v8_base_sources
      src/snapshot/natives-external.cc
      src/snapshot/snapshot-external.cc
  )
endif()

if(v8_enable_embedded_builtins AND emit_builtins_as_inline_asm)
  list(
      APPEND
      v8_base_sources
      native-gen/embedded.cc
  )
elseif(v8_enable_embedded_builtins)
  list(
      APPEND
      v8_base_sources
      native-gen/embedded.S
  )
else()
  list(
      APPEND
      v8_base_sources
      src/snapshot/embedded-empty.cc
  )
endif()

# See BUILD.gn, v8_libplatform, sources
set(
    v8_libplatform_sources
    src/libplatform/default-foreground-task-runner.cc
    src/libplatform/default-platform.cc
    src/libplatform/default-worker-threads-task-runner.cc
    src/libplatform/task-queue.cc
    src/libplatform/tracing/trace-buffer.cc
    src/libplatform/tracing/trace-config.cc
    src/libplatform/tracing/trace-object.cc
    src/libplatform/tracing/trace-writer.cc
    src/libplatform/tracing/tracing-controller.cc
    src/libplatform/worker-thread.cc
)

add_library(v8_libplatform ${v8_libplatform_sources})
target_include_directories(
    v8_libplatform
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/parent_inject
)

# See BUILD.gn, v8_libsampler, sources
set(
    v8_libsampler_sources
    src/libsampler/sampler.cc
)

add_library(v8_libsampler ${v8_libsampler_sources})

find_package(PythonInterp 2 REQUIRED) # PYTHON_EXECUTABLE

set(
    protocol_generator_py
    ${CMAKE_CURRENT_LIST_DIR}/third_party/inspector_protocol/CodeGenerator.py
)

set(generated_dir ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(generated_inspector_dir ${CMAKE_CURRENT_BINARY_DIR}/generated/src/inspector)
set(generated_inspector_include_dir ${CMAKE_CURRENT_BINARY_DIR}/generated/include/inspector)

set(
    generated_inspector_sources
    ${generated_inspector_dir}/protocol/Console.cpp
    ${generated_inspector_dir}/protocol/Debugger.cpp
    ${generated_inspector_dir}/protocol/HeapProfiler.cpp
    ${generated_inspector_dir}/protocol/Profiler.cpp
    ${generated_inspector_dir}/protocol/Protocol.cpp
    ${generated_inspector_dir}/protocol/Runtime.cpp
    ${generated_inspector_dir}/protocol/Schema.cpp
)

add_custom_command(
    OUTPUT
    ${generated_inspector_sources}
    COMMAND
    ${CMAKE_COMMAND}
    -E
    make_directory
    ${generated_inspector_dir}
    COMMAND
    ${CMAKE_COMMAND}
    -E
    make_directory
    ${generated_inspector_include_dir}
    COMMAND
    ${PYTHON_EXECUTABLE}
    ${protocol_generator_py}
    --jinja_dir
    ${CMAKE_CURRENT_LIST_DIR}/third_party
    --output_base
    ${generated_inspector_dir}
    --config
    ${CMAKE_CURRENT_LIST_DIR}/src/inspector/inspector_protocol_config.json
)

add_custom_command(
    OUTPUT
    ${generated_inspector_dir}/injected-script-source.h
    COMMAND
    ${CMAKE_COMMAND}
    -E
    make_directory
    ${generated_inspector_dir}
    COMMAND
    ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_LIST_DIR}/src/inspector/build/xxd.py
    InjectedScriptSource_js
    ${CMAKE_CURRENT_LIST_DIR}/src/inspector/injected-script-source.js
    ${generated_inspector_dir}/injected-script-source.h
    DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/src/inspector/build/xxd.py
    ${CMAKE_CURRENT_LIST_DIR}/src/inspector/injected-script-source.js
)

add_custom_target(
    inspector_custom_target
    COMMAND
    ${CMAKE_COMMAND} -E echo "Dummy target"
    DEPENDS
    ${generated_inspector_sources}
    ${generated_inspector_dir}/injected-script-source.h
)

# See src/inspector/BUILD.gn, inspector, sources
set(
    v8_inspector_sources
    src/inspector/injected-script.cc
    src/inspector/injected-script.h
    src/inspector/inspected-context.cc
    src/inspector/inspected-context.h
    src/inspector/remote-object-id.cc
    src/inspector/remote-object-id.h
    src/inspector/search-util.cc
    src/inspector/search-util.h
    src/inspector/string-16.cc
    src/inspector/string-16.h
    src/inspector/string-util.cc
    src/inspector/string-util.h
    src/inspector/test-interface.cc
    src/inspector/test-interface.h
    src/inspector/v8-console-agent-impl.cc
    src/inspector/v8-console-agent-impl.h
    src/inspector/v8-console-message.cc
    src/inspector/v8-console-message.h
    src/inspector/v8-console.cc
    src/inspector/v8-console.h
    src/inspector/v8-debugger-agent-impl.cc
    src/inspector/v8-debugger-agent-impl.h
    src/inspector/v8-debugger-script.cc
    src/inspector/v8-debugger-script.h
    src/inspector/v8-debugger.cc
    src/inspector/v8-debugger.h
    src/inspector/v8-function-call.cc
    src/inspector/v8-function-call.h
    src/inspector/v8-heap-profiler-agent-impl.cc
    src/inspector/v8-heap-profiler-agent-impl.h
    src/inspector/v8-injected-script-host.cc
    src/inspector/v8-injected-script-host.h
    src/inspector/v8-inspector-impl.cc
    src/inspector/v8-inspector-impl.h
    src/inspector/v8-inspector-session-impl.cc
    src/inspector/v8-inspector-session-impl.h
    src/inspector/v8-internal-value-type.cc
    src/inspector/v8-internal-value-type.h
    src/inspector/v8-profiler-agent-impl.cc
    src/inspector/v8-profiler-agent-impl.h
    src/inspector/v8-regex.cc
    src/inspector/v8-regex.h
    src/inspector/v8-runtime-agent-impl.cc
    src/inspector/v8-runtime-agent-impl.h
    src/inspector/v8-schema-agent-impl.cc
    src/inspector/v8-schema-agent-impl.h
    src/inspector/v8-stack-trace-impl.cc
    src/inspector/v8-stack-trace-impl.h
    src/inspector/v8-value-utils.cc
    src/inspector/v8-value-utils.h
    src/inspector/wasm-translation.cc
    src/inspector/wasm-translation.h
    ${generated_inspector_sources}
)

add_library(v8_inspector ${v8_inspector_sources})
add_dependencies(v8_inspector inspector_custom_target)
target_include_directories(
    v8_inspector
    PUBLIC
    $<BUILD_INTERFACE:${generated_dir}>
)

# See BUILD.gn, v8_libbase, sources
set(
    v8_libbase_sources
    src/base/adapters.h
    src/base/atomic-utils.h
    src/base/atomicops.h
    src/base/atomicops_internals_atomicword_compat.h
    src/base/atomicops_internals_portable.h
    src/base/atomicops_internals_std.h
    src/base/base-export.h
    src/base/bits.cc
    src/base/bits.h
    src/base/build_config.h
    src/base/compiler-specific.h
    src/base/cpu.cc
    src/base/cpu.h
    src/base/debug/stack_trace.cc
    src/base/debug/stack_trace.h
    src/base/division-by-constant.cc
    src/base/division-by-constant.h
    src/base/export-template.h
    src/base/file-utils.cc
    src/base/file-utils.h
    src/base/flags.h
    src/base/format-macros.h
    src/base/free_deleter.h
    src/base/functional.cc
    src/base/functional.h
    src/base/hashmap-entry.h
    src/base/hashmap.h
    src/base/ieee754.cc
    src/base/ieee754.h
    src/base/iterator.h
    src/base/lazy-instance.h
    src/base/list.h
    src/base/logging.cc
    src/base/logging.h
    src/base/macros.h
    src/base/once.cc
    src/base/once.h
    src/base/optional.h
    src/base/page-allocator.cc
    src/base/page-allocator.h
    src/base/platform/condition-variable.cc
    src/base/platform/condition-variable.h
    src/base/platform/elapsed-timer.h
    src/base/platform/mutex.cc
    src/base/platform/mutex.h
    src/base/platform/platform.h
    src/base/platform/semaphore.cc
    src/base/platform/semaphore.h
    src/base/platform/time.cc
    src/base/platform/time.h
    src/base/ring-buffer.h
    src/base/safe_conversions.h
    src/base/safe_conversions_impl.h
    src/base/safe_math.h
    src/base/safe_math_impl.h
    src/base/sys-info.cc
    src/base/sys-info.h
    src/base/template-utils.h
    src/base/timezone-cache.h
    src/base/tsan.h
    src/base/utils/random-number-generator.cc
    src/base/utils/random-number-generator.h
)

if(is_posix OR is_fuchsia)
  list(
      APPEND
      v8_libbase_sources
      src/base/platform/platform-posix.cc
      src/base/platform/platform-posix.h
  )
  if(NOT is_aix)
    list(
        APPEND
        v8_libbase_sources
        src/base/platform/platform-posix-time.cc
        src/base/platform/platform-posix-time.h
    )
  endif()
endif()

if(is_linux)
  list(
      APPEND
      v8_libbase_sources
      src/base/debug/stack_trace_posix.cc
      src/base/platform/platform-linux.cc
  )
elseif(is_aix)
  list(
      APPEND
      v8_libbase_sources
      src/base/debug/stack_trace_posix.cc
      src/base/platform/platform-aix.cc
  )
elseif(ANDROID)
  list(
      APPEND
      v8_libbase_sources
      src/base/debug/stack_trace_android.cc
      src/base/platform/platform-linux.cc
  )
elseif(is_fuchsia)
  list(
      APPEND
      v8_libbase_sources
      src/base/debug/stack_trace_fuchsia.cc
      src/base/platform/platform-fuchsia.cc
  )
elseif(APPLE AND NOT IOS)
  list(
      APPEND
      v8_libbase_sources
      src/base/debug/stack_trace_posix.cc
      src/base/platform/platform-macos.cc
  )
elseif(WIN32)
  list(
      APPEND
      v8_libbase_sources
      src/base/debug/stack_trace_win.cc
      src/base/platform/platform-win32.cc
      src/base/win32-headers.h
  )
endif()

add_library(v8_libbase ${v8_libbase_sources})

# See BUILD.gn, torque, sources
set(
    torque_base_sources
    src/torque/TorqueBaseVisitor.cpp
    src/torque/TorqueBaseVisitor.h
    src/torque/TorqueLexer.cpp
    src/torque/TorqueLexer.h
    src/torque/TorqueParser.cpp
    src/torque/TorqueParser.h
    src/torque/TorqueVisitor.cpp
    src/torque/TorqueVisitor.h
    src/torque/ast-generator.cc
    src/torque/ast-generator.h
    src/torque/ast.h
    src/torque/contextual.h
    src/torque/declarable.cc
    src/torque/declarable.h
    src/torque/declaration-visitor.cc
    src/torque/declaration-visitor.h
    src/torque/declarations.cc
    src/torque/declarations.h
    src/torque/file-visitor.cc
    src/torque/file-visitor.h
    src/torque/global-context.h
    src/torque/implementation-visitor.cc
    src/torque/implementation-visitor.h
    src/torque/scope.cc
    src/torque/scope.h
    src/torque/torque.cc
    src/torque/type-oracle.h
    src/torque/types.cc
    src/torque/types.h
    src/torque/utils.cc
    src/torque/utils.h
)

add_library(torque_base ${torque_base_sources})
target_include_directories(
    torque_base
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

# See BUILD.gn, torque, sources
add_executable(torque src/torque/torque.cc)
target_link_libraries(torque PRIVATE torque_base v8_libbase)

set(
    v8_extras_libraries_source
    ${CMAKE_CURRENT_BINARY_DIR}/extras-libraries.cc
)

add_custom_command(
    OUTPUT
        ${v8_extras_libraries_source}
    COMMAND
        ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
        ${v8_extras_libraries_source}
        EXTRAS
    DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
    COMMENT
        "Generating ${v8_extras_libraries_source} file"
)

set(
    v8_libraries_source
    ${CMAKE_CURRENT_BINARY_DIR}/libraries.cc
)

add_custom_command(
    OUTPUT
        ${v8_libraries_source}
    COMMAND
        ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
        ${v8_libraries_source}
        CORE
        ${CMAKE_CURRENT_LIST_DIR}/src/js/macros.py
        ${CMAKE_CURRENT_LIST_DIR}/src/messages.h
        ${CMAKE_CURRENT_LIST_DIR}/src/js/prologue.js
        ${CMAKE_CURRENT_LIST_DIR}/src/js/array.js
        ${CMAKE_CURRENT_LIST_DIR}/src/js/typedarray.js
        ${CMAKE_CURRENT_LIST_DIR}/src/debug/liveedit.js
        ${CMAKE_CURRENT_LIST_DIR}/src/js/intl.js
    DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
        ${CMAKE_CURRENT_LIST_DIR}/src/js/macros.py
        ${CMAKE_CURRENT_LIST_DIR}/src/messages.h
        ${CMAKE_CURRENT_LIST_DIR}/src/js/prologue.js
        ${CMAKE_CURRENT_LIST_DIR}/src/js/array.js
        ${CMAKE_CURRENT_LIST_DIR}/src/js/typedarray.js
        ${CMAKE_CURRENT_LIST_DIR}/src/debug/liveedit.js
        ${CMAKE_CURRENT_LIST_DIR}/src/js/intl.js
)

set(
    v8_experimental_extras_libraries_source
    ${CMAKE_CURRENT_BINARY_DIR}/experimental-extras-libraries.cc
)

add_custom_command(
    OUTPUT
        ${v8_experimental_extras_libraries_source}
    COMMAND
        ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
        ${v8_experimental_extras_libraries_source}
        EXPERIMENTAL_EXTRAS
    DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
)

# We have to create separate target because generated file used in
# v8_nosnapshot and v8_snapshot.
# From:
# - https://cmake.org/cmake/help/latest/command/add_custom_command.html
#
#    Do not list the output in more than one independent target that
#    may build in parallel or the two instances of the rule may conflict
add_library(
    v8_extras_libraries
    ${v8_extras_libraries_source}
    ${v8_libraries_source}
    ${v8_experimental_extras_libraries_source}
)

add_library(
    v8_nosnapshot
    src/snapshot/snapshot-empty.cc
)
target_link_libraries(v8_nosnapshot PUBLIC v8_extras_libraries)

add_library(
    v8_snapshot
    src/setup-isolate-deserialize.cc
)
target_link_libraries(v8_snapshot PUBLIC v8_extras_libraries)

# See BUILD.gn, torque_files
set(
    torque_files
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/base.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-foreach.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-sort.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/typed-array.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/data-view.tq
    ${CMAKE_CURRENT_LIST_DIR}/test/torque/test-torque.tq
)

# See BUILD.gn, torque_modules
set(
    torque_namespaces
    base
    array
    typed-array
    data-view
    test
)

set(torque_generated_parentdir ${CMAKE_CURRENT_BINARY_DIR}/tgd)
set(torque_generated_dir ${torque_generated_parentdir}/torque-generated)

set(
    torque_output_files
    ${torque_generated_dir}/builtin-definitions-from-dsl.h
)

set(torque_output_files_cc "")

foreach(ns IN LISTS torque_namespaces)
  list(
      APPEND
      torque_output_files
      ${torque_generated_dir}/builtins-${ns}-from-dsl-gen.cc
      ${torque_generated_dir}/builtins-${ns}-from-dsl-gen.h
  )
  list(
      APPEND
      torque_output_files_cc
      ${torque_generated_dir}/builtins-${ns}-from-dsl-gen.cc
  )
endforeach()

if(CMAKE_CROSSCOMPILING)
  include(hunter_experimental_add_host_project)
  hunter_experimental_add_host_project(cmake/host)
  set(torque_executable "${HUNTER_HOST_ROOT}/bin/torque")
else()
  set(torque_executable $<TARGET_FILE:torque>)
endif()

add_custom_command(
    OUTPUT
        ${torque_output_files}
    COMMAND
        ${CMAKE_COMMAND}
        -E
        make_directory
        ${torque_generated_dir}
    COMMAND
        ${torque_executable}
        -o
        ${torque_generated_dir}
        ${torque_files}
    DEPENDS
        torque
        ${torque_files}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}
)

add_custom_target(
    torque_custom_target
    COMMAND
    ${CMAKE_COMMAND} -E echo "Dummy target"
    DEPENDS
    ${torque_output_files}
)

# See BUILD.gn, v8_initializers, sources
set(
    v8_initializer_sources
    src/builtins/builtins-arguments-gen.cc
    src/builtins/builtins-arguments-gen.h
    src/builtins/builtins-array-gen.cc
    src/builtins/builtins-array-gen.h
    src/builtins/builtins-async-function-gen.cc
    src/builtins/builtins-async-gen.cc
    src/builtins/builtins-async-gen.h
    src/builtins/builtins-async-generator-gen.cc
    src/builtins/builtins-async-iterator-gen.cc
    src/builtins/builtins-boolean-gen.cc
    src/builtins/builtins-call-gen.cc
    src/builtins/builtins-call-gen.h
    src/builtins/builtins-collections-gen.cc
    src/builtins/builtins-console-gen.cc
    src/builtins/builtins-constructor-gen.cc
    src/builtins/builtins-constructor-gen.h
    src/builtins/builtins-constructor.h
    src/builtins/builtins-conversion-gen.cc
    src/builtins/builtins-data-view-gen.h
    src/builtins/builtins-date-gen.cc
    src/builtins/builtins-debug-gen.cc
    src/builtins/builtins-function-gen.cc
    src/builtins/builtins-generator-gen.cc
    src/builtins/builtins-global-gen.cc
    src/builtins/builtins-handler-gen.cc
    src/builtins/builtins-ic-gen.cc
    src/builtins/builtins-internal-gen.cc
    src/builtins/builtins-interpreter-gen.cc
    src/builtins/builtins-intl-gen.cc
    src/builtins/builtins-iterator-gen.cc
    src/builtins/builtins-iterator-gen.h
    src/builtins/builtins-lazy-gen.cc
    src/builtins/builtins-lazy-gen.h
    src/builtins/builtins-math-gen.cc
    src/builtins/builtins-math-gen.h
    src/builtins/builtins-number-gen.cc
    src/builtins/builtins-object-gen.cc
    src/builtins/builtins-promise-gen.cc
    src/builtins/builtins-promise-gen.h
    src/builtins/builtins-proxy-gen.cc
    src/builtins/builtins-proxy-gen.h
    src/builtins/builtins-reflect-gen.cc
    src/builtins/builtins-regexp-gen.cc
    src/builtins/builtins-regexp-gen.h
    src/builtins/builtins-sharedarraybuffer-gen.cc
    src/builtins/builtins-string-gen.cc
    src/builtins/builtins-string-gen.h
    src/builtins/builtins-symbol-gen.cc
    src/builtins/builtins-typed-array-gen.cc
    src/builtins/builtins-typed-array-gen.h
    src/builtins/builtins-utils-gen.h
    src/builtins/builtins-wasm-gen.cc
    src/builtins/growable-fixed-array-gen.cc
    src/builtins/growable-fixed-array-gen.h
    src/builtins/setup-builtins-internal.cc
    src/heap/setup-heap-internal.cc
    src/ic/accessor-assembler.cc
    src/ic/accessor-assembler.h
    src/ic/binary-op-assembler.cc
    src/ic/binary-op-assembler.h
    src/ic/keyed-store-generic.cc
    src/ic/keyed-store-generic.h
    src/interpreter/interpreter-assembler.cc
    src/interpreter/interpreter-assembler.h
    src/interpreter/interpreter-generator.cc
    src/interpreter/interpreter-generator.h
    src/interpreter/interpreter-intrinsics-generator.cc
    src/interpreter/interpreter-intrinsics-generator.h
    src/interpreter/setup-interpreter-internal.cc
    src/interpreter/setup-interpreter.h
    ${torque_output_files_cc}
)

if(x64_architecture)
  list(
      APPEND
      v8_initializer_sources
      src/builtins/x64/builtins-x64.cc
  )
endif()

if(arm64_architecture)
  list(
      APPEND
      v8_initializer_sources
      src/builtins/arm64/builtins-arm64.cc
  )
endif()

if(arm_architecture)
  list(
      APPEND
      v8_initializer_sources
      src/builtins/arm/builtins-arm.cc
  )
endif()

add_library(v8_initializers ${v8_initializer_sources})
add_dependencies(v8_initializers torque_custom_target)

add_dependencies(v8_initializers inspector_custom_target) # depends on generated headers
target_include_directories(
    v8_initializers
    PUBLIC
    $<BUILD_INTERFACE:${generated_dir}>
)

add_library(v8_base ${v8_base_sources})
add_dependencies(v8_base torque_custom_target) # depends on generated headers

target_link_libraries(v8_extras_libraries PUBLIC v8_base)
target_link_libraries(v8_inspector PUBLIC v8_base)

target_include_directories(
    v8_base
    PUBLIC
    $<BUILD_INTERFACE:${torque_generated_parentdir}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/parent_inject>
)

# See BUILD.gn, v8_init, sources
add_library(v8_init src/setup-isolate-full.cc)
target_link_libraries(v8_init PUBLIC v8_initializers)

target_link_libraries(v8_libplatform PUBLIC v8_libbase)
target_link_libraries(v8_libsampler PUBLIC v8_libbase)
target_link_libraries(v8_base PUBLIC v8_libsampler)
target_link_libraries(v8_initializers PUBLIC v8_base)

if(x64_architecture)
  target_compile_definitions(
      v8_libbase
      PUBLIC
      $<BUILD_INTERFACE:V8_TARGET_ARCH_X64>
  )
endif()

if(arm64_architecture)
  target_compile_definitions(
      v8_libbase
      PUBLIC
      $<BUILD_INTERFACE:V8_TARGET_ARCH_ARM64>
  )
endif()

if(arm_architecture)
  target_compile_definitions(
      v8_libbase
      PUBLIC
      $<BUILD_INTERFACE:V8_TARGET_ARCH_ARM>
  )
endif()

target_compile_definitions(
    v8_libbase
    PUBLIC
    V8_DEPRECATION_WARNINGS

    $<BUILD_INTERFACE:DISABLE_UNTRUSTED_CODE_MITIGATIONS>
    $<BUILD_INTERFACE:ENABLE_DISASSEMBLER>
    $<BUILD_INTERFACE:ENABLE_HANDLE_ZAPPING=1>
    $<BUILD_INTERFACE:ENABLE_MINOR_MC>
    $<BUILD_INTERFACE:OBJECT_PRINT>
    $<BUILD_INTERFACE:V8_CONCURRENT_MARKING>
    $<BUILD_INTERFACE:V8_EMBEDDED_BUILTINS>
    $<BUILD_INTERFACE:V8_INTL_SUPPORT>
    $<BUILD_INTERFACE:V8_TYPED_ARRAY_MAX_SIZE_IN_HEAP=64>
    $<BUILD_INTERFACE:VERIFY_HEAP>

    # Debug
    $<BUILD_INTERFACE:$<$<CONFIG:Debug>:VERIFY_HEAP=1>>
    $<BUILD_INTERFACE:$<$<CONFIG:Debug>:OBJECT_PRINT=1>>
    $<BUILD_INTERFACE:$<$<CONFIG:Debug>:ENABLE_DISASSEMBLER=1>>
    $<BUILD_INTERFACE:$<$<CONFIG:Debug>:V8_ENABLE_CHECKS=1>>
    $<BUILD_INTERFACE:$<$<CONFIG:Debug>:TRACE_MAPS=1>>
    $<BUILD_INTERFACE:$<$<CONFIG:Debug>:ENABLE_SLOW_DCHECKS=1>>
    $<BUILD_INTERFACE:$<$<CONFIG:Debug>:DEBUG=1>>
)

if(V8_USE_SNAPSHOT)
  target_compile_definitions(
      v8_libbase
      PUBLIC
      $<BUILD_INTERFACE:V8_USE_SNAPSHOT>
  )
endif()

if(V8_USE_EXTERNAL_STARTUP_DATA)
  target_compile_definitions(
      v8_libbase
      PUBLIC
      $<BUILD_INTERFACE:V8_USE_EXTERNAL_STARTUP_DATA>
  )
endif()

if(NOT ANDROID)
  target_compile_definitions(
      v8_libbase
      PUBLIC
      $<BUILD_INTERFACE:ENABLE_GDB_JIT_INTERFACE>
  )
endif()

if(RASPBERRY_PI)
  target_compile_definitions(
      v8_libbase
      PRIVATE
      V8_NO_GETAUXVAL
  )
endif()

target_include_directories(
    v8_libbase
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

include(CheckLibraryExists) # check_library_exists
check_library_exists(rt clock_gettime "" rt_library)

if(rt_library)
  target_link_libraries(v8_libbase PUBLIC rt)
endif()

target_link_libraries(v8_libbase PUBLIC GTest::gtest ICU::i18n)

if(ANDROID)
  find_package(android_log REQUIRED)
  target_link_libraries(v8_libbase PUBLIC android_log::android_log)
endif()

### Installation (https://github.com/forexample/package-example) {

# Introduce variables:
# * CMAKE_INSTALL_LIBDIR
# * CMAKE_INSTALL_BINDIR
# * CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(NAMESPACE "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)

# Use:
# * PROJECT_VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Use:
# * TARGETS_EXPORT_NAME
# * PROJECT_NAME
configure_package_config_file(
    "cmake/template/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(
    DIRECTORY "include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING
    PATTERN "*.h"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${NAMESPACE}"
    DESTINATION "${config_install_dir}"
)

### }

install(
    TARGETS
        v8_base
        v8_extras_libraries
        v8_inspector
        v8_libbase
        v8_libplatform
        v8_libsampler
        v8_init
        v8_initializers
        v8_nosnapshot
        v8_snapshot
        torque
        torque_base
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Use:
# - TARGETS_EXPORT_NAME
# - CMAKE_INSTALL_LIBDIR
add_subdirectory(third_party/antlr4)
target_link_libraries(torque_base PUBLIC antlr4::antlr4)

if(V8_BUILD_HELLOWORLD)
  add_executable(helloworld samples/hello-world.cc)
  target_link_libraries(
      helloworld
      PRIVATE
      v8_libplatform
      v8_libbase
      v8_base
      v8_nosnapshot
      v8_init
      v8_initializers
      v8_libsampler
  )
endif()
