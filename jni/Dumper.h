#ifndef DUMPER_H
#define DUMPER_H

#include "libil2cpp-v24/blob.h"
#include "libil2cpp-v24/metadata.h"
#include "libil2cpp-v24/tabledefs.h"

using namespace std;

namespace Dumper {
    ofstream dumpcs;
    //ofstream genscript;

    const char * IL2CppTypeEnumToString(Il2CppTypeEnum type) {
        switch (type) {
            case IL2CPP_TYPE_END:  /* End of List */
                return "LIST_END";
            case IL2CPP_TYPE_VOID:
                return "void";
            case IL2CPP_TYPE_BOOLEAN:
                return "bool";
            case IL2CPP_TYPE_CHAR:
                return "char";
            case IL2CPP_TYPE_I1:
                return "sbyte";
            case IL2CPP_TYPE_U1:
                return "byte";
            case IL2CPP_TYPE_I2:
                return "short";
            case IL2CPP_TYPE_U2:
                return "ushort";
            case IL2CPP_TYPE_I4:
                return "int";
            case IL2CPP_TYPE_U4:
                return "uint";
            case IL2CPP_TYPE_I8:
                return "long";
            case IL2CPP_TYPE_U8:
                return "ulong";
            case IL2CPP_TYPE_R4:
                return "float";
            case IL2CPP_TYPE_R8:
                return "double";
            case IL2CPP_TYPE_STRING:
                return "string";
            case IL2CPP_TYPE_PTR:
                return "Ptr";
            case IL2CPP_TYPE_BYREF:
                return "REF";
            case IL2CPP_TYPE_VALUETYPE:
                return "ValueType";
            case IL2CPP_TYPE_CLASS:
                return "Class";
            case IL2CPP_TYPE_VAR:
                return "T";
            case IL2CPP_TYPE_ARRAY:
                return "Array";
            case IL2CPP_TYPE_GENERICINST:
                return "Generic";
            case IL2CPP_TYPE_TYPEDBYREF:
                return "System.TypedReference";
            case IL2CPP_TYPE_I:
                return "IntPtr";
            case IL2CPP_TYPE_U:
                return "UIntPtr";
            case IL2CPP_TYPE_FNPTR:
                return "FuncPtr";
            case IL2CPP_TYPE_OBJECT:
                return "object";
            case IL2CPP_TYPE_SZARRAY:
                return "SZARRAY";
            case IL2CPP_TYPE_MVAR:
                return "T";
            case IL2CPP_TYPE_CMOD_REQD:
                return "CMOD_REQD";
            case IL2CPP_TYPE_CMOD_OPT:
                return "CMOD_OPT";
            case IL2CPP_TYPE_INTERNAL:
                return "internal_type";
            case IL2CPP_TYPE_MODIFIER:
                return "modifier";
            case IL2CPP_TYPE_SENTINEL:
                return "sentinel";
            case IL2CPP_TYPE_PINNED:
                return "pinned";
            case IL2CPP_TYPE_ENUM:
                return "enum";
        }
        return "";
    }

    string vStringJoin(const vector<string> &v, string delimiter) {
        string ret;
        for (int i = 0; i < v.size(); i++) {
            if (v[i].empty()) continue;
            ret += v[i];
            if (i < v.size() - 1) {
                ret += delimiter;
            }
        }
        return ret;
    }

    string GetIl2CppTypeName(const Il2CppType *type);

    string GetIl2CppTypeDefName(const Il2CppTypeDefinition *typeDef) {
        string ret;
        TypeIndex idx = typeDef->declaringTypeIndex;
        if (idx != kTypeIndexInvalid) {
            ret += GetIl2CppTypeName(MetaData::GetIl2CppType(idx)) + ".";
        }
        ret += MetaData::GetString(typeDef->nameIndex);
        return ret;
    }

