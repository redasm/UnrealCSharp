﻿#include "FGeneratorCore.h"
#include "FDelegateGenerator.h"

FString FGeneratorCore::GetBasePath()
{
	return FPaths::Combine(FPaths::ProjectDir(), TEXT("Script"), TEXT("Proxy"));
}

FString FGeneratorCore::GetModuleName(const UField* InField)
{
	if (InField == nullptr)
	{
		return TEXT("");
	}

	auto ModuleName = InField->GetOuter() ? InField->GetOuter()->GetName() : TEXT("");

	if (InField->IsNative())
	{
		ModuleName = ModuleName.Replace(TEXT("/Script/"), TEXT("/"));
	}

	return ModuleName;
}

FString FGeneratorCore::GetPathNameAttribute(const UField* InField)
{
	if (InField == nullptr)
	{
		return TEXT("");
	}

	const auto ModuleName = InField->GetOuter() ? InField->GetOuter()->GetName() : TEXT("");

	const auto PathName = FString::Printf(TEXT(
		"%s%s"
	),
	                                      *InField->GetName(),
	                                      InField->IsNative()
		                                      ? TEXT("")
		                                      : *FString::Printf(TEXT(
			                                      ".%s_C"
		                                      ),
		                                                         *InField->GetName()));

	return FString::Printf(TEXT(
		"%s%s%s"
	),
	                       *ModuleName,
	                       *FString(InField->IsNative() ? TEXT(".") : TEXT("/")),
	                       *PathName);
}

FString FGeneratorCore::GetFullClass(const UStruct* InStruct)
{
	if (InStruct == nullptr)
	{
		return TEXT("");
	}

	return FString::Printf(TEXT(
		"%s%s"
	),
	                       InStruct->IsNative() ? InStruct->GetPrefixCPP() : TEXT(""),
	                       *InStruct->GetName());
}

FString FGeneratorCore::GetClassNameSpace(const UStruct* InStruct)
{
	if (InStruct == nullptr)
	{
		return TEXT("");
	}

	auto ModuleName = InStruct->GetOuter() ? InStruct->GetOuter()->GetName() : TEXT("");

	if (InStruct->IsNative())
	{
		ModuleName = ModuleName.Replace(TEXT("/Script/"), TEXT("/"));
	}

	return FString::Printf(TEXT(
		"%s%s"
	),
	                       TEXT("Script"),
	                       *ModuleName.Replace(TEXT("/"), TEXT(".")));
}

FString FGeneratorCore::GetFullClass(const UEnum* InEnum)
{
	if (InEnum == nullptr)
	{
		return TEXT("");
	}

	return InEnum->GetName();
}

FString FGeneratorCore::GetClassNameSpace(const UEnum* InStruct)
{
	if (InStruct == nullptr)
	{
		return "";
	}

	FString ModuleName = InStruct->GetOuter() ? InStruct->GetOuter()->GetName() : TEXT("");

	if (InStruct->IsNative())
	{
		ModuleName = ModuleName.Replace(TEXT("/Script/"), TEXT("/"));
	}

	return FString::Printf(TEXT(
		"%s%s"
	),
	                       TEXT("Script"),
	                       *ModuleName.Replace(TEXT("/"), TEXT(".")));
}

FString FGeneratorCore::GetFullClass(const FDelegateProperty* InDelegateProperty)
{
	if (InDelegateProperty == nullptr)
	{
		return TEXT("");
	}

	const auto SignatureFunction = InDelegateProperty->SignatureFunction;

	if (SignatureFunction == nullptr)
	{
		return TEXT("");
	}

	auto DelegateName = SignatureFunction->GetName();

	DelegateName.LeftChopInline(FString(HEADER_GENERATED_DELEGATE_SIGNATURE_SUFFIX).Len(), false);

	return FString::Printf(TEXT(
		"F%s"
	),
	                       *DelegateName);
}

