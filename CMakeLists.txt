cmake_minimum_required(VERSION 3.2)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.114.tar.gz"
    SHA1 "712b3f5ea467fbddd4769a7800a060fbd6a31ab5"
)

project(v8 VERSION 7.4.98 LANGUAGES C CXX ASM)

# GN: see 'v8_use_snapshot'
option(V8_USE_SNAPSHOT "..." OFF)

# GN: see 'v8_use_external_startup_data'
option(V8_USE_EXTERNAL_STARTUP_DATA "..." OFF)

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
    src/accessors.cc
    src/address-map.cc
    src/allocation.cc
    src/api-arguments.cc
    src/api-natives.cc
    src/api.cc
    src/arguments.cc
    src/asmjs/asm-js.cc
    src/asmjs/asm-parser.cc
    src/asmjs/asm-scanner.cc
    src/asmjs/asm-types.cc
    src/assembler.cc
    src/assert-scope.cc
    src/ast/ast-function-literal-id-reindexer.cc
    src/ast/ast-value-factory.cc
    src/ast/ast.cc
    src/ast/modules.cc
    src/ast/prettyprinter.cc
    src/ast/scopes.cc
    src/ast/source-range-ast-visitor.cc
    src/ast/variables.cc
    src/bailout-reason.cc
    src/basic-block-profiler.cc
    src/bignum-dtoa.cc
    src/bignum.cc
    src/bit-vector.cc
    src/bootstrapper.cc
    src/builtins/builtins-api.cc
    src/builtins/builtins-array.cc
    src/builtins/builtins-arraybuffer.cc
    src/builtins/builtins-bigint.cc
    src/builtins/builtins-boolean.cc
    src/builtins/builtins-call.cc
    src/builtins/builtins-callsite.cc
    src/builtins/builtins-collections.cc
    src/builtins/builtins-console.cc
    src/builtins/builtins-dataview.cc
    src/builtins/builtins-date.cc
    src/builtins/builtins-error.cc
    src/builtins/builtins-extras-utils.cc
    src/builtins/builtins-function.cc
    src/builtins/builtins-global.cc
    src/builtins/builtins-internal.cc
    src/builtins/builtins-intl.cc
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
    src/builtins/builtins-trace.cc
    src/builtins/builtins-typed-array.cc
    src/builtins/builtins-weak-refs.cc
    src/builtins/builtins.cc
    src/builtins/constants-table-builder.cc
    src/cached-powers.cc
    src/cancelable-task.cc
    src/char-predicates.cc
    src/code-comments.cc
    src/code-desc.cc
    src/code-factory.cc
    src/code-reference.cc
    src/compilation-cache.cc
    src/compilation-statistics.cc
    src/compiler-dispatcher/compiler-dispatcher.cc
    src/compiler-dispatcher/optimizing-compile-dispatcher.cc
    src/compiler.cc
    src/compiler/access-builder.cc
    src/compiler/access-info.cc
    src/compiler/all-nodes.cc
    src/compiler/backend/code-generator.cc
    src/compiler/backend/frame-elider.cc
    src/compiler/backend/gap-resolver.cc
    src/compiler/backend/instruction-scheduler.cc
    src/compiler/backend/instruction-selector.cc
    src/compiler/backend/instruction.cc
    src/compiler/backend/jump-threading.cc
    src/compiler/backend/live-range-separator.cc
    src/compiler/backend/move-optimizer.cc
    src/compiler/backend/register-allocator-verifier.cc
    src/compiler/backend/register-allocator.cc
    src/compiler/basic-block-instrumentor.cc
    src/compiler/branch-elimination.cc
    src/compiler/bytecode-analysis.cc
    src/compiler/bytecode-graph-builder.cc
    src/compiler/bytecode-liveness-map.cc
    src/compiler/c-linkage.cc
    src/compiler/checkpoint-elimination.cc
    src/compiler/code-assembler.cc
    src/compiler/common-node-cache.cc
    src/compiler/common-operator-reducer.cc
    src/compiler/common-operator.cc
    src/compiler/compilation-dependencies.cc
    src/compiler/compiler-source-position-table.cc
    src/compiler/constant-folding-reducer.cc
    src/compiler/control-equivalence.cc
    src/compiler/control-flow-optimizer.cc
    src/compiler/dead-code-elimination.cc
    src/compiler/effect-control-linearizer.cc
    src/compiler/escape-analysis-reducer.cc
    src/compiler/escape-analysis.cc
    src/compiler/frame-states.cc
    src/compiler/frame.cc
    src/compiler/graph-assembler.cc
    src/compiler/graph-reducer.cc
    src/compiler/graph-trimmer.cc
    src/compiler/graph-visualizer.cc
    src/compiler/graph.cc
    src/compiler/int64-lowering.cc
    src/compiler/js-call-reducer.cc
    src/compiler/js-context-specialization.cc
    src/compiler/js-create-lowering.cc
    src/compiler/js-generic-lowering.cc
    src/compiler/js-graph.cc
    src/compiler/js-heap-broker.cc
    src/compiler/js-heap-copy-reducer.cc
    src/compiler/js-inlining-heuristic.cc
    src/compiler/js-inlining.cc
    src/compiler/js-intrinsic-lowering.cc
    src/compiler/js-native-context-specialization.cc
    src/compiler/js-operator.cc
    src/compiler/js-type-hint-lowering.cc
    src/compiler/js-typed-lowering.cc
    src/compiler/linkage.cc
    src/compiler/load-elimination.cc
    src/compiler/loop-analysis.cc
    src/compiler/loop-peeling.cc
    src/compiler/loop-variable-optimizer.cc
    src/compiler/machine-graph-verifier.cc
    src/compiler/machine-graph.cc
    src/compiler/machine-operator-reducer.cc
    src/compiler/machine-operator.cc
    src/compiler/memory-optimizer.cc
    src/compiler/node-cache.cc
    src/compiler/node-marker.cc
    src/compiler/node-matchers.cc
    src/compiler/node-origin-table.cc
    src/compiler/node-properties.cc
    src/compiler/node.cc
    src/compiler/opcodes.cc
    src/compiler/operation-typer.cc
    src/compiler/operator-properties.cc
    src/compiler/operator.cc
    src/compiler/osr.cc
    src/compiler/pipeline-statistics.cc
    src/compiler/pipeline.cc
    src/compiler/property-access-builder.cc
    src/compiler/raw-machine-assembler.cc
    src/compiler/redundancy-elimination.cc
    src/compiler/refs-map.cc
    src/compiler/representation-change.cc
    src/compiler/schedule.cc
    src/compiler/scheduler.cc
    src/compiler/select-lowering.cc
    src/compiler/serializer-for-background-compilation.cc
    src/compiler/simd-scalar-lowering.cc
    src/compiler/simplified-lowering.cc
    src/compiler/simplified-operator-reducer.cc
    src/compiler/simplified-operator.cc
    src/compiler/state-values-utils.cc
    src/compiler/store-store-elimination.cc
    src/compiler/type-cache.cc
    src/compiler/type-narrowing-reducer.cc
    src/compiler/typed-optimization.cc
    src/compiler/typer.cc
    src/compiler/types.cc
    src/compiler/value-numbering-reducer.cc
    src/compiler/verifier.cc
    src/compiler/wasm-compiler.cc
    src/compiler/zone-stats.cc
    src/constant-pool.cc
    src/contexts.cc
    src/conversions.cc
    src/counters.cc
    src/date.cc
    src/dateparser.cc
    src/debug/debug-coverage.cc
    src/debug/debug-evaluate.cc
    src/debug/debug-frames.cc
    src/debug/debug-property-iterator.cc
    src/debug/debug-scope-iterator.cc
    src/debug/debug-scopes.cc
    src/debug/debug-stack-trace-iterator.cc
    src/debug/debug-type-profile.cc
    src/debug/debug.cc
    src/debug/liveedit.cc
    src/deoptimize-reason.cc
    src/deoptimizer.cc
    src/detachable-vector.cc
    src/disassembler.cc
    src/diy-fp.cc
    src/dtoa.cc
    src/eh-frame.cc
    src/elements-kind.cc
    src/elements.cc
    src/execution.cc
    src/extensions/externalize-string-extension.cc
    src/extensions/free-buffer-extension.cc
    src/extensions/gc-extension.cc
    src/extensions/ignition-statistics-extension.cc
    src/extensions/statistics-extension.cc
    src/extensions/trigger-failure-extension.cc
    src/external-reference-table.cc
    src/external-reference.cc
    src/fast-dtoa.cc
    src/feedback-vector.cc
    src/field-type.cc
    src/fixed-dtoa.cc
    src/flags.cc
    src/frames.cc
    src/futex-emulation.cc
    src/gdb-jit.cc
    src/global-handles.cc
    src/handler-table.cc
    src/handles.cc
    src/heap/array-buffer-collector.cc
    src/heap/array-buffer-tracker.cc
    src/heap/code-stats.cc
    src/heap/concurrent-marking.cc
    src/heap/embedder-tracing.cc
    src/heap/factory.cc
    src/heap/gc-idle-time-handler.cc
    src/heap/gc-tracer.cc
    src/heap/heap-controller.cc
    src/heap/heap.cc
    src/heap/incremental-marking-job.cc
    src/heap/incremental-marking.cc
    src/heap/invalidated-slots.cc
    src/heap/item-parallel-job.cc
    src/heap/mark-compact.cc
    src/heap/marking.cc
    src/heap/memory-reducer.cc
    src/heap/object-stats.cc
    src/heap/objects-visiting.cc
    src/heap/scavenge-job.cc
    src/heap/scavenger.cc
    src/heap/slot-set.cc
    src/heap/spaces.cc
    src/heap/store-buffer.cc
    src/heap/stress-marking-observer.cc
    src/heap/stress-scavenge-observer.cc
    src/heap/sweeper.cc
    src/ic/call-optimization.cc
    src/ic/handler-configuration.cc
    src/ic/ic-stats.cc
    src/ic/ic.cc
    src/ic/stub-cache.cc
    src/icu_util.cc
    src/identity-map.cc
    src/interface-descriptors.cc
    src/interpreter/bytecode-array-accessor.cc
    src/interpreter/bytecode-array-builder.cc
    src/interpreter/bytecode-array-iterator.cc
    src/interpreter/bytecode-array-random-iterator.cc
    src/interpreter/bytecode-array-writer.cc
    src/interpreter/bytecode-decoder.cc
    src/interpreter/bytecode-flags.cc
    src/interpreter/bytecode-generator.cc
    src/interpreter/bytecode-label.cc
    src/interpreter/bytecode-node.cc
    src/interpreter/bytecode-operands.cc
    src/interpreter/bytecode-register-optimizer.cc
    src/interpreter/bytecode-register.cc
    src/interpreter/bytecode-source-info.cc
    src/interpreter/bytecodes.cc
    src/interpreter/constant-array-builder.cc
    src/interpreter/control-flow-builders.cc
    src/interpreter/handler-table-builder.cc
    src/interpreter/interpreter-intrinsics.cc
    src/interpreter/interpreter.cc
    src/isolate-allocator.cc
    src/isolate.cc
    src/json-parser.cc
    src/json-stringifier.cc
    src/keys.cc
    src/layout-descriptor.cc
    src/log-utils.cc
    src/log.cc
    src/lookup-cache.cc
    src/lookup.cc
    src/machine-type.cc
    src/map-updater.cc
    src/math-random.cc
    src/memcopy.cc
    src/messages.cc
    src/microtask-queue.cc
    src/objects-debug.cc
    src/objects-printer.cc
    src/objects.cc
    src/objects/bigint.cc
    src/objects/debug-objects.cc
    src/objects/embedder-data-array.cc
    src/objects/intl-objects.cc
    src/objects/js-array-buffer.cc
    src/objects/js-break-iterator.cc
    src/objects/js-collator.cc
    src/objects/js-date-time-format.cc
    src/objects/js-list-format.cc
    src/objects/js-locale.cc
    src/objects/js-number-format.cc
    src/objects/js-objects.cc
    src/objects/js-plural-rules.cc
    src/objects/js-relative-time-format.cc
    src/objects/js-segment-iterator.cc
    src/objects/js-segmenter.cc
    src/objects/literal-objects.cc
    src/objects/managed.cc
    src/objects/map.cc
    src/objects/module.cc
    src/objects/ordered-hash-table.cc
    src/objects/scope-info.cc
    src/objects/string-comparator.cc
    src/objects/string.cc
    src/objects/template-objects.cc
    src/optimized-compilation-info.cc
    src/ostreams.cc
    src/parsing/expression-scope-reparenter.cc
    src/parsing/func-name-inferrer.cc
    src/parsing/parse-info.cc
    src/parsing/parser.cc
    src/parsing/parsing.cc
    src/parsing/preparse-data.cc
    src/parsing/preparser.cc
    src/parsing/rewriter.cc
    src/parsing/scanner-character-streams.cc
    src/parsing/scanner.cc
    src/parsing/token.cc
    src/pending-compilation-error-handler.cc
    src/perf-jit.cc
    src/profiler/allocation-tracker.cc
    src/profiler/cpu-profiler.cc
    src/profiler/heap-profiler.cc
    src/profiler/heap-snapshot-generator.cc
    src/profiler/profile-generator.cc
    src/profiler/profiler-listener.cc
    src/profiler/sampling-heap-profiler.cc
    src/profiler/strings-storage.cc
    src/profiler/tick-sample.cc
    src/profiler/tracing-cpu-profiler.cc
    src/property-descriptor.cc
    src/property.cc
    src/regexp/interpreter-irregexp.cc
    src/regexp/jsregexp.cc
    src/regexp/property-sequences.cc
    src/regexp/regexp-ast.cc
    src/regexp/regexp-macro-assembler-irregexp.cc
    src/regexp/regexp-macro-assembler-tracer.cc
    src/regexp/regexp-macro-assembler.cc
    src/regexp/regexp-parser.cc
    src/regexp/regexp-stack.cc
    src/regexp/regexp-utils.cc
    src/register-configuration.cc
    src/reloc-info.cc
    src/roots.cc
    src/runtime-profiler.cc
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
    src/runtime/runtime-wasm.cc
    src/runtime/runtime-weak-refs.cc
    src/runtime/runtime.cc
    src/safepoint-table.cc
    src/simulator-base.cc
    src/snapshot/code-serializer.cc
    src/snapshot/deserializer-allocator.cc
    src/snapshot/deserializer.cc
    src/snapshot/embedded-data.cc
    src/snapshot/natives-common.cc
    src/snapshot/object-deserializer.cc
    src/snapshot/partial-deserializer.cc
    src/snapshot/partial-serializer.cc
    src/snapshot/read-only-deserializer.cc
    src/snapshot/read-only-serializer.cc
    src/snapshot/roots-serializer.cc
    src/snapshot/serializer-allocator.cc
    src/snapshot/serializer-common.cc
    src/snapshot/serializer.cc
    src/snapshot/snapshot-common.cc
    src/snapshot/snapshot-source-sink.cc
    src/snapshot/startup-deserializer.cc
    src/snapshot/startup-serializer.cc
    src/source-position-table.cc
    src/source-position.cc
    src/startup-data-util.cc
    src/string-builder.cc
    src/string-case.cc
    src/string-constants.cc
    src/string-stream.cc
    src/strtod.cc
    src/task-utils.cc
    src/third_party/siphash/halfsiphash.cc
    src/thread-id.cc
    src/tracing/trace-event.cc
    src/tracing/traced-value.cc
    src/tracing/tracing-category-observer.cc
    src/transitions.cc
    src/trap-handler/handler-inside.cc
    src/trap-handler/handler-outside.cc
    src/trap-handler/handler-shared.cc
    src/turbo-assembler.cc
    src/type-hints.cc
    src/unicode-decoder.cc
    src/unicode.cc
    src/unoptimized-compilation-info.cc
    src/unwinder.cc
    src/uri.cc
    src/utils.cc
    src/v8.cc
    src/v8threads.cc
    src/value-serializer.cc
    src/vector-slot-pair.cc
    src/version.cc
    src/visitors.cc
    src/wasm/baseline/liftoff-assembler.cc
    src/wasm/baseline/liftoff-compiler.cc
    src/wasm/function-body-decoder.cc
    src/wasm/function-compiler.cc
    src/wasm/graph-builder-interface.cc
    src/wasm/jump-table-assembler.cc
    src/wasm/local-decl-encoder.cc
    src/wasm/memory-tracing.cc
    src/wasm/module-compiler.cc
    src/wasm/module-decoder.cc
    src/wasm/module-instantiate.cc
    src/wasm/signature-map.cc
    src/wasm/streaming-decoder.cc
    src/wasm/wasm-code-manager.cc
    src/wasm/wasm-debug.cc
    src/wasm/wasm-engine.cc
    src/wasm/wasm-external-refs.cc
    src/wasm/wasm-features.cc
    src/wasm/wasm-interpreter.cc
    src/wasm/wasm-js.cc
    src/wasm/wasm-memory.cc
    src/wasm/wasm-module-builder.cc
    src/wasm/wasm-module.cc
    src/wasm/wasm-objects.cc
    src/wasm/wasm-opcodes.cc
    src/wasm/wasm-result.cc
    src/wasm/wasm-serialization.cc
    src/wasm/wasm-text.cc
    src/zone/accounting-allocator.cc
    src/zone/zone-segment.cc
    src/zone/zone.cc
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
      src/compiler/backend/x64/code-generator-x64.cc
      src/compiler/backend/x64/instruction-codes-x64.h
      src/compiler/backend/x64/instruction-scheduler-x64.cc
      src/compiler/backend/x64/instruction-selector-x64.cc
      src/compiler/backend/x64/unwinding-info-writer-x64.cc
      src/compiler/backend/x64/unwinding-info-writer-x64.h
      src/debug/x64/debug-x64.cc
      src/regexp/x64/regexp-macro-assembler-x64.cc
      src/regexp/x64/regexp-macro-assembler-x64.h
      src/third_party/valgrind/valgrind.h
      src/wasm/baseline/x64/liftoff-assembler-x64.h
      src/x64/assembler-x64-inl.h
      src/x64/assembler-x64.cc
      src/x64/assembler-x64.h
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
      src/x64/register-x64.h
      src/x64/sse-instr.h
  )

  if(is_linux OR APPLE)
    list(
        APPEND
        v8_base_sources
        src/trap-handler/handler-inside-posix.cc
        src/trap-handler/handler-inside-posix.h
        src/trap-handler/handler-outside-posix.cc
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

add_library(v8_base ${v8_base_sources})

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

# See BUILD.gn, v8_libsampler, sources
set(
    v8_libsampler_sources
    src/libsampler/sampler.cc
)

add_library(v8_libsampler ${v8_libsampler_sources})

# See src/inspector/BUILD.gn, inspector, sources
set(
    v8_inspector_sources
    src/inspector/custom-preview.cc
    src/inspector/custom-preview.h
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
    src/inspector/v8-heap-profiler-agent-impl.cc
    src/inspector/v8-heap-profiler-agent-impl.h
    src/inspector/v8-inspector-impl.cc
    src/inspector/v8-inspector-impl.h
    src/inspector/v8-inspector-session-impl.cc
    src/inspector/v8-inspector-session-impl.h
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
    src/inspector/value-mirror.cc
    src/inspector/value-mirror.h
    src/inspector/wasm-translation.cc
    src/inspector/wasm-translation.h

    native-gen/src/inspector/protocol/Console.cpp
    native-gen/src/inspector/protocol/Debugger.cpp
    native-gen/src/inspector/protocol/HeapProfiler.cpp
    native-gen/src/inspector/protocol/Profiler.cpp
    native-gen/src/inspector/protocol/Protocol.cpp
    native-gen/src/inspector/protocol/Runtime.cpp
    native-gen/src/inspector/protocol/Schema.cpp
)

add_library(v8_inspector ${v8_inspector_sources})

# See BUILD.gn, v8_libbase, sources
set(
    v8_libbase_sources
    src/base/adapters.h
    src/base/address-region.h
    src/base/atomic-utils.h
    src/base/atomicops.h
    src/base/atomicops_internals_atomicword_compat.h
    src/base/atomicops_internals_portable.h
    src/base/atomicops_internals_std.h
    src/base/base-export.h
    src/base/bits.cc
    src/base/bits.h
    src/base/bounded-page-allocator.cc
    src/base/bounded-page-allocator.h
    src/base/build_config.h
    src/base/compiler-specific.h
    src/base/cpu.cc
    src/base/cpu.h
    src/base/debug/stack_trace.cc
    src/base/debug/stack_trace.h
    src/base/division-by-constant.cc
    src/base/division-by-constant.h
    src/base/enum-set.h
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
    src/base/lsan-page-allocator.cc
    src/base/lsan-page-allocator.h
    src/base/macros.h
    src/base/once.cc
    src/base/once.h
    src/base/optional.h
    src/base/overflowing-math.h
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
    src/base/region-allocator.cc
    src/base/region-allocator.h
    src/base/ring-buffer.h
    src/base/safe_conversions.h
    src/base/safe_conversions_impl.h
    src/base/safe_math.h
    src/base/safe_math_impl.h
    src/base/small-vector.h
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

# See BUILD.gn, torque_base, sources
set(torque_base_sources
    src/torque/ast.h
    src/torque/cfg.cc
    src/torque/cfg.h
    src/torque/contextual.h
    src/torque/csa-generator.cc
    src/torque/csa-generator.h
    src/torque/declarable.cc
    src/torque/declarable.h
    src/torque/declaration-visitor.cc
    src/torque/declaration-visitor.h
    src/torque/declarations.cc
    src/torque/declarations.h
    src/torque/earley-parser.cc
    src/torque/earley-parser.h
    src/torque/file-visitor.cc
    src/torque/file-visitor.h
    src/torque/global-context.h
    src/torque/implementation-visitor.cc
    src/torque/implementation-visitor.h
    src/torque/instructions.cc
    src/torque/instructions.h
    src/torque/source-positions.cc
    src/torque/source-positions.h
    src/torque/torque-parser.cc
    src/torque/torque-parser.h
    src/torque/type-oracle.cc
    src/torque/type-oracle.h
    src/torque/types.cc
    src/torque/types.h
    src/torque/utils.cc
    src/torque/utils.h
  )

add_library(torque_base ${torque_base_sources})
target_link_libraries(torque_base PUBLIC v8_libbase)

# See BUILD.gn, torque, sources
add_executable(torque src/torque/torque.cc)
target_link_libraries(torque PRIVATE torque_base)

set(
    v8_extras_libraries_source
    ${CMAKE_CURRENT_BINARY_DIR}/extras-libraries.cc
)

find_package(PythonInterp 2 REQUIRED) # PYTHON_EXECUTABLE

add_custom_command(
    OUTPUT
        ${v8_extras_libraries_source}
    COMMAND
        ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
        ${v8_extras_libraries_source}
        EXTRAS
        ${CMAKE_CURRENT_LIST_DIR}/test/cctest/test-extra.js
    DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/tools/js2c.py
        ${CMAKE_CURRENT_LIST_DIR}/test/cctest/test-extra.js
    COMMENT
        "Generating ${v8_extras_libraries_source} file"
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
)
target_link_libraries(v8_extras_libraries PUBLIC v8_base)

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

target_link_libraries(v8_inspector PUBLIC v8_base)

# See BUILD.gn, torque_files
set(
    torque_files
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/base.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/frames.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/arguments.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-copywithin.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-filter.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-foreach.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-join.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-lastindexof.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-of.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-map.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-reverse.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-slice.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-splice.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/array-unshift.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/collections.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/data-view.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/extras-utils.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/object-fromentries.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/iterator.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/string-startswith.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/typed-array.tq
    ${CMAKE_CURRENT_LIST_DIR}/src/builtins/typed-array-createtypedarray.tq
    ${CMAKE_CURRENT_LIST_DIR}/test/torque/test-torque.tq
    ${CMAKE_CURRENT_LIST_DIR}/third_party/v8/builtins/array-sort.tq
)

# See BUILD.gn, torque_namespaces
set(
    torque_namespaces
    arguments
    array
    array-copywithin
    array-filter
    array-foreach
    array-join
    array-map
    array-of
    array-reverse
    array-slice
    array-splice
    array-unshift
    array-lastindexof
    base
    collections
    data-view
    extras-utils
    iterator
    object
    string
    test
    typed-array
    typed-array-createtypedarray
)

set(torque_generated_parentdir ${CMAKE_CURRENT_BINARY_DIR}/tgd)
set(torque_generated_dir ${torque_generated_parentdir}/torque-generated)

set(
    torque_output_files
    ${torque_generated_dir}/builtin-definitions-from-dsl.h
    ${torque_generated_dir}/class-definitions-from-dsl.h
)

foreach(ns IN LISTS torque_namespaces)
  list(
      APPEND
      torque_output_files
      ${torque_generated_dir}/builtins-${ns}-from-dsl-gen.cc
      ${torque_generated_dir}/builtins-${ns}-from-dsl-gen.h
  )
endforeach()

add_custom_command(
    COMMAND
        ${CMAKE_COMMAND}
        -E
        make_directory
        ${torque_generated_dir}
    COMMAND
        ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_LIST_DIR}/tools/run.py
        $<TARGET_FILE:torque>
        -o
        ${torque_generated_dir}
        ${torque_files}
    DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/tools/run.py
        torque
        ${torque_files}
    OUTPUT
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
    src/builtins/builtins-bigint-gen.cc
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
    src/builtins/builtins-microtask-queue-gen.cc
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
    src/code-stub-assembler.cc
    src/code-stub-assembler.h
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
    ${torque_output_files}
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
target_include_directories(
    v8_initializers
    PRIVATE
    ${torque_generated_parentdir}
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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/native-gen>
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