    string GetIl2CppGenericTypeParams(const Il2CppGenericInst *genericInst) {
        if (!genericInst || genericInst->type_argc <= 0)
            return "";
        string ret("<");
        vector<string> typeNames;
        kaddr type_argvArr = getPtr((kaddr) genericInst->type_argv);
        for (uint i = 0; i < genericInst->type_argc; ++i) {
            const Il2CppType* oriType = ReadPtr<Il2CppType>(type_argvArr + (i * sizeof(kaddr)));
            typeNames.push_back(GetIl2CppTypeName(oriType));
        }
        ret += vStringJoin(typeNames, ", ");
        ret += ">";
        return ret;
    }

    string GetModifiers(const Il2CppMethodDefinition *methodDef) {
        string str;
        switch (methodDef->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) {
            case METHOD_ATTRIBUTE_PRIVATE:
                str += "private ";
                break;
            case METHOD_ATTRIBUTE_PUBLIC:
                str += "public ";
                break;
            case METHOD_ATTRIBUTE_FAMILY:
                str += "protected ";
                break;
            case METHOD_ATTRIBUTE_ASSEM:
            case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
                str += "internal ";
                break;
            case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
                str += "protected internal ";
                break;
            default:
                break;
        }
        if ((methodDef->flags & METHOD_ATTRIBUTE_STATIC) != 0)
            str += "static ";
        if ((methodDef->flags & METHOD_ATTRIBUTE_ABSTRACT) != 0) {
            str += "abstract ";
            if ((methodDef->flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) ==
                METHOD_ATTRIBUTE_REUSE_SLOT)
                str += "override ";
        } else if ((methodDef->flags & METHOD_ATTRIBUTE_FINAL) != 0) {
            if ((methodDef->flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) ==
                METHOD_ATTRIBUTE_REUSE_SLOT)
                str += "sealed override ";
        } else if ((methodDef->flags & METHOD_ATTRIBUTE_VIRTUAL) != 0) {
            if ((methodDef->flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) ==
                METHOD_ATTRIBUTE_NEW_SLOT)
                str += "virtual ";
            else
                str += "override ";
        }
        if ((methodDef->flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) != 0)
            str += "extern ";
        return str;
    }

    string GetIl2CppTypeName(const Il2CppType *type) {
        //printf("GetIl2CppTypeName\n");
        string ret;
        switch (type->type) {
            case IL2CPP_TYPE_CLASS:
            case IL2CPP_TYPE_VALUETYPE: {
                const Il2CppTypeDefinition *typeDef = MetaData::GetTypeDefinition(type->data.klassIndex);
                ret += GetIl2CppTypeDefName(typeDef);
                break;
            }
            case IL2CPP_TYPE_GENERICINST: {
                Il2CppGenericClass *generic_class = ReadPtr<Il2CppGenericClass>((kaddr)type->data.generic_class);
                const Il2CppTypeDefinition *typeDef = MetaData::GetTypeDefinition(generic_class->typeDefinitionIndex);
                ret = MetaData::GetString(typeDef->nameIndex);
                const Il2CppGenericInst *genericInst =  ReadPtr<Il2CppGenericInst>((kaddr)generic_class->context.class_inst);
                ret += GetIl2CppGenericTypeParams(genericInst);
                break;
            }
            case IL2CPP_TYPE_ARRAY: {
                Il2CppArrayType *arrayType = ReadPtr<Il2CppArrayType>((kaddr)type->data.array);
                const Il2CppType *oriType = ReadPtr<Il2CppType>((kaddr)arrayType->etype);
                ret = GetIl2CppTypeName(oriType) + "[" + string(",", (unsigned int) arrayType->rank - 1) + "]";
                break;
            }
            case IL2CPP_TYPE_SZARRAY: {
                const Il2CppType *oriType = ReadPtr<Il2CppType>((kaddr)type->data.type);
                ret = GetIl2CppTypeName(oriType) + "[]";
                break;
            }
            case IL2CPP_TYPE_PTR: {
                const Il2CppType *oriType = ReadPtr<Il2CppType>((kaddr)type->data.type);
                ret = GetIl2CppTypeName(oriType) + "*";
                break;
            }
            default:
                ret = IL2CppTypeEnumToString(type->type);
                break;
        }
        return ret;
    }

