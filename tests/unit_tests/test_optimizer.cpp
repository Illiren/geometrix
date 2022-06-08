/*
 * Output CPU features and check for runtime errors
 */

#include "../../include/optimizer.hpp"
#include <iostream>
#include <cstring>

using namespace Geometrix;
using std::cout;
using std::endl;

std::string get_vendor_string()
{
    int32_t CPUInfo[4];
    char name[13];

    _OptimizerInternal::cpuid(CPUInfo, 0, 0);
    memcpy(name + 0, &CPUInfo[1], 4);
    memcpy(name + 4, &CPUInfo[3], 4);
    memcpy(name + 8, &CPUInfo[2], 4);
    name[12] = '\0';

    return name;
}

void print(const char* label, bool yes){
    cout << label;
    cout << (yes ? "Yes" : "No") << endl;
}

void printFeatures()
{
    cout << "Hardware Features:" << endl;
    print("    MMX         = ", Optimizer::hasFeature(Optimizer::CPU_X86_MMX));
    print("    x64         = ", Optimizer::hasFeature(Optimizer::CPU_X86_x64));
    print("    ABM         = ", Optimizer::hasFeature(Optimizer::CPU_X86_ABM));
    print("    RDRAND      = ", Optimizer::hasFeature(Optimizer::CPU_X86_RDRAND));
    print("    RDSEED      = ", Optimizer::hasFeature(Optimizer::CPU_X86_RDSEED));
    print("    BMI1        = ", Optimizer::hasFeature(Optimizer::CPU_X86_BMI1));
    print("    BMI2        = ", Optimizer::hasFeature(Optimizer::CPU_X86_BMI2));
    print("    ADX         = ", Optimizer::hasFeature(Optimizer::CPU_X86_ADX));
    print("    MPX         = ", Optimizer::hasFeature(Optimizer::CPU_X86_MPX));
    print("    PREFETCHW   = ", Optimizer::hasFeature(Optimizer::CPU_X86_PREFETCHW));
    print("    PREFETCHWT1 = ", Optimizer::hasFeature(Optimizer::CPU_X86_PREFETCHWT1));
    print("    RDPID       = ", Optimizer::hasFeature(Optimizer::CPU_X86_RDPID));
    print("    GFNI        = ", Optimizer::hasFeature(Optimizer::CPU_X86_GFNI));
    print("    VAES        = ", Optimizer::hasFeature(Optimizer::CPU_X86_VAES));
    cout << endl;

    cout << "SIMD: 128-bit" << endl;
    print("    SSE         = ", Optimizer::hasFeature(Optimizer::CPU_X86_SSE));
    print("    SSE2        = ", Optimizer::hasFeature(Optimizer::CPU_X86_SSE2));
    print("    SSE3        = ", Optimizer::hasFeature(Optimizer::CPU_X86_SSE3));
    print("    SSSE3       = ", Optimizer::hasFeature(Optimizer::CPU_X86_SSSE3));
    print("    SSE4a       = ", Optimizer::hasFeature(Optimizer::CPU_X86_SSE4a));
    print("    SSE4.1      = ", Optimizer::hasFeature(Optimizer::CPU_X86_SSE41));
    print("    SSE4.2      = ", Optimizer::hasFeature(Optimizer::CPU_X86_SSE42));
    print("    AES-NI      = ", Optimizer::hasFeature(Optimizer::CPU_X86_AES));
    print("    SHA         = ", Optimizer::hasFeature(Optimizer::CPU_X86_SHA));
    cout << endl;

    cout << "SIMD: 256-bit" << endl;
    print("    AVX         = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX));
    print("    XOP         = ", Optimizer::hasFeature(Optimizer::CPU_X86_XOP));
    print("    FMA3        = ", Optimizer::hasFeature(Optimizer::CPU_X86_FMA3));
    print("    FMA4        = ", Optimizer::hasFeature(Optimizer::CPU_X86_FMA4));
    print("    AVX2        = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX2));
    cout << endl;

    cout << "SIMD: 512-bit" << endl;
    print("    AVX512-F         = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_F));
    print("    AVX512-CD        = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_CD));
    print("    AVX512-PF        = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_PF));
    print("    AVX512-ER        = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_ER));
    print("    AVX512-VL        = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_VL));
    print("    AVX512-BW        = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_BW));
    print("    AVX512-DQ        = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_DQ));
    print("    AVX512-IFMA      = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_IFMA));
    print("    AVX512-VBMI      = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_VBMI));
    print("    AVX512-VPOPCNTDQ = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_VPOPCNTDQ));
    print("    AVX512-4FMAPS    = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_4FMAPS));
    print("    AVX512-4VNNIW    = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_4VNNIW));
    print("    AVX512-VBMI2     = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_VBMI2));
    print("    AVX512-VPCLMUL   = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_VPCLMUL));
    print("    AVX512-VNNI      = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_VNNI));
    print("    AVX512-BITALG    = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_BITALG));
    print("    AVX512-BF16      = ", Optimizer::hasFeature(Optimizer::CPU_X86_AVX512_BF16));
    cout << endl;
}

void printVendor()
{
    std::string vendor(get_vendor_string());
    if (vendor == "GenuineIntel" || vendor == "AuthenticAMD")
        cout << "CPU Vendor: " << vendor;
    else
        cout << "CPU Vendor: " << "Counterfeit CPU";
    cout << "\n\n";
}


int main()
{
    Optimizer::init();
    printVendor();
    printFeatures();

    return 0;
}