FString FGeneratorCore::GetClassNameSpace(const FDelegateProperty* InDelegateProperty)
{
	if (InDelegateProperty == nullptr)
	{
		return TEXT("");
	}

	const auto SignatureFunction = InDelegateProperty->SignatureFunction;

	if (SignatureFunction == nullptr)
	{
		return TEXT("");
	}

	if (const auto Class = Cast<UClass>(SignatureFunction->GetOuter()))
	{
		return FString::Printf(TEXT(
			"%s.%s"
		),
		                       *GetClassNameSpace(Class),
		                       *SignatureFunction->GetOuter()->GetName());
	}

	if (const auto Package = Cast<UPackage>(SignatureFunction->GetOuter()))
	{
		const auto ModuleName = Package->GetName().Replace(TEXT("/Script/"), TEXT("/"));

		return FString::Printf(TEXT(
			"%s%s"
		),
		                       TEXT("Script"),
		                       *ModuleName.Replace(TEXT("/"), TEXT(".")));
	}

	return TEXT("");
}

FString FGeneratorCore::GetFullClass(const FMulticastDelegateProperty* InMulticastDelegateProperty)
{
	if (InMulticastDelegateProperty == nullptr)
	{
		return TEXT("");
	}

	const auto SignatureFunction = InMulticastDelegateProperty->SignatureFunction;

	if (SignatureFunction == nullptr)
	{
		return TEXT("");
	}

	auto DelegateName = SignatureFunction->GetName();

	DelegateName.LeftChopInline(FString(HEADER_GENERATED_DELEGATE_SIGNATURE_SUFFIX).Len(), false);

	return FString::Printf(TEXT(
		"F%s"
	),
	                       *DelegateName);
}

FString FGeneratorCore::GetClassNameSpace(const FMulticastDelegateProperty* InMulticastDelegateProperty)
{
	if (InMulticastDelegateProperty == nullptr)
	{
		return TEXT("");
	}

	const auto SignatureFunction = InMulticastDelegateProperty->SignatureFunction;

	if (SignatureFunction == nullptr)
	{
		return TEXT("");
	}

	if (const auto Class = Cast<UClass>(SignatureFunction->GetOuter()))
	{
		return FString::Printf(TEXT(
			"%s.%s"
		),
		                       *GetClassNameSpace(Class),
		                       *SignatureFunction->GetOuter()->GetName());
	}

	if (const auto Package = Cast<UPackage>(SignatureFunction->GetOuter()))
	{
		const auto ModuleName = Package->GetName().Replace(TEXT("/Script/"), TEXT("/"));

		return FString::Printf(TEXT(
			"%s%s"
		),
		                       TEXT("Script"),
		                       *ModuleName.Replace(TEXT("/"), TEXT(".")));
	}

	return TEXT("");
}

