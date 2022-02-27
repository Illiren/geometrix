#pragma once

/* Mostly taken from https://github.com/Mysticial/FeatureDetector
 *
 * Original Author  : Alexander J. Yee
 * Date Created*     : 04/12/2014
 *
 * * - Modified to better fit this project
 *
 *
 * File contains static class Optimizer which controls usage of
 * advanced CPU instructions in functions implementations.
 * Calling Optimizer::init() at the program startup enables these implementations
 * by detecting CPU features on target machine.
 */
#include <stdint.h>
#include "matrix_implementation.hpp"
#include "trigonometry_implementation.hpp"

namespace _OptimizerInternal
{

    template<typename T>
    using SinCosFP = void (*)(T,T&,T&);
    template<typename T>
    using OneArgRetFP = T (*)(T);
    template<typename T>
    using TwoArgRetVecFP = void (*)(T (&)[4], T (&)[4], T(&)[4]);
    template<typename T>
    using TwoArgRetVecSingleFP = void (*)(T (&)[4], T, T(&)[4]);
    template<typename T>
    using TwoArgRetMatrixFP = void (*)(T (&)[4][4], T (&)[4][4], T(&)[4][4]);
    template<typename T>
    using TwoArgRetMatrixSingleFP = void (*)(T (&)[4][4], T, T(&)[4][4]);

    //using TwoArgRetFP = std::array<T,4> (*)(T (&)[4], T (&)[4]);

    SinCosFP<float> sinCosPtrf = &_Impl::sinCosFallbackImplementation<float>;
    SinCosFP<double> sinCosPtrd = &_Impl::sinCosFallbackImplementation<double>;
    OneArgRetFP<float> sinPtrf = &std::sin;
    OneArgRetFP<double> sinPtrd = &std::sin;
    OneArgRetFP<float> cosPtrf = &std::cos;
    OneArgRetFP<double> cosPtrd = &std::cos;

    TwoArgRetVecFP<float> subTwoVec4f = &_Impl::subVecFallbackImplementation;
    TwoArgRetVecFP<float> addTwoVec4f = &_Impl::addVecFallbackImplementation;
    TwoArgRetVecFP<float> mulTwoVec4f = &_Impl::mulVecFallbackImplementation;
    TwoArgRetVecFP<float> divTwoVec4f = &_Impl::divVecFallbackImplementation;
    TwoArgRetVecSingleFP<float> subVecSingle4f = &_Impl::subVecSingleFallbackImplementation;
    TwoArgRetVecSingleFP<float> addVecSingle4f = &_Impl::addVecSingleFallbackImplementation;
    TwoArgRetVecSingleFP<float> mulVecSingle4f = &_Impl::mulVecSingleFallbackImplementation;
    TwoArgRetVecSingleFP<float> divVecSingle4f = &_Impl::divVecSingleFallbackImplementation;
    TwoArgRetVecFP<double> subTwoVec4d = &_Impl::subVecFallbackImplementation;
    TwoArgRetVecFP<double> addTwoVec4d = &_Impl::addVecFallbackImplementation;
    TwoArgRetVecFP<double> mulTwoVec4d = &_Impl::mulVecFallbackImplementation;
    TwoArgRetVecFP<double> divTwoVec4d = &_Impl::divVecFallbackImplementation;
    TwoArgRetVecSingleFP<double> subVecSingle4d = &_Impl::subVecSingleFallbackImplementation;
    TwoArgRetVecSingleFP<double> addVecSingle4d = &_Impl::addVecSingleFallbackImplementation;
    TwoArgRetVecSingleFP<double> mulVecSingle4d = &_Impl::mulVecSingleFallbackImplementation;
    TwoArgRetVecSingleFP<double> divVecSingle4d = &_Impl::divVecSingleFallbackImplementation;

