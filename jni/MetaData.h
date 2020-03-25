#ifndef METADATA_H
#define METADATA_H

#include "libil2cpp-v24/il2cpp-metadata.h"
#include "libil2cpp-v24/class-internals.h"

namespace MetaData {
    kaddr s_GlobalMetadata;
    kaddr s_Il2CppCodeRegistration;
	kaddr s_Il2CppMetadataRegistration;

    Il2CppGlobalMetadataHeader s_GlobalMetadataHeader;
    Il2CppCodeRegistration s_CodeRegistration;
    Il2CppMetadataRegistration s_MetadataRegistration;

    //-----Code and MetaData Registration-----//
    const Il2CppType* GetIl2CppType(TypeIndex index){
		//printf("TypeIndex: %d\n", index);
        if (index >= s_MetadataRegistration.typesCount) {return nullptr;}
		kaddr typePtr = getPtr(((kaddr) s_MetadataRegistration.types) + (index * sizeof(kaddr)));
        return ReadPtr<Il2CppType>(typePtr);
    }

    int32_t GetFieldOffset(TypeIndex typeIndex, int32_t fieldIndexInType) {
		//printf("FieldOffset: TypeIndex(%d) , FType(%d)\n", typeIndex, fieldIndexInType);
        if (typeIndex > s_MetadataRegistration.typeDefinitionsSizesCount || fieldIndexInType > s_MetadataRegistration.fieldOffsetsCount) {return 0;}
		kaddr OffsetArr = getPtr(((kaddr) s_MetadataRegistration.fieldOffsets) + (typeIndex * sizeof(TypeIndex)));
		return Read<int>(OffsetArr + (fieldIndexInType * sizeof(int32_t)));
    }

    kaddr GetMethodPointer(MethodIndex index){
		//printf("MethodIndex: %d\n", index);
        if (index >= s_CodeRegistration.methodPointersCount) {return 0;}
        return getPtr(((kaddr) s_CodeRegistration.methodPointers) + (index * sizeof(kaddr)));
    }

    kaddr GetMetadataUsagePointer(uint32_t index){
        //printf("MetadataUsagePointerIndex: %d\n", index);
        if (index >= s_MetadataRegistration.metadataUsagesCount) {return 0;}
        return getPtr(((kaddr) s_MetadataRegistration.metadataUsages) + (index * sizeof(kaddr)));
    }

    //-----MetaData Counts-----//
    int getImageDefCount(){
        return s_GlobalMetadataHeader.imagesCount / sizeof(Il2CppImageDefinition);
    }

    int getMetadataUsageList(){
        return s_GlobalMetadataHeader.metadataUsageListsCount / sizeof(Il2CppMetadataUsageList);
    }

    //-----MetaData Definations-----//
	const Il2CppImageDefinition* GetImageDefinition(ImageIndex index){
		//printf("ImageIndex: %d\n", index);
        if (index >= getImageDefCount()) {return nullptr;}
        return ReadPtr<Il2CppImageDefinition>(s_GlobalMetadata + s_GlobalMetadataHeader.imagesOffset + (index * sizeof(Il2CppImageDefinition)));
    }

	const Il2CppTypeDefinition* GetTypeDefinition(TypeDefinitionIndex index){
		//printf("TypeDefinitionIndex: %d\n", index);
        if (index >= s_GlobalMetadataHeader.typeDefinitionsCount / sizeof(Il2CppTypeDefinition)) {return nullptr;}
        return ReadPtr<Il2CppTypeDefinition>(s_GlobalMetadata + s_GlobalMetadataHeader.typeDefinitionsOffset + (index * sizeof(Il2CppTypeDefinition)));
    }

    const Il2CppType* GetInterface(InterfacesIndex index){
		//printf("InterfacesIndex: %d ", index);
        if (index > s_GlobalMetadataHeader.interfacesCount / sizeof(TypeIndex)) {return nullptr;}
		TypeIndex typeindex = Read<TypeIndex>(s_GlobalMetadata + s_GlobalMetadataHeader.interfacesOffset + (index * sizeof(TypeIndex)));
		//printf("InterfacesTypeIndex: %d\n", typeindex);
		return GetIl2CppType(typeindex);
    }

    const Il2CppEventDefinition* GetEventDefinition(EventIndex index){
		//printf("EventIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.eventsCount / sizeof(Il2CppEventDefinition)) {return nullptr;}
        return ReadPtr<Il2CppEventDefinition>(s_GlobalMetadata + s_GlobalMetadataHeader.eventsOffset + (index * sizeof(Il2CppEventDefinition)));
    }

    const Il2CppFieldDefinition* GetFieldDefinition(FieldIndex index){
		//printf("FieldIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.fieldsCount / sizeof(Il2CppFieldDefinition)) {return nullptr;}
        return ReadPtr<Il2CppFieldDefinition>(s_GlobalMetadata + s_GlobalMetadataHeader.fieldsOffset + (index * sizeof(Il2CppFieldDefinition)));
    }