FString FGeneratorCore::GetPropertyType(FProperty* Property)
{
	if (Property == nullptr) return "";

	if (const auto ByteProperty = CastField<FByteProperty>(Property))
		return ByteProperty->Enum != nullptr
			       ? GetFullClass(ByteProperty->Enum)
			       : TEXT("Byte");

	if (CastField<FUInt16Property>(Property)) return TEXT("UInt16");

	if (CastField<FUInt32Property>(Property)) return TEXT("UInt32");

	if (CastField<FUInt64Property>(Property)) return TEXT("UInt64");

	if (CastField<FInt8Property>(Property)) return TEXT("SByte");

	if (CastField<FInt16Property>(Property)) return TEXT("Int16");

	if (CastField<FIntProperty>(Property)) return TEXT("Int32");

	if (CastField<FInt64Property>(Property)) return TEXT("Int64");

	if (CastField<FBoolProperty>(Property)) return TEXT("Boolean");

	if (CastField<FFloatProperty>(Property)) return TEXT("Single");

	// @TODO
	if (CastField<FClassProperty>(Property)) return TEXT("Object");

	if (const auto ObjectProperty = CastField<FObjectProperty>(Property))
		return GetFullClass(
			ObjectProperty->PropertyClass);

	if (CastField<FNameProperty>(Property)) return TEXT("FName");

	if (const auto DelegateProperty = CastField<FDelegateProperty>(Property)) return GetFullClass(DelegateProperty);

	// @TODO
	if (CastField<FInterfaceProperty>(Property)) return TEXT("Object");

	if (const auto StructProperty = CastField<FStructProperty>(Property)) return GetFullClass(StructProperty->Struct);

	if (const auto ArrayProperty = CastField<FArrayProperty>(Property))
	{
		FDelegateGenerator::Generator(ArrayProperty->Inner);

		return FString::Printf(TEXT(
			"TArray<%s>"
		),
		                       *GetPropertyType(ArrayProperty->Inner)
		);
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property)) return GetFullClass(EnumProperty->GetEnum());

	if (CastField<FStrProperty>(Property)) return TEXT("FString");

	if (CastField<FTextProperty>(Property)) return TEXT("FText");

	if (const auto MulticastDelegateProperty = CastField<FMulticastDelegateProperty>(Property))
		return GetFullClass(
			MulticastDelegateProperty);

	// @TODO
	if (CastField<FMulticastInlineDelegateProperty>(Property)) return TEXT("Object");

	// @TODO
	if (CastField<FMulticastSparseDelegateProperty>(Property)) return TEXT("Object");

	// @TODO
	if (CastField<FWeakObjectProperty>(Property)) return TEXT("Object");

	// @TODO
	if (CastField<FLazyObjectProperty>(Property)) return TEXT("Object");

	// @TODO
	if (CastField<FSoftObjectProperty>(Property)) return TEXT("Object");

	if (CastField<FDoubleProperty>(Property)) return TEXT("Double");

	if (const auto MapProperty = CastField<FMapProperty>(Property))
	{
		FDelegateGenerator::Generator(MapProperty->KeyProp);

		FDelegateGenerator::Generator(MapProperty->ValueProp);

		return FString::Printf(TEXT(
			"TMap<%s,%s>"
		),
		                       *GetPropertyType(MapProperty->KeyProp),
		                       *GetPropertyType(MapProperty->ValueProp)
		);
	}

	if (const auto SetProperty = CastField<FSetProperty>(Property))
	{
		FDelegateGenerator::Generator(SetProperty->ElementProp);

		return FString::Printf(TEXT(
			"TSet<%s>"
		),
		                       *GetPropertyType(SetProperty->ElementProp)
		);
	}

	// @TODO
	if (CastField<FFieldPathProperty>(Property)) return TEXT("Object");

	return TEXT("");
}