    void dump_class(const char* imgName, TypeDefinitionIndex index){
        const Il2CppTypeDefinition *pDef = MetaData::GetTypeDefinition(index);
        if(!pDef){ return; }

        bool isStruct = false;
        bool isEnum = false;
        vector<string> extends;

        if(pDef->parentIndex >= 0){
            const Il2CppType *parent = MetaData::GetIl2CppType(pDef->parentIndex);
            string parentName = GetIl2CppTypeName(parent);
            if (parentName == "ValueType")
                isStruct = true;
            else if (parentName == "Enum")
                isEnum = true;
            else if (parentName != "object")
                extends.push_back(parentName);
        }

        if(pDef->interfaces_count > 0){
            for (int i = 0; i < pDef->interfaces_count; i++) {
                const Il2CppType* interfaceType = MetaData::GetInterface(pDef->interfacesStart + i);
                extends.push_back(GetIl2CppTypeName(interfaceType));
            }
        }

        dumpcs << "// Namespace: " << imgName  << " -> " << MetaData::GetString(pDef->namespaceIndex) << endl;

        if (pDef->flags & TYPE_ATTRIBUTE_SERIALIZABLE)
            dumpcs << "[Serializable]" << endl;

        switch (pDef->flags & TYPE_ATTRIBUTE_VISIBILITY_MASK) {
            case TYPE_ATTRIBUTE_PUBLIC:
            case TYPE_ATTRIBUTE_NESTED_PUBLIC:
                dumpcs << "public " << flush;
                break;
            case TYPE_ATTRIBUTE_NOT_PUBLIC:
            case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
            case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
                dumpcs << "internal " << flush;
                break;
            case TYPE_ATTRIBUTE_NESTED_PRIVATE:
                dumpcs << "private " << flush;
                break;
            case TYPE_ATTRIBUTE_NESTED_FAMILY:
                dumpcs << "protected " << flush;
                break;
            case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
                dumpcs << "protected internal " << flush;
                break;
            default:
                break;
        }

        if ((pDef->flags & TYPE_ATTRIBUTE_ABSTRACT) != 0 && (pDef->flags & TYPE_ATTRIBUTE_SEALED) != 0)
            dumpcs << "static " << flush;
        else if ((pDef->flags & TYPE_ATTRIBUTE_INTERFACE) == 0 && (pDef->flags & TYPE_ATTRIBUTE_ABSTRACT) != 0)
            dumpcs << "abstract " << flush;
        else if (!isStruct && !isEnum && (pDef->flags & TYPE_ATTRIBUTE_SEALED) != 0)
            dumpcs << "sealed " << flush;
        if ((pDef->flags & TYPE_ATTRIBUTE_INTERFACE) != 0)
            dumpcs << "interface " << flush;
        else if (isStruct)
            dumpcs << "struct " << flush;
        else if (isEnum)
            dumpcs << "enum " << flush;
        else
            dumpcs << "class " << flush;

        dumpcs << GetIl2CppTypeDefName(pDef);
        if (!extends.empty()) {
            dumpcs << " : " << vStringJoin(extends, ", ");
        }
        //dumpcs << " // TypeDefIndex: " << to_string((int) index) << " {" << flush;
        dumpcs << " {" << flush;

        if(pDef->field_count > 0) {
            dumpcs << "\n\t// Fields\n" << flush;

            for (FieldIndex fieldIndex = pDef->fieldStart; fieldIndex < (pDef->fieldStart + pDef->field_count); fieldIndex++) {
                const Il2CppFieldDefinition* pFD = MetaData::GetFieldDefinition(fieldIndex);
                if(!pFD){ continue; }

                const Il2CppType* ftype = MetaData::GetIl2CppType(pFD->typeIndex);
				if (!ftype) { continue; }

                dumpcs << "\t";

                switch (ftype->attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK) {
                    case FIELD_ATTRIBUTE_PRIVATE:
                        dumpcs << "private " << flush;
                        break;
                    case FIELD_ATTRIBUTE_PUBLIC:
                        dumpcs << "public " << flush;
                        break;
                    case FIELD_ATTRIBUTE_FAMILY:
                        dumpcs << "protected " << flush;
                        break;
                    case FIELD_ATTRIBUTE_ASSEMBLY:
                    case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                        dumpcs << "internal " << flush;
                        break;
                    case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                        dumpcs << "protected internal " << flush;
                        break;
                    default:
                        break;
                }

                if ((ftype->attrs & FIELD_ATTRIBUTE_LITERAL) != 0) {
                    dumpcs << "const " << flush;
                }
                else {
                    if ((ftype->attrs & FIELD_ATTRIBUTE_STATIC) != 0)
                        dumpcs << "static " << flush;
                    if ((ftype->attrs & FIELD_ATTRIBUTE_INIT_ONLY) != 0)
                        dumpcs << "readonly " << flush;
                }

                dumpcs << GetIl2CppTypeName(ftype) << " " << MetaData::GetString(pFD->nameIndex) << flush;
				dumpcs << "; // 0x" << setbase(16) << (unsigned int)MetaData::GetFieldOffset(index, (fieldIndex - pDef->fieldStart)) << endl;
            }
        }

        if(pDef->property_count > 0) {
            dumpcs << "\n\t// Properties\n";

            for (PropertyIndex propertyIndex = pDef->propertyStart; propertyIndex < (pDef->propertyStart + pDef->property_count); propertyIndex++) {
                const Il2CppPropertyDefinition* propDef = MetaData::GetPropertyDefinition(propertyIndex);
                if(!propDef) { continue; }
                dumpcs << "\t";

                if(propDef->get >= 0){
                    const Il2CppMethodDefinition* methodDef = MetaData::GetMethodDefinition(pDef->methodStart + propDef->get);
                    dumpcs << GetModifiers(methodDef);
                    const Il2CppType* proptype = MetaData::GetIl2CppType(methodDef->returnType);
                    dumpcs << GetIl2CppTypeName(proptype) << " " << MetaData::GetString(propDef->nameIndex) << " { ";
                } else if (propDef->set > 0) {
                    const Il2CppMethodDefinition* methodDef = MetaData::GetMethodDefinition(pDef->methodStart + propDef->set);
                    dumpcs << GetModifiers(methodDef);
                    const Il2CppParameterDefinition* paramDef = MetaData::GetParameterDefinition(methodDef->parameterStart);
                    const Il2CppType* proptype = MetaData::GetIl2CppType(paramDef->typeIndex);
                    dumpcs << GetIl2CppTypeName(proptype) << " " << MetaData::GetString(propDef->nameIndex) << " { ";
                }

                if(propDef->get >= 0){
                    dumpcs << "get; ";
                }
                if (propDef->set >= 0) {
                    dumpcs << "set; ";
                }

                dumpcs << "}\n";
            }
        }

        if(pDef->method_count > 0){
            dumpcs << "\n\t// Methods\n";

            for (MethodIndex methodIndex = pDef->methodStart; methodIndex < (pDef->methodStart + pDef->method_count); methodIndex++) {
                const Il2CppMethodDefinition* methodDef = MetaData::GetMethodDefinition(methodIndex);
                if(!methodDef){ continue; }
                dumpcs << "\t";

                dumpcs << GetModifiers(methodDef);
                const Il2CppType* returntype = MetaData::GetIl2CppType(methodDef->returnType);
                dumpcs << GetIl2CppTypeName(returntype) << " " << MetaData::GetString(methodDef->nameIndex) << "(";

                if(methodDef->parameterCount > 0) {
                    string parameterStrs;
                    for (int param = 0; param < methodDef->parameterCount; param++) {
                        const Il2CppParameterDefinition *parameterDef = MetaData::GetParameterDefinition(methodDef->parameterStart + param);
                        if(!parameterDef){ continue; }
                        const Il2CppType *parameterType = MetaData::GetIl2CppType(parameterDef->typeIndex);

                        if ((parameterType->attrs & PARAM_ATTRIBUTE_OPTIONAL) != 0)
                            parameterStrs += "optional ";
                        if ((parameterType->attrs & PARAM_ATTRIBUTE_OUT) != 0)
                            parameterStrs += "out ";

                        parameterStrs += GetIl2CppTypeName(parameterType);
                        parameterStrs += " ";
                        parameterStrs += MetaData::GetString(parameterDef->nameIndex);

                        if (param < (methodDef->parameterCount - 1)) {
                            parameterStrs += ", ";
                        }
                    }

                    dumpcs << parameterStrs;
                }

                kaddr offset = MetaData::GetMethodPointer(methodDef->methodIndex);
                if(offset > 0){
                    dumpcs << "); // 0x" << setbase(16) << (unsigned int)((kaddr) offset - libbase) << "\n";
                } else {
                    dumpcs << "); // -1\n";
                }
            }
        }

        dumpcs << "}\n\n" << flush;
    }