    const Il2CppMethodDefinition* GetMethodDefinition(MethodIndex index){
		//printf("MethodIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.methodsCount / sizeof(Il2CppMethodDefinition)) {return nullptr;}
        return ReadPtr<Il2CppMethodDefinition>(s_GlobalMetadata + s_GlobalMetadataHeader.methodsOffset + (index * sizeof(Il2CppMethodDefinition)));
    }

    const Il2CppPropertyDefinition* GetPropertyDefinition(PropertyIndex index){
		//printf("PropertyIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.propertiesCount / sizeof(Il2CppPropertyDefinition)) {return nullptr;}
        return ReadPtr<Il2CppPropertyDefinition>(s_GlobalMetadata + s_GlobalMetadataHeader.propertiesOffset + (index * sizeof(Il2CppPropertyDefinition)));
    }

    const Il2CppParameterDefinition* GetParameterDefinition(ParameterIndex index){
		//printf("ParameterIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.parametersCount / sizeof(Il2CppParameterDefinition)) {return nullptr;}
        return ReadPtr<Il2CppParameterDefinition>(s_GlobalMetadata + s_GlobalMetadataHeader.parametersOffset + (index * sizeof(Il2CppParameterDefinition)));
    }

    const Il2CppMetadataUsageList* GetMetadataUsageList(uint32_t index){
        //printf("MetadataUsageIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.metadataUsageListsCount / sizeof(Il2CppMetadataUsageList)) {return nullptr;}
        return ReadPtr<Il2CppMetadataUsageList>(s_GlobalMetadata + s_GlobalMetadataHeader.metadataUsageListsOffset + (index * sizeof(Il2CppMetadataUsageList)));
    }

    const Il2CppMetadataUsagePair* GetMetadataUsagePairList(uint32_t index){
        //printf("MetadataUsagePair: %d\n", index);
        if (index > s_GlobalMetadataHeader.metadataUsagePairsCount / sizeof(Il2CppMetadataUsagePair)) {return nullptr;}
        return ReadPtr<Il2CppMetadataUsagePair>(s_GlobalMetadata + s_GlobalMetadataHeader.metadataUsagePairsOffset + (index * sizeof(Il2CppMetadataUsagePair)));
    }

    const char* GetString(StringIndex index){
		//printf("StringIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.stringCount){return nullptr;}
		int count = 0, MAX_SIZE = 40;

		char* buff = ReadArr<char>(s_GlobalMetadata + s_GlobalMetadataHeader.stringOffset + index, (unsigned int) MAX_SIZE);
		while (buff[count] != '\0' && count < MAX_SIZE) {count++;}

		char* strings = new char[count];
		strncpy(strings, buff, (size_t) count);
		free(buff);

		return (const char*) strings;
    }

    const char* GetStringLiteral(StringIndex index){
        //printf("StringLiteralIndex: %d\n", index);
        if (index > s_GlobalMetadataHeader.stringLiteralCount){return nullptr;}
        const Il2CppStringLiteral* stringLiteral = ReadPtr<Il2CppStringLiteral>(s_GlobalMetadata + s_GlobalMetadataHeader.stringLiteralOffset + (index * sizeof(Il2CppStringLiteral)));
        return (const char*) ReadArr<char>(s_GlobalMetadata + s_GlobalMetadataHeader.stringLiteralDataOffset + stringLiteral->dataIndex, stringLiteral->length);
    }

    void init(kaddr globalMeta, kaddr MetaReg, kaddr CodeReg){
        s_GlobalMetadata = getPtr(getRealOffset(globalMeta));
        s_Il2CppCodeRegistration = getPtr(getRealOffset(CodeReg));
        s_Il2CppMetadataRegistration = getPtr(getRealOffset(MetaReg));
		s_GlobalMetadataHeader = Read<Il2CppGlobalMetadataHeader>(s_GlobalMetadata);
        s_CodeRegistration = Read<Il2CppCodeRegistration>(s_Il2CppCodeRegistration);
        s_MetadataRegistration = Read<Il2CppMetadataRegistration>(s_Il2CppMetadataRegistration);

		printf("MetaData Sanity: %x\n", s_GlobalMetadataHeader.sanity);
		printf("MetaData version: %d\n", s_GlobalMetadataHeader.version);
        printf("GlobalMetadata: %x\n", s_GlobalMetadata);
        printf("Il2CppCodeRegistration: %x\n", (unsigned int)(s_Il2CppCodeRegistration - libbase));
        printf("Il2CppMetadataRegistration: %x\n\n", (unsigned int)(s_Il2CppMetadataRegistration - libbase));
    }
}

#endif