TSet<FString> FGeneratorCore::GetPropertyTypeNameSpace(FProperty* Property)
{
	if (Property == nullptr) return {""};

	if (const auto ByteProperty = CastField<FByteProperty>(Property))
		return {
			ByteProperty->Enum != nullptr ? GetClassNameSpace(ByteProperty->Enum) : TEXT("System")
		};

	if (CastField<FUInt16Property>(Property)) return {TEXT("System")};

	if (CastField<FUInt32Property>(Property)) return {TEXT("System")};

	if (CastField<FUInt64Property>(Property)) return {TEXT("System")};

	if (CastField<FInt8Property>(Property)) return {TEXT("System")};

	if (CastField<FInt16Property>(Property)) return {TEXT("System")};

	if (CastField<FIntProperty>(Property)) return {TEXT("System")};

	if (CastField<FInt64Property>(Property)) return {TEXT("System")};

	if (CastField<FBoolProperty>(Property)) return {TEXT("System")};

	if (CastField<FFloatProperty>(Property)) return {TEXT("System")};

	// @TODO
	if (CastField<FClassProperty>(Property)) return {TEXT("System")};

	if (const auto ObjectProperty = CastField<FObjectProperty>(Property))
		return {
			GetClassNameSpace(ObjectProperty->PropertyClass)
		};

	if (CastField<FNameProperty>(Property)) return {TEXT("Script.Common")};

	if (const auto DelegateProperty = CastField<FDelegateProperty>(Property))
		return {
			GetClassNameSpace(DelegateProperty)
		};

	// @TODO
	if (CastField<FInterfaceProperty>(Property)) return {TEXT("System")};

	if (const auto StructProperty = CastField<FStructProperty>(Property))
		return {
			GetClassNameSpace(StructProperty->Struct)
		};

	if (const auto ArrayProperty = CastField<FArrayProperty>(Property))
		return
			GetPropertyTypeNameSpace(ArrayProperty->Inner).Union({TEXT("Script.Common")});

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
		return {
			GetClassNameSpace(EnumProperty->GetEnum())
		};

	if (CastField<FStrProperty>(Property)) return {"Script.Common"};

	if (CastField<FTextProperty>(Property)) return {"Script.Common"};

	if (const auto MulticastDelegateProperty = CastField<FMulticastDelegateProperty>(Property))
		return {
			GetClassNameSpace(MulticastDelegateProperty)
		};

	// @TODO
	if (CastField<FMulticastInlineDelegateProperty>(Property)) return {TEXT("System")};

	// @TODO
	if (CastField<FMulticastSparseDelegateProperty>(Property)) return {TEXT("System")};

	// @TODO
	if (CastField<FWeakObjectProperty>(Property)) return {TEXT("System")};

	// @TODO
	if (CastField<FLazyObjectProperty>(Property)) return {TEXT("System")};

	// @TODO
	if (CastField<FSoftObjectProperty>(Property)) return {TEXT("System")};

	if (CastField<FDoubleProperty>(Property)) return {TEXT("System")};

	if (const auto MapProperty = CastField<FMapProperty>(Property))
		return
			GetPropertyTypeNameSpace(MapProperty->KeyProp).Union(GetPropertyTypeNameSpace(MapProperty->ValueProp)).
			                                               Union({
				                                               TEXT("Script.Common")
			                                               });

	if (const auto SetProperty = CastField<FSetProperty>(Property))
		return
			GetPropertyTypeNameSpace(SetProperty->ElementProp).Union({TEXT("Script.Common")});

	// @TODO
	if (CastField<FFieldPathProperty>(Property)) return {TEXT("System")};

	return {TEXT("")};
}

