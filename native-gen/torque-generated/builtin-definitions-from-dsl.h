#ifndef V8_BUILTINS_BUILTIN_DEFINITIONS_FROM_DSL_H_
#define V8_BUILTINS_BUILTIN_DEFINITIONS_FROM_DSL_H_

#define BUILTIN_LIST_FROM_DSL(CPP, API, TFJ, TFC, TFS, TFH, ASM) \
TFJ(ArrayPrototypeCopyWithin, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayFilterLoopEagerDeoptContinuation, 6, kReceiver, kCallback, kThisArg, kArray, kInitialK, kLength, kInitialTo) \
TFJ(ArrayFilterLoopLazyDeoptContinuation, 8, kReceiver, kCallback, kThisArg, kArray, kInitialK, kLength, kValueK, kInitialTo, kResult) \
TFS(ArrayFilterLoopContinuation, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLength, kInitialTo) \
TFJ(ArrayFilter, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayForEachLoopEagerDeoptContinuation, 4, kReceiver, kCallback, kThisArg, kInitialK, kLength) \
TFJ(ArrayForEachLoopLazyDeoptContinuation, 5, kReceiver, kCallback, kThisArg, kInitialK, kLength, kResult) \
TFS(ArrayForEachLoopContinuation, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLen, kTo) \
TFJ(ArrayForEach, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFS(LoadJoinElement20ATDictionaryElements, kReceiver, kK) \
TFS(LoadJoinElement25ATFastSmiOrObjectElements, kReceiver, kK) \
TFS(LoadJoinElement20ATFastDoubleElements, kReceiver, kK) \
TFS(ConvertToLocaleString, kElement, kLocales, kOptions) \
TFS(JoinStackPush, kStack, kReceiver) \
TFS(JoinStackPop, kStack, kReceiver) \
TFJ(ArrayPrototypeJoin, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayPrototypeToLocaleString, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayPrototypeToString, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(TypedArrayPrototypeJoin, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(TypedArrayPrototypeToLocaleString, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayPrototypeLastIndexOf, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayOf, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayMapLoopEagerDeoptContinuation, 5, kReceiver, kCallback, kThisArg, kArray, kInitialK, kLength) \
TFJ(ArrayMapLoopLazyDeoptContinuation, 6, kReceiver, kCallback, kThisArg, kArray, kInitialK, kLength, kResult) \
TFS(ArrayMapLoopContinuation, kReceiver, kCallbackfn, kThisArg, kArray, kO, kInitialK, kLength, kInitialTo) \
TFJ(ArrayMap, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayPrototypeReverse, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayPrototypeSlice, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayPrototypeSplice, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ArrayPrototypeUnshift, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetBuffer, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetByteLength, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetByteOffset, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetUint8, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetInt8, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetUint16, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetInt16, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetUint32, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetInt32, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetFloat32, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetFloat64, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetBigUint64, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeGetBigInt64, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetUint8, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetInt8, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetUint16, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetInt16, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetUint32, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetInt32, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetFloat32, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetFloat64, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetBigUint64, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(DataViewPrototypeSetBigInt64, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ExtrasUtilsCreatePrivateSymbol, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ExtrasUtilsMarkPromiseAsHandled, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ExtrasUtilsPromiseState, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(ObjectFromEntries, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFJ(StringPrototypeStartsWith, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFS(TypedArrayMergeSort, kArray, kComparefn, kSource, kFrom, kTo, kTarget) \
TFJ(TypedArrayPrototypeSort, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFS(CreateTypedArray, kTarget, kNewTarget, kArg1, kArg2, kArg3) \
TFS(GenericBuiltinTest20UT5ATSmi10HeapObject, kParam) \
TFS(TestHelperPlus1, kX) \
TFS(TestHelperPlus2, kX) \
TFS(Load23ATFastPackedSmiElements, kSortState, kIndex) \
TFS(Load25ATFastSmiOrObjectElements, kSortState, kIndex) \
TFS(Load20ATFastDoubleElements, kSortState, kIndex) \
TFS(Load20ATDictionaryElements, kSortState, kIndex) \
TFS(Load19ATTempArrayElements, kSortState, kIndex) \
TFS(Store23ATFastPackedSmiElements, kSortState, kIndex, kValue) \
TFS(Store25ATFastSmiOrObjectElements, kSortState, kIndex, kValue) \
TFS(Store20ATFastDoubleElements, kSortState, kIndex, kValue) \
TFS(Store20ATDictionaryElements, kSortState, kIndex, kValue) \
TFS(Store19ATTempArrayElements, kSortState, kIndex, kValue) \
TFS(SortCompareDefault, kComparefn, kX, kY) \
TFS(SortCompareUserFn, kComparefn, kX, kY) \
TFS(CanUseSameAccessor25ATGenericElementsAccessor, kReceiver, kInitialReceiverMap, kInitialReceiverLength) \
TFS(CanUseSameAccessor20ATDictionaryElements, kReceiver, kInitialReceiverMap, kInitialReceiverLength) \
TFS(CopyFromTempArray, kSortState, kDstPos, kSrcPos, kLength) \
TFS(CopyWithinSortArray, kSortState, kSrcPos, kDstPos, kLength) \
TFS(BinaryInsertionSort, kSortState, kLow, kStartArg, kHigh) \
TFS(MergeAt, kSortState, kI) \
TFS(GallopLeft, kSortState, kLoad, kKey, kBase, kLength, kHint) \
TFS(GallopRight, kSortState, kLoad, kKey, kBase, kLength, kHint) \
TFS(ArrayTimSort, kSortState, kLength) \
TFJ(ArrayPrototypeSort, SharedFunctionInfo::kDontAdaptArgumentsSentinel) \
TFS(LoadJoinElement25ATGenericElementsAccessor, kReceiver, kK) \
TFS(LoadJoinTypedElement17ATFixedInt32Array, kReceiver, kK) \
TFS(LoadJoinTypedElement19ATFixedFloat32Array, kReceiver, kK) \
TFS(LoadJoinTypedElement19ATFixedFloat64Array, kReceiver, kK) \
TFS(LoadJoinTypedElement24ATFixedUint8ClampedArray, kReceiver, kK) \
TFS(LoadJoinTypedElement21ATFixedBigUint64Array, kReceiver, kK) \
TFS(LoadJoinTypedElement20ATFixedBigInt64Array, kReceiver, kK) \
TFS(LoadJoinTypedElement17ATFixedUint8Array, kReceiver, kK) \
TFS(LoadJoinTypedElement16ATFixedInt8Array, kReceiver, kK) \
TFS(LoadJoinTypedElement18ATFixedUint16Array, kReceiver, kK) \
TFS(LoadJoinTypedElement17ATFixedInt16Array, kReceiver, kK) \
TFS(LoadJoinTypedElement18ATFixedUint32Array, kReceiver, kK) \
TFS(LoadFixedElement17ATFixedInt32Array, kArray, kIndex) \
TFS(StoreFixedElement17ATFixedInt32Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement19ATFixedFloat32Array, kArray, kIndex) \
TFS(StoreFixedElement19ATFixedFloat32Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement19ATFixedFloat64Array, kArray, kIndex) \
TFS(StoreFixedElement19ATFixedFloat64Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement24ATFixedUint8ClampedArray, kArray, kIndex) \
TFS(StoreFixedElement24ATFixedUint8ClampedArray, kArray, kIndex, kValue) \
TFS(LoadFixedElement21ATFixedBigUint64Array, kArray, kIndex) \
TFS(StoreFixedElement21ATFixedBigUint64Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement20ATFixedBigInt64Array, kArray, kIndex) \
TFS(StoreFixedElement20ATFixedBigInt64Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement17ATFixedUint8Array, kArray, kIndex) \
TFS(StoreFixedElement17ATFixedUint8Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement16ATFixedInt8Array, kArray, kIndex) \
TFS(StoreFixedElement16ATFixedInt8Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement18ATFixedUint16Array, kArray, kIndex) \
TFS(StoreFixedElement18ATFixedUint16Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement17ATFixedInt16Array, kArray, kIndex) \
TFS(StoreFixedElement17ATFixedInt16Array, kArray, kIndex, kValue) \
TFS(LoadFixedElement18ATFixedUint32Array, kArray, kIndex) \
TFS(StoreFixedElement18ATFixedUint32Array, kArray, kIndex, kValue) \
TFS(GenericBuiltinTest5ATSmi, kParam) \
TFS(CanUseSameAccessor20ATFastDoubleElements, kReceiver, kInitialReceiverMap, kInitialReceiverLength) \
TFS(CanUseSameAccessor23ATFastPackedSmiElements, kReceiver, kInitialReceiverMap, kInitialReceiverLength) \
TFS(CanUseSameAccessor25ATFastSmiOrObjectElements, kReceiver, kInitialReceiverMap, kInitialReceiverLength) \
TFS(Load25ATGenericElementsAccessor, kSortState, kIndex) \
TFS(Store25ATGenericElementsAccessor, kSortState, kIndex, kValue) \

#define TORQUE_FUNCTION_POINTER_TYPE_TO_BUILTIN_MAP(V) \
  V(0,LoadJoinElement20ATDictionaryElements)\
  V(1,LoadFixedElement17ATFixedInt32Array)\
  V(2,StoreFixedElement17ATFixedInt32Array)\
  V(3,GenericBuiltinTest20UT5ATSmi10HeapObject)\
  V(4,Load23ATFastPackedSmiElements)\
  V(5,Store23ATFastPackedSmiElements)\
  V(6,CanUseSameAccessor25ATGenericElementsAccessor)\
  V(7,SortCompareDefault)\
  V(8,TestHelperPlus1)\
  V(9,GenericBuiltinTest5ATSmi)\

#endif  // V8_BUILTINS_BUILTIN_DEFINITIONS_FROM_DSL_H_