    void DumpCS(){
        int classcount = 0;

        dumpcs.open("/sdcard/dump.cs", ofstream::out);
        //genscript.open("/sdcard/script.py", ofstream::out);

        if (dumpcs.is_open()) {
            printf("Dumping Classes\n");

            clock_t begin = clock();
            dumpcs << "// Images count: " << MetaData::getImageDefCount() << "\n\n" << flush;
            for(int image = 0; image < MetaData::getImageDefCount(); image++) {
                const Il2CppImageDefinition *pImg = MetaData::GetImageDefinition(image);
                if(!pImg) continue;

                dumpcs << "// Image[" << image << "]: " << MetaData::GetString(pImg->nameIndex) << " - " << pImg->typeCount << "\n" << flush;
            }
            dumpcs << endl;
            for(int image = 0; image < MetaData::getImageDefCount(); image++) {
                const Il2CppImageDefinition *pImg = MetaData::GetImageDefinition(image);
                if(!pImg) continue;

                for(TypeDefinitionIndex typeIndex = pImg->typeStart; typeIndex < (pImg->typeStart + pImg->typeCount); typeIndex++){
                    dump_class(MetaData::GetString(pImg->nameIndex), typeIndex);
                    classcount++;
                }
            }
            clock_t end = clock();

            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			printf("%d Classes Dumped in %fS\n", classcount, elapsed_secs);

            dumpcs.close();
        }
    }

