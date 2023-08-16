// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNLPFORUE_init() {}
	NLPFORUE_API UFunction* Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature();
	NLPFORUE_API UFunction* Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_NLPFORUE;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_NLPFORUE()
	{
		if (!Z_Registration_Info_UPackage__Script_NLPFORUE.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/NLPFORUE",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0x18D3487B,
				0x89FF791B,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_NLPFORUE.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_NLPFORUE.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_NLPFORUE(Z_Construct_UPackage__Script_NLPFORUE, TEXT("/Script/NLPFORUE"), Z_Registration_Info_UPackage__Script_NLPFORUE, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x18D3487B, 0x89FF791B));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