    TwoArgRetMatrixFP<float> subTwoMatrix4 = &_Impl::subMatrixFallbackImplementation;
    TwoArgRetMatrixFP<float> addTwoMatrix4 = &_Impl::addMatrixFallbackImplementation;
    TwoArgRetMatrixFP<float> mulTwoMatrix4 = &_Impl::mulMatrixFallbackImplementation;
    TwoArgRetMatrixFP<float> divTwoMatrix4 = &_Impl::divMatrixFallbackImplementation;
    TwoArgRetMatrixSingleFP<float> subMatrixSingle4 = &_Impl::subMatrixSingleFallbackImplementation;
    TwoArgRetMatrixSingleFP<float> addMatrixSingle4 = &_Impl::addMatrixSingleFallbackImplementation;
    TwoArgRetMatrixSingleFP<float> mulMatrixSingle4 = &_Impl::mulMatrixSingleFallbackImplementation;
    TwoArgRetMatrixSingleFP<float> divMatrixSingle4 = &_Impl::divMatrixSingleFallbackImplementation;
    //TwoArgRetFP<float> subTwoArr4x4 = &subFallbackImplementation;

    // current cpu features
    uint64_t features;


    #if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
    #if _WIN32
    #include <Windows.h>
    #include <intrin.h>
    //========================== Windows ========================//
    void cpuid(int32_t out[4], int32_t eax, int32_t ecx)
    {
        __cpuidex(out, eax, ecx);
    }
    __int64 xgetbv(unsigned int x)
    {
        return _xgetbv(x);
    }
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    //  Detect 64-bit - Note that this snippet of code for detecting 64-bit has been copied from MSDN.
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    BOOL IsWow64()
    {
        BOOL bIsWow64 = FALSE;

        LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
            GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

        if (NULL != fnIsWow64Process)
        {
            if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
            {
                printf("Error Detecting Operating System.\n");
                printf("Defaulting to 32-bit OS.\n\n");
                bIsWow64 = FALSE;
            }
        }
        return bIsWow64;
    }
    bool detect_OS_x64()
    {
    #ifdef _M_X64
        return true;
    #else
        return IsWow64() != 0;
    #endif
    #elif defined(__GNUC__) || defined(__clang__)
    //============================ Linux ========================//
    #include <cpuid.h>
    void cpuid(int32_t out[4], int32_t eax, int32_t ecx)
    {
        __cpuid_count(eax, ecx, out[0], out[1], out[2], out[3]);
    }
    uint64_t xgetbv(unsigned int index)
    {
        uint32_t eax, edx;
        __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
        return ((uint64_t)edx << 32) | eax;
    }
    #define _XCR_XFEATURE_ENABLED_MASK  0
    bool detect_OS_x64()
    {
        //  We support only x64 on Linux.
        return true;
    }
    #else
    #warning "No cpuid intrinsic defined for compiler."
        void cpuid(int32_t out[4], int32_t eax, int32_t ecx){}
    #endif
    #else
    #warning "No cpuid intrinsic defined for processor architecture."
        void cpuid(int32_t out[4], int32_t eax, int32_t ecx){}
    #endif

    bool detect_OS_AVX()
    {
        //  Copied from: http://stackoverflow.com/a/22521619/922184

        bool avxSupported = false;

        int cpuInfo[4];
        _OptimizerInternal::cpuid(cpuInfo, 1, 0);

        bool osUsesXSAVE_XRSTORE = (cpuInfo[2] & (1 << 27)) != 0;
        bool cpuAVXSuport = (cpuInfo[2] & (1 << 28)) != 0;

        if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
        {
            uint64_t xcrFeatureMask = _OptimizerInternal::xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            avxSupported = (xcrFeatureMask & 0x6) == 0x6;
        }

        return avxSupported;
    }
    bool detect_OS_AVX512()
    {
        if (!detect_OS_AVX())
            return false;

        uint64_t xcrFeatureMask = _OptimizerInternal::xgetbv(_XCR_XFEATURE_ENABLED_MASK);
        return (xcrFeatureMask & 0xe6) == 0xe6;
    }
}

namespace Geometrix
{
class Optimizer
{
public:

    // x86, x86_64
    enum
    {
        //  Misc.
        CPU_X86_MMX = 0,
        CPU_X86_x64,
        CPU_X86_ABM,
        CPU_X86_RDRAND,
        CPU_X86_RDSEED,
        CPU_X86_BMI1,
        CPU_X86_BMI2,
        CPU_X86_ADX,
        CPU_X86_MPX,
        CPU_X86_PREFETCHW,
        CPU_X86_PREFETCHWT1,
        CPU_X86_RDPID,

        //  SIMD: 128-bit
        CPU_X86_SSE,
        CPU_X86_SSE2,
        CPU_X86_SSE3,
        CPU_X86_SSSE3,
        CPU_X86_SSE41,
        CPU_X86_SSE42,
        CPU_X86_SSE4a,
        CPU_X86_AES,
        CPU_X86_SHA,

        //  SIMD: 256-bit
        CPU_X86_AVX,
        CPU_X86_XOP,
        CPU_X86_FMA3,
        CPU_X86_FMA4,
        CPU_X86_AVX2,

        //  SIMD: 512-bit
        CPU_X86_AVX512_F,
        CPU_X86_AVX512_CD,

        //  Knights Landing
        CPU_X86_AVX512_PF,
        CPU_X86_AVX512_ER,

        //  Skylake Purley
        CPU_X86_AVX512_VL,
        CPU_X86_AVX512_BW,
        CPU_X86_AVX512_DQ,

        //  Cannon Lake
        CPU_X86_AVX512_IFMA,
        CPU_X86_AVX512_VBMI,

        //  Knights Mill
        CPU_X86_AVX512_VPOPCNTDQ,
        CPU_X86_AVX512_4FMAPS,
        CPU_X86_AVX512_4VNNIW,

        //  Cascade Lake
        CPU_X86_AVX512_VNNI,

        //  Cooper Lake
        CPU_X86_AVX512_BF16,

        //  Ice Lake
        CPU_X86_AVX512_VBMI2,
        CPU_X86_GFNI,
        CPU_X86_VAES,
        CPU_X86_AVX512_VPCLMUL,
        CPU_X86_AVX512_BITALG,

        NUM_X86_FEATURES
    };

    // arm -> TODO add these later
    // https://developer.arm.com/documentation/dui0472/k/Using-NEON-Support/NEON-intrinsics-for-operations-with-a-scalar-value
    enum
    {
        CPU_ARM_UMAAL = NUM_X86_FEATURES,
        CPU_ARM_EDSP,
        CPU_ARM_VFP,
        CPU_ARM_VFP3,
        CPU_ARM_NEON
    };

    Optimizer() = delete;