    void DumpUsage(){
        for (uint32_t n = 0; n < MetaData::getMetadataUsageList(); n++) {
            const Il2CppMetadataUsageList* metadataUsageLists = MetaData::GetMetadataUsageList(n);

            uint32_t start = metadataUsageLists->start;
            uint32_t count = metadataUsageLists->count;

            for (uint32_t i = 0; i < count; i++){
                uint32_t offset = start + i;
                const Il2CppMetadataUsagePair* metadataUsagePairs = MetaData::GetMetadataUsagePairList(offset);

                uint32_t destinationIndex = metadataUsagePairs->destinationIndex;
                uint32_t encodedSourceIndex = metadataUsagePairs->encodedSourceIndex;

                Il2CppMetadataUsage usage = GetEncodedIndexType(encodedSourceIndex);
                uint32_t decodedIndex = GetDecodedMethodIndex(encodedSourceIndex);

                switch (usage) {
                    case kIl2CppMetadataUsageTypeInfo:
                        //*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = GetTypeInfoFromTypeIndex(decodedIndex);
                        break;
                    case kIl2CppMetadataUsageIl2CppType:
                        //*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = const_cast<Il2CppType*>(GetIl2CppTypeFromIndex(decodedIndex));
                        break;
                    case kIl2CppMetadataUsageMethodDef:
                    case kIl2CppMetadataUsageMethodRef:
                        //*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = const_cast<MethodInfo*>(GetMethodInfoFromIndex(encodedSourceIndex));
                        break;
                    case kIl2CppMetadataUsageFieldInfo:
                        //*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = GetFieldInfoFromIndex(decodedIndex);
                        break;
                    case kIl2CppMetadataUsageStringLiteral: {
                        //*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = GetStringLiteralFromIndex(decodedIndex);
                        const char *theStr = MetaData::GetStringLiteral(decodedIndex);
                        printf("[%s] Index:%d Addr:0x%x\n", theStr, destinationIndex,
                               MetaData::GetMetadataUsagePointer(destinationIndex));
                        break;
                    }
                    default:
                        //NOT_IMPLEMENTED(MetadataCache::InitializeMethodMetadata);
                        break;
                }
            }
        }
    }
}

#endif //DUMPER_H