FString FGeneratorCore::GetGetAccessorType(FProperty* Property)
{
	if (CastField<FByteProperty>(Property))
	{
		return TEXT("Byte");
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
	{
		return *GetPropertyType(EnumProperty->GetUnderlyingProperty());
	}

	return *GetPropertyType(Property);
}

FString FGeneratorCore::GetGetAccessorReturnParamName(FProperty* Property)
{
	if (const auto ByteProperty = CastField<FByteProperty>(Property))
	{
		if (ByteProperty->Enum != nullptr)
		{
			return FString::Printf(TEXT(
				"(%s) value"
			),
			                       *GetFullClass(ByteProperty->Enum));
		}
		else
		{
			return TEXT("value");
		}
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
	{
		return FString::Printf(TEXT(
			"(%s) value"
		),
		                       *GetFullClass(EnumProperty->GetEnum()));
	}

	return TEXT("value");
}

FString FGeneratorCore::GetSetAccessorParamName(FProperty* Property)
{
	if (CastField<FByteProperty>(Property))
	{
		return TEXT("(Byte) value");
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
	{
		return FString::Printf(TEXT(
			"(%s) value"
		),
		                       *GetPropertyType(EnumProperty->GetUnderlyingProperty()));
	}

	return TEXT("value");
}

bool FGeneratorCore::IsSafeProperty(FProperty* Property)
{
	if (CastField<FByteProperty>(Property) || CastField<FUInt16Property>(Property) ||
		CastField<FUInt32Property>(Property) || CastField<FUInt64Property>(Property) ||
		CastField<FInt8Property>(Property) || CastField<FInt16Property>(Property) ||
		CastField<FIntProperty>(Property) || CastField<FInt64Property>(Property) ||
		CastField<FBoolProperty>(Property) || CastField<FFloatProperty>(Property) ||
		CastField<FEnumProperty>(Property) || CastField<FDoubleProperty>(Property))
	{
		return false;
	}

	return true;
}

FString FGeneratorCore::GetOutParamString(FProperty* Property, const uint32 Index)
{
	if (Property == nullptr) return TEXT("");

	if (const auto ByteProperty = CastField<FByteProperty>(Property))
	{
		if (ByteProperty->Enum != nullptr)
		{
			return FString::Printf(TEXT(
				"(%s) (*(Byte*)((IntPtr) __OutValue.Value[%d]).ToPointer())"
			),
			                       *GetPropertyType(Property), Index);
		}
		else
		{
			return FString::Printf(TEXT(
				"*(%s*) ((IntPtr) __OutValue.Value[%d]).ToPointer()"
			),
			                       *GetPropertyType(Property),
			                       Index);
		}
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
	{
		return FString::Printf(TEXT(
			"(%s) (*(%s*)((IntPtr) __OutValue.Value[%d]).ToPointer())"
		),
		                       *GetPropertyType(Property),
		                       *GetPropertyType(EnumProperty->GetUnderlyingProperty()),
		                       Index);
	}

	if (CastField<FUInt16Property>(Property) || CastField<FUInt32Property>(Property) ||
		CastField<FUInt64Property>(Property) || CastField<FInt8Property>(Property) ||
		CastField<FInt16Property>(Property) || CastField<FIntProperty>(Property) ||
		CastField<FInt64Property>(Property) || CastField<FBoolProperty>(Property) ||
		CastField<FFloatProperty>(Property) || CastField<FDoubleProperty>(Property))
	{
		return FString::Printf(TEXT(
			"*(%s*) ((IntPtr) __OutValue.Value[%d]).ToPointer()"
		),
		                       *GetPropertyType(Property),
		                       Index);
	}

	return FString::Printf(TEXT(
		"(%s) __OutValue.Value[%d]"
	),
	                       *GetPropertyType(Property),
	                       Index);
}

FString FGeneratorCore::GetParamName(FProperty* Property)
{
	if (const auto ByteProperty = CastField<FByteProperty>(Property))
	{
		if (ByteProperty->Enum != nullptr)
		{
			return FString::Printf(TEXT(
				"(Byte) %s"
			),
			                       *ByteProperty->GetName());
		}
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
	{
		return FString::Printf(TEXT(
			"(%s) %s"
		),
		                       *GetPropertyType(EnumProperty->GetUnderlyingProperty()),
		                       *EnumProperty->GetName());
	}

	return Property->GetName();
}

FString FGeneratorCore::GetReturnParamType(FProperty* Property)
{
	if (CastField<FByteProperty>(Property))
	{
		return TEXT("Byte");
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
	{
		return GetPropertyType(EnumProperty->GetUnderlyingProperty());
	}

	return GetPropertyType(Property);
}

FString FGeneratorCore::GetReturnParamName(FProperty* Property)
{
	if (const auto ByteProperty = CastField<FByteProperty>(Property))
	{
		if (ByteProperty->Enum != nullptr)
		{
			return FString::Printf(TEXT(
				"(%s) __ReturnValue"
			),
			                       *GetFullClass(ByteProperty->Enum));
		}
	}

	if (const auto EnumProperty = CastField<FEnumProperty>(Property))
	{
		return FString::Printf(TEXT(
			"(%s) __ReturnValue"
		),
		                       *GetFullClass(EnumProperty->GetEnum()));
	}

	return TEXT("__ReturnValue");
}

bool FGeneratorCore::SaveStringToFile(const FString& FileName, const FString& String)
{
	auto& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	const auto DirectoryName = FPaths::GetPath(FileName);

	if (!PlatformFile.DirectoryExists(*DirectoryName))
	{
		PlatformFile.CreateDirectoryTree(*DirectoryName);
	}

	const auto FileManager = &IFileManager::Get();

	return FFileHelper::SaveStringToFile(String, *FileName, FFileHelper::EEncodingOptions::ForceUTF8, FileManager,
	                                     FILEWRITE_None);
}