    // Use this to enable CPU instructions enchanced implementation!
    static void init()
    {
        // Detect features
        using namespace _OptimizerInternal;
        features = 0u;
        //  OS Features
        bool OS_x64 = _OptimizerInternal::detect_OS_x64();
        bool OS_AVX = _OptimizerInternal::detect_OS_AVX();
        bool OS_AVX512 = _OptimizerInternal::detect_OS_AVX512();

        int info[4];
        _OptimizerInternal::cpuid(info, 0, 0);
        int nIds = info[0];

        _OptimizerInternal::cpuid(info, 0x80000000, 0);
        uint32_t nExIds = info[0];

        //  Detect Features
        if (nIds >= 0x00000001){
            cpuid(info, 0x00000001, 0);
            if ((info[3] & ((int)1 << 23)) != 0) features |= 1ull << CPU_X86_MMX;
            if ((info[3] & ((int)1 << 25)) != 0) features |= 1ull << CPU_X86_SSE;
            if ((info[3] & ((int)1 << 26)) != 0) features |= 1ull << CPU_X86_SSE2;
            if ((info[2] & ((int)1 <<  0)) != 0) features |= 1ull << CPU_X86_SSE3;

            if ((info[2] & ((int)1 <<  9)) != 0) features |= 1ull << CPU_X86_SSSE3;
            if ((info[2] & ((int)1 << 19)) != 0) features |= 1ull << CPU_X86_SSE41;
            if ((info[2] & ((int)1 << 20)) != 0) features |= 1ull << CPU_X86_SSE42;
            if ((info[2] & ((int)1 << 25)) != 0) features |= 1ull << CPU_X86_AES;

            if (((info[2] & ((int)1 << 28)) != 0) && OS_AVX) features |= 1ull << CPU_X86_AVX;
            if ((info[2] & ((int)1 << 12)) != 0) features |= 1ull << CPU_X86_FMA3;

            if ((info[2] & ((int)1 << 30)) != 0) features |= 1ull << CPU_X86_RDRAND;
        }
        if (nIds >= 0x00000007){
            cpuid(info, 0x00000007, 0);
            if (((info[1] & ((int)1 <<  5)) != 0) && OS_AVX) features |= 1ull << CPU_X86_AVX2;

            if ((info[1] & ((int)1 <<  3)) != 0) features |= 1ull << CPU_X86_BMI1;
            if ((info[1] & ((int)1 <<  8)) != 0) features |= 1ull << CPU_X86_BMI2;
            if ((info[1] & ((int)1 << 19)) != 0) features |= 1ull << CPU_X86_ADX;
            if ((info[1] & ((int)1 << 14)) != 0) features |= 1ull << CPU_X86_MPX;
            if ((info[1] & ((int)1 << 29)) != 0) features |= 1ull << CPU_X86_SHA;
            if ((info[1] & ((int)1 << 18)) != 0) features |= 1ull << CPU_X86_RDSEED;
            if ((info[2] & ((int)1 <<  0)) != 0) features |= 1ull << CPU_X86_PREFETCHWT1;
            if ((info[2] & ((int)1 << 22)) != 0) features |= 1ull << CPU_X86_RDPID;

            if (((info[1] & ((int)1 << 16)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_F;
            if (((info[1] & ((int)1 << 28)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_CD;
            if (((info[1] & ((int)1 << 26)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_PF;
            if (((info[1] & ((int)1 << 27)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_ER;

            if (((info[1] & ((int)1 << 31)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_VL;
            if (((info[1] & ((int)1 << 30)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_BW;
            if (((info[1] & ((int)1 << 17)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_DQ;

            if (((info[1] & ((int)1 << 21)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_IFMA;
            if (((info[2] & ((int)1 <<  1)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_VBMI;

            if (((info[2] & ((int)1 << 14)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_VPOPCNTDQ;
            if (((info[3] & ((int)1 <<  2)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_4FMAPS;
            if (((info[3] & ((int)1 <<  3)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_4VNNIW;

            if (((info[2] & ((int)1 << 11)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_VNNI;

            if (((info[2] & ((int)1 <<  6)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_VBMI2;
            if ((info[2] & ((int)1 <<  8)) != 0) features |= 1ull << CPU_X86_GFNI;
            if ((info[2] & ((int)1 <<  9)) != 0) features |= 1ull << CPU_X86_VAES;
            if (((info[2] & ((int)1 << 10)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_VPCLMUL;
            if (((info[2] & ((int)1 << 12)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_BITALG;


            cpuid(info, 0x00000007, 1);
            if (((info[0] & ((int)1 <<  5)) != 0) && OS_AVX512) features |= 1ull << CPU_X86_AVX512_BF16;
        }
        if (nExIds >= 0x80000001){
            cpuid(info, 0x80000001, 0);
            if (((info[3] & ((int)1 << 29)) != 0) && OS_x64) features |= 1ull << CPU_X86_x64;
            if ((info[2] & ((int)1 <<  5)) != 0) features |= 1ull << CPU_X86_ABM;
            if ((info[2] & ((int)1 <<  6)) != 0) features |= 1ull << CPU_X86_SSE4a;
            if ((info[2] & ((int)1 << 16)) != 0) features |= 1ull << CPU_X86_FMA4;
            if ((info[2] & ((int)1 << 11)) != 0) features |= 1ull << CPU_X86_XOP;
            if ((info[2] & ((int)1 << 8)) != 0) features |= 1ull << CPU_X86_PREFETCHW;
        }
        // assign function implementation
#ifdef __SSE2__
        if (hasFeature(CPU_X86_SSE))
        {
#endif
#ifdef __FMA__
            if (hasFeature(CPU_X86_FMA3))
            {
                _OptimizerInternal::sinPtrf = &_Impl::sinFastSSEImplementation;
                _OptimizerInternal::sinPtrd = &_Impl::sinFastSSEImplementation;
                _OptimizerInternal::cosPtrf = &_Impl::cosFastSSEImplementation;
                _OptimizerInternal::cosPtrd = &_Impl::cosFastSSEImplementation;
                _OptimizerInternal::sinCosPtrf = &_Impl::sinCosSSEImplementation;
                _OptimizerInternal::sinCosPtrd = &_Impl::sinCosSSEImplementation;
            }
#endif
#ifdef __SSE2__
            _OptimizerInternal::addTwoVec4f = &_Impl::addVecIntrinImplementation;
            _OptimizerInternal::addVecSingle4f = &_Impl::addVecSingleIntrinImplementation;
            _OptimizerInternal::subTwoVec4f = &_Impl::subVecIntrinImplementation;
            _OptimizerInternal::subVecSingle4f = &_Impl::subVecSingleIntrinImplementation;
            _OptimizerInternal::mulTwoVec4f = &_Impl::mulVecIntrinImplementation;
            _OptimizerInternal::mulVecSingle4f = &_Impl::mulVecSingleIntrinImplementation;
            _OptimizerInternal::divTwoVec4f = &_Impl::divVecIntrinImplementation;
            _OptimizerInternal::divVecSingle4f = &_Impl::divVecSingleIntrinImplementation;
        }
#endif
#if defined(__AVX__)
        if (hasFeature(CPU_X86_AVX))
        {
            _OptimizerInternal::addTwoVec4d = &_Impl::addVecIntrinImplementation;
            _OptimizerInternal::addVecSingle4d = &_Impl::addVecSingleIntrinImplementation;
            _OptimizerInternal::subTwoVec4d = &_Impl::subVecIntrinImplementation;
            _OptimizerInternal::subVecSingle4d = &_Impl::subVecSingleIntrinImplementation;
            _OptimizerInternal::mulTwoVec4d = &_Impl::mulVecIntrinImplementation;
            _OptimizerInternal::mulVecSingle4d = &_Impl::mulVecSingleIntrinImplementation;
            _OptimizerInternal::divTwoVec4d = &_Impl::divVecIntrinImplementation;
            _OptimizerInternal::divVecSingle4d = &_Impl::divVecSingleIntrinImplementation;
        }
#endif
#if defined(__AVX512F__)
        if (hasFeature(CPU_X86_AVX512_F))
        {
            _OptimizerInternal::subTwoMatrix4 = &_Impl::subMatrixIntrinImplementation;
            _OptimizerInternal::subMatrixSingle4 = &_Impl::subMatrixSingleIntrinImplementation;
            _OptimizerInternal::addTwoMatrix4 = &_Impl::addMatrixIntrinImplementation;
            _OptimizerInternal::addMatrixSingle4 = &_Impl::addMatrixSingleIntrinImplementation;
            _OptimizerInternal::mulTwoMatrix4 = &_Impl::mulMatrixIntrinImplementation;
            _OptimizerInternal::mulMatrixSingle4 = &_Impl::mulMatrixSingleIntrinImplementation;
            _OptimizerInternal::divTwoMatrix4 = &_Impl::divMatrixIntrinImplementation;
            _OptimizerInternal::divMatrixSingle4 = &_Impl::divMatrixSingleIntrinImplementation;
        }
#endif

    }

    static bool hasFeature(int mask)
    {
        return _OptimizerInternal::features & (1 << mask);
    }
};
}
