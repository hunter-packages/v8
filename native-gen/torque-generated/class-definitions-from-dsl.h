#ifndef V8_CLASS_BUILTIN_DEFINITIONS_FROM_DSL_H_
#define V8_CLASS_BUILTIN_DEFINITIONS_FROM_DSL_H_


#define HEAP_OBJECT_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kMapOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define INTERNAL_CLASS_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kAOffset, kTaggedSize) \
V(kBOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSARGUMENTS_OBJECT_WITH_LENGTH_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kLengthOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSARRAY_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kLengthOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSARRAY_BUFFER_FIELDS(V) \
V(kByteLengthOffset, kIntptrSize) \
V(kBackingStoreOffset, kSystemPointerSize) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSARRAY_BUFFER_VIEW_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kBufferOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kByteOffsetOffset, kIntptrSize) \
V(kByteLengthOffset, kIntptrSize) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSBOUND_FUNCTION_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kBoundTargetFunctionOffset, kTaggedSize) \
V(kBoundThisOffset, kTaggedSize) \
V(kBoundArgumentsOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSFUNCTION_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kSharedFunctionInfoOffset, kTaggedSize) \
V(kContextOffset, kTaggedSize) \
V(kFeedbackCellOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kCodeOffset, kTaggedSize) \
V(kPrototypeOrInitialMapOffset, kTaggedSize) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSOBJECT_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kElementsOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSRECEIVER_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kPropertiesOrHashOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define JSTYPED_ARRAY_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define SHARED_FUNCTION_INFO_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kFunctionDataOffset, kTaggedSize) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kNameOrScopeInfoOffset, kTaggedSize) \
V(kOuterScopeInfoOrFeedbackMetadataOffset, kTaggedSize) \
V(kScriptOrDebugInfoOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kLengthOffset, kUInt16Size) \
V(kFormalParameterCountOffset, kUInt16Size) \
V(kExpectedNofPropertiesOffset, kUInt8Size) \
V(kBuiltinFunctionIdOffset, kUInt8Size) \
V(kFunctionTokenOffsetOffset, kUInt16Size) \
V(kFlagsOffset, kInt32Size) \
V(kSize, 0) \

#define SHARED_FUNCTION_INFO_WITH_ID_FIELDS(V) \
V(kUniqueIdOffset, kInt32Size) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define SORT_STATE_FIELDS(V) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kReceiverOffset, kTaggedSize) \
V(kInitialReceiverMapOffset, kTaggedSize) \
V(kInitialReceiverLengthOffset, kTaggedSize) \
V(kUserCmpFnOffset, kTaggedSize) \
V(kSortComparePtrOffset, kTaggedSize) \
V(kLoadFnOffset, kTaggedSize) \
V(kStoreFnOffset, kTaggedSize) \
V(kCanUseSameAccessorFnOffset, kTaggedSize) \
V(kBailoutStatusOffset, kTaggedSize) \
V(kMinGallopOffset, kTaggedSize) \
V(kPendingRunsSizeOffset, kTaggedSize) \
V(kPendingRunsOffset, kTaggedSize) \
V(kTempArrayOffset, kTaggedSize) \
V(kAccessorOffset, kTaggedSize) \
V(kEndOfStrongFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \

#define TEST_CLASS_WITH_ALL_TYPES_FIELDS(V) \
V(kAOffset, kUInt8Size) \
V(kBOffset, kUInt8Size) \
V(kB2Offset, kUInt8Size) \
V(kB3Offset, kUInt8Size) \
V(kCOffset, kUInt16Size) \
V(kDOffset, kUInt16Size) \
V(kEOffset, kInt32Size) \
V(kFOffset, kInt32Size) \
V(kGOffset, kSystemPointerSize) \
V(kHOffset, kIntptrSize) \
V(kIOffset, kIntptrSize) \
V(kStartOfPointerFieldsOffset, 0) \
V(kStartOfWeakFieldsOffset, 0) \
V(kEndOfWeakFieldsOffset, 0) \
V(kStartOfStrongFieldsOffset, 0) \
V(kEndOfStrongFieldsOffset, 0) \
V(kEndOfTaggedFieldsOffset, 0) \
V(kSize, 0) \


#endif  // V8_CLASS_BUILTIN_DEFINITIONS_FROM_DSL_H_
