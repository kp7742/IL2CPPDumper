#include "kmods.h"

using namespace std;

const char* short_options = "hlrfsp:g:m:c:o:";
const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"lib", no_argument, NULL, 'l'},
        {"package", required_argument, NULL, 'p'},
        {"raw", no_argument, NULL, 'r'},
        {"fast", no_argument, NULL, 'f'},
        {"script", no_argument, NULL, 's'},
        {"global", required_argument, NULL, 'g'},
        {"meta", required_argument, NULL, 'm'},
        {"code", required_argument, NULL, 'c'},
        {"output", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}
};

void Usage() {
    printf("IL2CPPDumper v0.4 <==> Made By KMODs(kp7742)\n");
    printf("Usage: il2cppdumper <option(s)>\n");
    printf("Dump Lib libil2cpp.so from Memory of Game Process and Generate structure dump.cs\n");
    printf("@@Unity il2cpp v24 Supported Only@@\n");
    printf(" Options:\n");
    printf("--Lib Args--------------------------------------------------------------\n");
    printf("  -l --lib                              Dump libil2cpp.so from Memory\n");
    printf("  -p --package <packageName>            Package Name of Game\n");
    printf("  -r --raw(Optional)                    Output Raw Lib and Not Rebuild It\n");
    printf("  -f --fast(Optional)                   Enable Fast Dumping\n");
    printf("                                        (May Miss Some Bytes in Dump)\n");
    printf("--Script Args-----------------------------------------------------------\n");
    printf("  -s --script                           Generate structure dump.cs\n");
    printf("  -p --package <packageName>            Package Name of Game\n");
    printf("  -g --global  <address>                GlobalMetaData Address\n");
    printf("  -m --meta    <address>                MetadataRegistration Address\n");
    printf("  -c --code    <address>                CodeRegistration Address\n");
    printf("--Other Args------------------------------------------------------------\n");
    printf("  -o --output <outputPath>              File Output path(Default: /sdcard)\n");
    printf("  -h --help                             Display this information\n");
}

int main(int argc, char *argv[]) {
    int c, mode=-1;
    string pkg, output("/sdcard");
    bool isValidArg = true, isFastDump = false, isRawDump = false;
    kaddr globalMeta = 0, MetaReg = 0, CodeReg = 0;

    while((c = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (c) {
            case 'l':
                mode = 1;
                break;
            case 'p':
                pkg = optarg;
                break;
            case 'r':
                isRawDump = true;
                break;
            case 'f':
                isFastDump = true;
                break;
            case 's':
                mode = 2;
                break;
            case 'g':
                globalMeta = strtoul(optarg, 0, 16);
                break;
            case 'm':
                MetaReg = strtoul(optarg, 0, 16);
                break;
            case 'c':
                CodeReg = strtoul(optarg, 0, 16);
                break;
            case 'o':
                output = optarg;
                break;
            default:
                isValidArg = false;
                break;
        }
    }

    if(!isValidArg || mode < 0 || output.empty()) {
        printf("Wrong Arguments, Please Check!!\n");
        Usage();
        return -1;
    }

    if(mode == 1){
        //Find PID
        target_pid = find_pid(pkg.data());
        if (target_pid == -1) {
            cout << "Can't find the process" << endl;
            return -1;
        }
        cout << "Process name: " << pkg.data() << ", Pid: " << target_pid << endl;

        //Lib Base Address
        libbase = get_module_base(lib_name);
        if (libbase == 0) {
            cout << "Can't find module" << endl;
            return -1;
        }
        cout << "Base Address of " << lib_name << " Found At " << setbase(16) << libbase << setbase(10) << endl;
        cout << "End Address of " << lib_name << " Found At " << setbase(16) << get_module_end(lib_name) << setbase(10) << endl;

        //Lib Dump
        kaddr start_addr = libbase;
        kaddr end_addr = get_module_end(lib_name);
        size_t libsize = (end_addr - start_addr);

        cout << "Lib Size: " << libsize << endl;

        clock_t begin = clock();
        if(isRawDump){
            ofstream ldump(output + "/libil2cpp.so", ofstream::out | ofstream::binary);
            if (ldump.is_open()) {
                if (isFastDump) {
                    uint8 *buffer = new uint8[libsize];
                    memset(buffer, '\0', libsize);
                    vm_readv((void *) start_addr, buffer, libsize);
                    ldump.write((char *) buffer, libsize);
                } else {
                    char *buffer = new char[1];
                    while (libsize != 0) {
                        vm_readv((void *) (start_addr++), buffer, 1);
                        ldump.write(buffer, 1);
                        --libsize;
                    }
                }
            } else {
                cout << "Can't Output File" << endl;
                return -1;
            }
            ldump.close();
        } else {
            ElfReader elf_reader;
            string tempPath = output + "/KTemp.dat";

            ofstream ldump(tempPath, ofstream::out | ofstream::binary);
            if (ldump.is_open()) {
                if (isFastDump) {
                    uint8 *buffer = new uint8[libsize];
                    memset(buffer, '\0', libsize);
                    vm_readv((void *) start_addr, buffer, libsize);
                    ldump.write((char *) buffer, libsize);
                } else {
                    char *buffer = new char[1];
                    while (libsize != 0) {
                        vm_readv((void *) (start_addr++), buffer, 1);
                        ldump.write(buffer, 1);
                        --libsize;
                    }
                }
            } else {
                cout << "Can't Output File" << endl;
                return -1;
            }
            ldump.close();

            //SoFixer Code//
			cout << "Rebuilding Elf(So)" << endl;
			
            elf_reader.setDumpSoFile(true);
            elf_reader.setDumpSoBaseAddr(start_addr);

            auto file = fopen(tempPath.c_str(), "rb");
            if(nullptr == file) {
                printf("source so file cannot found!!!\n");
                return -1;
            }
            auto fd = fileno(file);

            elf_reader.setSource(tempPath.c_str(), fd);

            if(!elf_reader.Load()) {
                printf("source so file is invalid\n");
                return -1;
            }

            ElfRebuilder elf_rebuilder(&elf_reader);
            if(!elf_rebuilder.Rebuild()) {
                printf("error occured in rebuilding elf file\n");
                return -1;
            }
            fclose(file);
            //SoFixer Code//

            ofstream rdump(output + "/libil2cpp.so", ofstream::out | ofstream::binary);
            if (rdump.is_open()) {
                rdump.write((char*) elf_rebuilder.getRebuildData(), elf_rebuilder.getRebuildSize());
            } else {
                cout << "Can't Output File" << endl;
                return -1;
            }
            remove(tempPath.c_str());
            rdump.close();
        }
        clock_t end = clock();

        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        printf("Lib Dumped in %fS\n", elapsed_secs);
    } else if(mode == 2){
        //Find PID
        target_pid = find_pid(pkg.data());
        if (target_pid == -1) {
            cout << "Can't find the process" << endl;
            return -1;
        }
        cout << "Process name: " << pkg.data() << ", Pid: " << target_pid << endl;

        //Lib Base Address
        libbase = get_module_base(lib_name);
        if (libbase == 0) {
            cout << "Can't find module" << endl;
            return -1;
        }
        cout << "Base Address of " << lib_name << " Found At " << setbase(16) << libbase << setbase(10) << endl;
        cout << endl;

        if(globalMeta < 1 || MetaReg < 1 || CodeReg < 1){
            cout << "Can't find the process" << endl;
            return -1;
        }

        //Il2cpp Dumping
        MetaData::init(globalMeta, MetaReg, CodeReg);
        Dumper::DumpCS(output);
    } else {
        Usage();
        return -1;
    }

	return 0;
}