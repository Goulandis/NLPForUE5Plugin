// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NLPFORUE/Network/Soc.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSoc() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	NLPFORUE_API UClass* Z_Construct_UClass_ASoc();
	NLPFORUE_API UClass* Z_Construct_UClass_ASoc_NoRegister();
	NLPFORUE_API UFunction* Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature();
	NLPFORUE_API UFunction* Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_NLPFORUE();
// End Cross Module References
	struct Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics
	{
		struct _Script_NLPFORUE_eventOnRecvMsg_Parms
		{
			FString Data;
		};
		static const UECodeGen_Private::FStrPropertyParams NewProp_Data;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::NewProp_Data = { "Data", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(_Script_NLPFORUE_eventOnRecvMsg_Parms, Data), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::NewProp_Data,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_NLPFORUE, nullptr, "OnRecvMsg__DelegateSignature", nullptr, nullptr, sizeof(Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::_Script_NLPFORUE_eventOnRecvMsg_Parms), Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
void FOnRecvMsg_DelegateWrapper(const FMulticastScriptDelegate& OnRecvMsg, const FString& Data)
{
	struct _Script_NLPFORUE_eventOnRecvMsg_Parms
	{
		FString Data;
	};
	_Script_NLPFORUE_eventOnRecvMsg_Parms Parms;
	Parms.Data=Data;
	OnRecvMsg.ProcessMulticastDelegate<UObject>(&Parms);
}
	struct Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_NLPFORUE, nullptr, "OnConnected__DelegateSignature", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
void FOnConnected_DelegateWrapper(const FMulticastScriptDelegate& OnConnected)
{
	OnConnected.ProcessMulticastDelegate<UObject>(NULL);
}
	DEFINE_FUNCTION(ASoc::execCmdFormat)
	{
		P_GET_PROPERTY(FStrProperty,Z_Param_Cmd);
		P_GET_PROPERTY(FStrProperty,Z_Param_Type);
		P_GET_PROPERTY(FStrProperty,Z_Param_Data);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FString*)Z_Param__Result=P_THIS->CmdFormat(Z_Param_Cmd,Z_Param_Type,Z_Param_Data);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ASoc::execSocClose)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SocClose();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ASoc::execConnetLTP)
	{
		P_GET_PROPERTY(FStrProperty,Z_Param_InIP);
		P_GET_PROPERTY(FIntProperty,Z_Param_InPort);
		P_GET_PROPERTY(FStrProperty,Z_Param_SocDescription);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ConnetLTP(Z_Param_InIP,Z_Param_InPort,Z_Param_SocDescription);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ASoc::execSendCmd)
	{
		P_GET_PROPERTY(FStrProperty,Z_Param_Cmd);
		P_GET_PROPERTY(FStrProperty,Z_Param_Type);
		P_GET_PROPERTY(FStrProperty,Z_Param_Data);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->SendCmd(Z_Param_Cmd,Z_Param_Type,Z_Param_Data);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ASoc::execSendMsg)
	{
		P_GET_PROPERTY(FStrProperty,Z_Param_Msg);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->SendMsg(Z_Param_Msg);
		P_NATIVE_END;
	}
	void ASoc::StaticRegisterNativesASoc()
	{
		UClass* Class = ASoc::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CmdFormat", &ASoc::execCmdFormat },
			{ "ConnetLTP", &ASoc::execConnetLTP },
			{ "SendCmd", &ASoc::execSendCmd },
			{ "SendMsg", &ASoc::execSendMsg },
			{ "SocClose", &ASoc::execSocClose },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ASoc_CmdFormat_Statics
	{
		struct Soc_eventCmdFormat_Parms
		{
			FString Cmd;
			FString Type;
			FString Data;
			FString ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Cmd_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Cmd;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Type_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Type;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Data_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Data;
		static const UECodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Cmd_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Cmd = { "Cmd", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventCmdFormat_Parms, Cmd), METADATA_PARAMS(Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Cmd_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Cmd_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Type_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventCmdFormat_Parms, Type), METADATA_PARAMS(Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Type_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Type_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Data_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Data = { "Data", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventCmdFormat_Parms, Data), METADATA_PARAMS(Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Data_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Data_MetaData)) };
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventCmdFormat_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ASoc_CmdFormat_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Cmd,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Type,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_Data,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_CmdFormat_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_CmdFormat_Statics::Function_MetaDataParams[] = {
		{ "Category", "NLPFORUE|Network" },
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ASoc_CmdFormat_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ASoc, nullptr, "CmdFormat", nullptr, nullptr, sizeof(Z_Construct_UFunction_ASoc_CmdFormat_Statics::Soc_eventCmdFormat_Parms), Z_Construct_UFunction_ASoc_CmdFormat_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_CmdFormat_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ASoc_CmdFormat_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_CmdFormat_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ASoc_CmdFormat()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ASoc_CmdFormat_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ASoc_ConnetLTP_Statics
	{
		struct Soc_eventConnetLTP_Parms
		{
			FString InIP;
			int32 InPort;
			FString SocDescription;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InIP_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_InIP;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InPort_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_InPort;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SocDescription_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_SocDescription;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InIP_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InIP = { "InIP", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventConnetLTP_Parms, InIP), METADATA_PARAMS(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InIP_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InIP_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InPort_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InPort = { "InPort", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventConnetLTP_Parms, InPort), METADATA_PARAMS(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InPort_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InPort_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_SocDescription_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_SocDescription = { "SocDescription", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventConnetLTP_Parms, SocDescription), METADATA_PARAMS(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_SocDescription_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_SocDescription_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ASoc_ConnetLTP_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InIP,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_InPort,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_ConnetLTP_Statics::NewProp_SocDescription,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_ConnetLTP_Statics::Function_MetaDataParams[] = {
		{ "Category", "NLPFORUE|Network" },
		{ "CPP_Default_InIP", "127.0.0.1" },
		{ "CPP_Default_InPort", "7214" },
		{ "CPP_Default_SocDescription", "tlp client" },
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ASoc_ConnetLTP_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ASoc, nullptr, "ConnetLTP", nullptr, nullptr, sizeof(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::Soc_eventConnetLTP_Parms), Z_Construct_UFunction_ASoc_ConnetLTP_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_ConnetLTP_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ASoc_ConnetLTP()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ASoc_ConnetLTP_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ASoc_SendCmd_Statics
	{
		struct Soc_eventSendCmd_Parms
		{
			FString Cmd;
			FString Type;
			FString Data;
			bool ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Cmd_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Cmd;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Type_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Type;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Data_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Data;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Cmd_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Cmd = { "Cmd", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventSendCmd_Parms, Cmd), METADATA_PARAMS(Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Cmd_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Cmd_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Type_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventSendCmd_Parms, Type), METADATA_PARAMS(Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Type_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Type_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Data_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Data = { "Data", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventSendCmd_Parms, Data), METADATA_PARAMS(Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Data_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Data_MetaData)) };
	void Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((Soc_eventSendCmd_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(Soc_eventSendCmd_Parms), &Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ASoc_SendCmd_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Cmd,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Type,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_Data,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_SendCmd_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_SendCmd_Statics::Function_MetaDataParams[] = {
		{ "Category", "NLPFORUE|Network" },
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ASoc_SendCmd_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ASoc, nullptr, "SendCmd", nullptr, nullptr, sizeof(Z_Construct_UFunction_ASoc_SendCmd_Statics::Soc_eventSendCmd_Parms), Z_Construct_UFunction_ASoc_SendCmd_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendCmd_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ASoc_SendCmd_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendCmd_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ASoc_SendCmd()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ASoc_SendCmd_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ASoc_SendMsg_Statics
	{
		struct Soc_eventSendMsg_Parms
		{
			FString Msg;
			bool ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Msg_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_Msg;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_Msg_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_Msg = { "Msg", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(Soc_eventSendMsg_Parms, Msg), METADATA_PARAMS(Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_Msg_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_Msg_MetaData)) };
	void Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((Soc_eventSendMsg_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(Soc_eventSendMsg_Parms), &Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ASoc_SendMsg_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_Msg,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ASoc_SendMsg_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_SendMsg_Statics::Function_MetaDataParams[] = {
		{ "Category", "NLPFORUE|Network" },
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ASoc_SendMsg_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ASoc, nullptr, "SendMsg", nullptr, nullptr, sizeof(Z_Construct_UFunction_ASoc_SendMsg_Statics::Soc_eventSendMsg_Parms), Z_Construct_UFunction_ASoc_SendMsg_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendMsg_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ASoc_SendMsg_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SendMsg_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ASoc_SendMsg()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ASoc_SendMsg_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ASoc_SocClose_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ASoc_SocClose_Statics::Function_MetaDataParams[] = {
		{ "Category", "NLPFORUE|Network" },
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ASoc_SocClose_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ASoc, nullptr, "SocClose", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ASoc_SocClose_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ASoc_SocClose_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ASoc_SocClose()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ASoc_SocClose_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ASoc);
	UClass* Z_Construct_UClass_ASoc_NoRegister()
	{
		return ASoc::StaticClass();
	}
	struct Z_Construct_UClass_ASoc_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnRecvMsg_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnRecvMsg;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnConnected_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnConnected;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASoc_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_NLPFORUE,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_ASoc_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ASoc_CmdFormat, "CmdFormat" }, // 2052601130
		{ &Z_Construct_UFunction_ASoc_ConnetLTP, "ConnetLTP" }, // 2544914232
		{ &Z_Construct_UFunction_ASoc_SendCmd, "SendCmd" }, // 873873971
		{ &Z_Construct_UFunction_ASoc_SendMsg, "SendMsg" }, // 2014922531
		{ &Z_Construct_UFunction_ASoc_SocClose, "SocClose" }, // 260647406
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASoc_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Network/Soc.h" },
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASoc_Statics::NewProp_OnRecvMsg_MetaData[] = {
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ASoc_Statics::NewProp_OnRecvMsg = { "OnRecvMsg", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(ASoc, OnRecvMsg), Z_Construct_UDelegateFunction_NLPFORUE_OnRecvMsg__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_ASoc_Statics::NewProp_OnRecvMsg_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ASoc_Statics::NewProp_OnRecvMsg_MetaData)) }; // 2450955677
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASoc_Statics::NewProp_OnConnected_MetaData[] = {
		{ "ModuleRelativePath", "Network/Soc.h" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ASoc_Statics::NewProp_OnConnected = { "OnConnected", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(ASoc, OnConnected), Z_Construct_UDelegateFunction_NLPFORUE_OnConnected__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_ASoc_Statics::NewProp_OnConnected_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ASoc_Statics::NewProp_OnConnected_MetaData)) }; // 1314364669
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ASoc_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ASoc_Statics::NewProp_OnRecvMsg,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ASoc_Statics::NewProp_OnConnected,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASoc_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASoc>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ASoc_Statics::ClassParams = {
		&ASoc::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_ASoc_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_ASoc_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ASoc_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ASoc_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASoc()
	{
		if (!Z_Registration_Info_UClass_ASoc.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ASoc.OuterSingleton, Z_Construct_UClass_ASoc_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ASoc.OuterSingleton;
	}
	template<> NLPFORUE_API UClass* StaticClass<ASoc>()
	{
		return ASoc::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASoc);
	struct Z_CompiledInDeferFile_FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ASoc, ASoc::StaticClass, TEXT("ASoc"), &Z_Registration_Info_UClass_ASoc, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ASoc), 3226308504U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_2313054071(TEXT("/Script/NLPFORUE"),
		Z_CompiledInDeferFile_FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_NLPForUE5Plugin_Plugins_NLPFORUE_Source_NLPFORUE_Network_Soc_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
