// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Network/Soc.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef NLPFORUE_Soc_generated_h
#error "Soc.generated.h already included, missing '#pragma once' in Soc.h"
#endif
#define NLPFORUE_Soc_generated_h

#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_8_DELEGATE \
NLPFORUE_API void FOnRecvMsg_DelegateWrapper(const FMulticastScriptDelegate& OnRecvMsg, const FString& Data);


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_9_DELEGATE \
NLPFORUE_API void FOnConnected_DelegateWrapper(const FMulticastScriptDelegate& OnConnected);


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_SPARSE_DATA
#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSocClose); \
	DECLARE_FUNCTION(execConnetLTP); \
	DECLARE_FUNCTION(execSendCmd); \
	DECLARE_FUNCTION(execSendMsg);


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSocClose); \
	DECLARE_FUNCTION(execConnetLTP); \
	DECLARE_FUNCTION(execSendCmd); \
	DECLARE_FUNCTION(execSendMsg);


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_ACCESSORS
#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesASoc(); \
	friend struct Z_Construct_UClass_ASoc_Statics; \
public: \
	DECLARE_CLASS(ASoc, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/NLPFORUE"), NO_API) \
	DECLARE_SERIALIZER(ASoc)


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_INCLASS \
private: \
	static void StaticRegisterNativesASoc(); \
	friend struct Z_Construct_UClass_ASoc_Statics; \
public: \
	DECLARE_CLASS(ASoc, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/NLPFORUE"), NO_API) \
	DECLARE_SERIALIZER(ASoc)


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ASoc(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ASoc) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASoc); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASoc); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASoc(ASoc&&); \
	NO_API ASoc(const ASoc&); \
public:


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASoc(ASoc&&); \
	NO_API ASoc(const ASoc&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASoc); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASoc); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ASoc)


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_11_PROLOG
#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_SPARSE_DATA \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_RPC_WRAPPERS \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_ACCESSORS \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_INCLASS \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_SPARSE_DATA \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_ACCESSORS \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_INCLASS_NO_PURE_DECLS \
	FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> NLPFORUE_API UClass* StaticClass<class ASoc>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
