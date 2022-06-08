#pragma once
/*
 * This file stores utilities for lookup table (LUT) generation
 * (constexpr LUT and source code generating LUT),
 * depending on the desired accuracy of approximation
*/
#include <iostream>
#include <ios>
#include <fstream>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <vector>



//================== constexpr array initialization ==========================//

// fold to 1/8 of the period range(pi/4)
constexpr int SIN_COS_FOLDING_RATIO = 8;

/*
 * Monotonic lookup table generator.
 * size value is limited by compiler constexpr operations limit and can be
 * modified by setting appropriate compile flags
 */
template <typename T, typename Info, typename Generator>
constexpr auto generateLUT(Generator&& f) noexcept
{
    using ValueType = decltype (f(T{0}, T{0}));
    std::array<ValueType,Info::size> data;
    T currentArg = Info::startValue;
    // set first value w/o gradient
    data[0] = f(Info::startValue, 0);
    currentArg += Info::step;

    for (std::size_t i = 1; i < Info::size; ++i)
    {
        data[i] = f(currentArg, std::get<0>(data[i-1]));
        currentArg += Info::step;
    }

    return data;
}

// array initializer
template <typename T, T (*Func)(T), typename Info>
inline constexpr auto getLUT = generateLUT<T,Info>([](T arg, T prevValue)
{
    // calculate value for arg, and gradient from previous value
    T value = Func(arg);

    return std::make_pair(value, value - prevValue);
});



//========================= header file generation ===========================//
// just an example look-up table header generator for sin/cos period type of function
inline constexpr auto FLOAT_FNAME = "float_table.hpp";
inline constexpr auto DOUBLE_FNAME = "double_table.hpp";
inline constexpr auto FT_SIN_NAME = "SIN_TABLE_F";
inline constexpr auto DT_SIN_NAME = "SIN_TABLE_D";
inline constexpr auto FT_SIN_GRAD_NAME = "SIN_GRAD_F";
inline constexpr auto DT_SIN_GRAD_NAME = "SIN_GRAD_D";
inline constexpr auto FT_COS_NAME = "COS_TABLE_F";
inline constexpr auto DT_COS_NAME = "COS_TABLE_D";
inline constexpr auto FT_COS_GRAD_NAME = "COS_GRAD_F";
inline constexpr auto DT_COS_GRAD_NAME = "COS_GRAD_D";
// numbers of significant digits for stdout, which would be enough,
// accuracy-wise
inline constexpr int FLOAT_PREC_DIGITS = 12;
inline constexpr int DOUBLE_PREC_DIGITS = 19;

int tableSizeFromAcc(double relError, int ratio)
{
    return int(M_PI / std::acos(1 - relError) / ratio) + 1;
}

using namespace std;

// class for redirecting std output to file
class FileRedirectStream
{
public:
    FileRedirectStream(const char* filename, int precision) :
      fileStream(filename),
      cout_buff(cout.rdbuf())
    {
        assert(fileStream && "can't open file");
        cout.rdbuf(fileStream.rdbuf());
        flags = cout.flags();
        cout << hexfloat << setprecision(precision);
        fileStream << "#pragma once\n\n";
    };
    ~FileRedirectStream()
    {
        if (fileStream)
           fileStream.close();
        cout.rdbuf(cout_buff);
        cout.flags(flags);
    }

    // for regular output of variables and so on
    template<typename T> FileRedirectStream& operator<<(const T& something)
    {
        cout << something;
        return *this;
    }
    // for manipulators like std::endl
    typedef std::ostream& (*stream_function)(std::ostream&);
    FileRedirectStream& operator<<(stream_function func)
    {
        func(cout);
        return *this;
    }

private:
    ofstream fileStream;
    basic_streambuf<char>* cout_buff;
    ios_base::fmtflags flags;
};


template<typename T, bool isSin>
static void writeTable(FileRedirectStream& file, int size, const char* countStr)
    requires(std::is_floating_point_v<T>)
{
    const char* decl = "inline constexpr ";
    file << decl;
    if constexpr (is_same<T, float>())
    {
        file << "float ";
        if constexpr (isSin)
            file << FT_SIN_NAME;
        else
            file << FT_COS_NAME;
    }
    else
    {
        file << "double ";
        if constexpr (isSin)
            file << DT_SIN_NAME;
        else
            file << DT_COS_NAME;
    }
    file << "[" << countStr << ']' << " = {\n";
    T startArg = 0.;
    T currentArg = startArg;
    T step = M_PI_4 / size;
    vector<T> table;
    table.resize(size);
    for(T i = 0; i < size; ++i)
    {
        T val;
        if constexpr (isSin)
            val = sin(currentArg);
        else
            val = cos(currentArg);
        table[i] = val;
        currentArg += step;
        file << val << ",\n";
    }
    file << "};\n\n";

    // gradient values
    file << decl;
    if constexpr (is_same<T, float>())
    {
        file << "float ";
        if constexpr (isSin)
            file << FT_SIN_GRAD_NAME;
        else
            file << FT_COS_GRAD_NAME;
    }
    else
    {
        file << "double ";
        if constexpr (isSin)
            file << DT_SIN_GRAD_NAME;
        else
            file << DT_COS_GRAD_NAME;
    }
    file << '[' << countStr << ']' << " = {\n";
    T prevVal = table[0];
    for(int i = 1; i < size-1; ++i)
    {
        file << (table[i] - prevVal) << ",\n";
        prevVal = table[i];
    }
    file << "};\n";
}

template<typename T>
static void generateSinCosTable()
{
    static_assert(is_floating_point<T>() && (is_same<T, float>() || is_same<T, double>()));
    int size;
    if constexpr (is_same<T, float>())
        size = tableSizeFromAcc(1E-9, SIN_COS_FOLDING_RATIO);
    else
        size = tableSizeFromAcc(1E-11, SIN_COS_FOLDING_RATIO);
    //actually, the size should be 1E-17, but the table would be too enormous
    assert(size > 0 && "invalid table size");
    const char* fileName;
    int precision;
    if constexpr (is_same<T, float>())
    {
        fileName = FLOAT_FNAME;
        precision = FLOAT_PREC_DIGITS;
    }
    else
    {
        fileName = DOUBLE_FNAME;
        precision = DOUBLE_PREC_DIGITS;
    }
    FileRedirectStream file(fileName, precision);
    const int strBufSize = 8;
    char sizeStr[strBufSize];
    sprintf(sizeStr, "%d", size);
    const char* countStr;
    if constexpr (is_same<T, float>())
        countStr = "TABLE_COUNT_F";
    else
        countStr = "TABLE_COUNT_D";
    const char* decl = "inline constexpr ";
    file << decl << "int " << countStr << " = " << sizeStr << ";\n\n";
    writeTable<T, true>(file, size, countStr);
    file << '\n';
    writeTable<T, false>(file, size, countStr);
}
