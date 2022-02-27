/*
 * This file stores sets of coeffients for minimax polynomial approximation
 * with different number of terms for variable accuracy. Values were calculated
 * to accurately approximate the funtions over the range 0..Pi/2, and optimized
 * further for relative error reduction. Degrees of polinomials were chosen
 * accuracy wise, in range from first meaningfull to the number that would cover
 * floating point accuracy.
 * Coeffients values for sine and cosine are taken from here:
 * https://gist.github.com/publik-void/067f7f2fef32dbe5c27d6e215f824c91
 * // TODO use Sollya to generate my own
 *
 * Arc tangent approximation polynomials are generated via Sollya library
 * fpminimax function which outputs minimax polynomial approx. for a given function
*/
#pragma once

#include <array>
#include <tuple>
#include <limits>
#include <stdint.h>
#include "constants.hpp"

namespace _ConstInternal
{

struct alignas (alignof (int)) ReductionRes
{
    int quad;
    bool noReduciton;
};

// simple fast additive reduction with possible loss of accuracy for large arguments
template<typename T> constexpr ReductionRes addRangeReduce(T& arg, double maxRange, double invMaxRange) noexcept
{
    if ((arg >= 0 ? arg : -arg) <= maxRange)
        return {0, true};

    double temp = arg;// use double for less loss of significance
    const int quad = (int)(temp * invMaxRange);
    temp = temp - (double)(quad * maxRange);
    arg = temp;
    return {quad, false};
}

template<typename T> constexpr auto getNearestInt(T x) noexcept
{
    static_assert(std::is_floating_point<T>(), "Invalid arg type");
    constexpr auto h = T(0.5) - std::numeric_limits<T>::epsilon();
    const T sign = x > 0 ? 1 : -1;
    return (int64_t)(x + sign*h);
}


using PolyData = const double*;
using PolyIndex = std::size_t;

//====================== Sine/Cosine polinomials =============================//
inline constexpr int SIN_POLIES_COUNT = 8;
// indexes for approximation, that should suffice floating point accuracy
inline constexpr int SP_ERROR_DEGREE_INDEX = 5;
inline constexpr int DP_ERROR_DEGREE_INDEX = 7;


// odd coeffs for sine
inline constexpr std::array<double,2> SIN_DEGREE_3 =
{0.992787728983164233059810507773856991,// 1-degree
 -0.146210290215383029232877806264248677};// 3-degree...
inline constexpr std::array<double,3> SIN_DEGREE_5 =
{0.999891821255810892885564707156941565,
 -0.165960116540878989063185380996540407,
 0.00760290334336935120704015646842617915};
inline constexpr std::array<double,4> SIN_DEGREE_7 =
{0.999999060898976336474926982596043563,
 -0.166655540927576933646197607200949732,
 0.00831189980138987918776159520367912155,
 -0.000184881402886071911033139680005197992};
inline constexpr std::array<double,5> SIN_DEGREE_9 =
{0.999999994686007336752316120259640318,
 -0.166666566840071513590695269999128453,
 0.00833302513896936729848481553136180314,
 -0.000198074187274269708745741141088641071,
 2.60190306765146018582500885337773154E-6};
inline constexpr std::array<double,6> SIN_DEGREE_11 =
{0.99999999997884898600402426033768998,
 -0.166666666088260696413164261885310067,
 0.00833333072055773645376566203656709979,
 -0.000198408328232619552901560108010257242,
 2.75239710746326498401791551303359689E-6,
 -2.3868346521031027639830001794722295E-8};
inline constexpr std::array<double,7> SIN_DEGREE_13 =
{0.999999999999937559931818667019042,
 -0.166666666664323314581815742382489749,
 0.00833333331876551401513171161164153048,
 -0.00019841266411622150098365220068523596,
 2.75569319265949080406017672747752072E-6,
 -2.50295188656032073490380438676828409E-8,
 1.54011703714146442508663314577356389E-10};
inline constexpr std::array<double,8> SIN_DEGREE_15 =
{0.99999999999999985783856947280351013,
 -0.166666666666659653164780128859839316,
 0.00833333333327592139676057538106745659,
 -0.000198412698232225093689107237380802637,
 2.75573164212929639596445204076151988E-6,
 -2.5051870883490902518590017165648945E-8,
 1.6047844633018114428263064708844387E-10,
 -7.37066278281678179293546235379257048E-13};
inline constexpr std::array<double,9> SIN_DEGREE_17 =
{0.99999999999999999974277490079943975,
 -0.166666666666666650522767323353840604,
 0.00833333333333316503140948668861163462,
 -0.00019841269841201840459252750531485886,
 2.75573192101527564362114785169078252E-6,
 -2.50521067982746148969440582709985054E-8,
 1.60589364903732230834314189302038183E-10,
 -7.64291780693694318128770390349958602E-13,
 2.72047909631134875287705126898888084E-15};

inline constexpr std::array<std::tuple<PolyIndex,PolyData>,SIN_POLIES_COUNT> SIN_POLIES =
{
    std::make_tuple(SIN_DEGREE_3.size(), SIN_DEGREE_3.data()),
    std::make_tuple(SIN_DEGREE_5.size(), SIN_DEGREE_5.data()),
    std::make_tuple(SIN_DEGREE_7.size(), SIN_DEGREE_7.data()),
    std::make_tuple(SIN_DEGREE_9.size(), SIN_DEGREE_9.data()),
    std::make_tuple(SIN_DEGREE_11.size(), SIN_DEGREE_11.data()),
    std::make_tuple(SIN_DEGREE_13.size(), SIN_DEGREE_13.data()),
    std::make_tuple(SIN_DEGREE_15.size(), SIN_DEGREE_15.data()),
    std::make_tuple(SIN_DEGREE_17.size(), SIN_DEGREE_17.data())
};

// even coeffs for cosine
inline constexpr std::array<double,3> COS_DEGREE_4 =
{0.997372645040477990699027658698347186,
 -0.490966242354240750313919970830772248,
 0.0351569652103601536791893003031729288};
inline constexpr std::array<double,4> COS_DEGREE_6 =
{0.999970210689953068626323587055728078,
 -0.499782706704688809140466617726333455,
 0.0413661149638482252569383872576459943,
 -0.0012412397582398600702129604944720102};
inline constexpr std::array<double,5> COS_DEGREE_8 =
{0.999999792271214939267553805222053911,
 -0.499997347307564798261689263275170926,
 0.0416605522442190334337486569138386962,
 -0.0013835791854080904305903409668113965,
 0.0000228155780226852578391801303428006217};
inline constexpr std::array<double,6> COS_DEGREE_10 =
{0.99999999901810067632218592152414676,
 -0.49999998049253581064488831264724178,
 0.0416665985274352494970529831079268818,
 -0.00138879697151174993540500936074733546,
 0.00002474324689798977846771995314323317,
 -2.57924183182520559803981154578763508E-7};
inline constexpr std::array<double,7> COS_DEGREE_12 =
{0.99999999999664497762294088303450344,
 -0.499999999904093446864749737540127153,
 0.0416666661919898461055893453767336909,
 -0.00138888797032770920681384355560203468,
 0.0000248007136556145113256051130495176344,
 -2.75135611164571371141959208910569516E-7,
 1.97644182995841772799444848310451781E-9};
inline constexpr std::array<double,8> COS_DEGREE_14 =
{0.9999999999999913246735627197920925,
 -0.499999999999662298517775822702998135,
 0.041666666664412501559930358186670219,
 -0.00138888888296507330038783473947564241,
 0.0000248015794582964580010321673714486073,
 -2.75567434405946778860326965959718338E-7,
 2.08529198807394186029297197627654707E-9,
 -1.094796343456433735930433763570635E-11};
inline constexpr std::array<double,9> COS_DEGREE_16 =
{0.99999999999999998243004448007448662,
 -0.499999999999999105881272803474436268,
 0.041666666666658914344068844317924505,
 -0.00138888888886231429175747130897185107,
 0.0000248015872549765577961155967511699095,
 -2.75573145508960795189972565635584642e-7,
 2.08764776731016710219609723288490596e-9,
 -1.14608862231521440480830153964369191e-11,
 4.58927688754481747776178904291483144E-14};
inline constexpr std::array<double,10> COS_DEGREE_18 =
{0.99999999999999999997137279452082542,
 -0.499999999999999998154315099318475409,
 0.0416666666666666464830761430785494906,
 -0.00138888888888880138354292714761533424,
 0.000024801587301391185639801128147609099,
 -2.75573191983631520203018748182144674E-7,
 2.08767549239714497727103009851893486E-9,
 -1.1470641559691907631205172698119569E-11,
 4.77627556974286641810975958891813886E-14,
 -1.50677871898184264862447669562686491E-16};

inline constexpr std::array<std::tuple<PolyIndex,PolyData>,SIN_POLIES_COUNT> COS_POLIES =
{
    std::make_tuple(COS_DEGREE_4.size(), COS_DEGREE_4.data()),
    std::make_tuple(COS_DEGREE_6.size(), COS_DEGREE_6.data()),
    std::make_tuple(COS_DEGREE_8.size(), COS_DEGREE_8.data()),
    std::make_tuple(COS_DEGREE_10.size(), COS_DEGREE_10.data()),
    std::make_tuple(COS_DEGREE_12.size(), COS_DEGREE_12.data()),
    std::make_tuple(COS_DEGREE_14.size(), COS_DEGREE_14.data()),
    std::make_tuple(COS_DEGREE_16.size(), COS_DEGREE_16.data()),
    std::make_tuple(COS_DEGREE_18.size(), COS_DEGREE_18.data())
};
inline constexpr std::size_t SIN_COS_ACC_MAP_COUNT = 11;
// map of number of required accurate fraction digits to fitting polynom index for sine and cosine approx.
inline constexpr std::array<std::size_t,SIN_COS_ACC_MAP_COUNT> SIN_COS_ACC_MAP =
{1,2,3,3,4,4,5,6,6,7,7};

// accuracy template parameter default values
template <typename T> constexpr std::size_t sinCosAcc = DP_ERROR_DEGREE_INDEX; // by default the most accurate
template <> inline constexpr std::size_t sinCosAcc<float> = SP_ERROR_DEGREE_INDEX;
template <> inline constexpr std::size_t sinCosAcc<double> = DP_ERROR_DEGREE_INDEX;


//============================= Tan/ATan polinomials =========================//
inline constexpr int TAN_POLIES_COUNT = 10;
inline constexpr int HP_ERROR_TAN = 1;
inline constexpr int SP_ERROR_TAN = 3;
inline constexpr int DP_ERROR_TAN = 5;

inline constexpr std::array<double,2> TAN_DEGREE_2 =
{
 -3.6112171,
 -4.6133253
};
inline constexpr std::array<double,3> TAN_DEGREE_3 =
{
 -3.61678027,
 0.134516124,
 -4.033321984
};
inline constexpr std::array<double,4> TAN_DEGREE_4 =
{
 211.849369664121,
 -12.5288887278440,
 269.7350131214121,
 -71.4145309347748
};
inline constexpr std::array<double,8> TAN_DEGREE_8 =
{
 10881241.46289544215469695742,
 -895306.0870564145957447087575,
 14181.99563014366386894487566,
 -45.63638305432707847378129653,
 13854426.92637036839270054048,
 -3988641.468163077300701338784,
 135299.4744550023680867559195,
 -1014.19757617656429288596025
};

template <typename T> constexpr std::size_t tanAcc = DP_ERROR_TAN; // by default the most accurate
template <> constexpr std::size_t tanAcc<float> = SP_ERROR_TAN;
template <> constexpr std::size_t tanAcc<double> = DP_ERROR_TAN;

inline constexpr std::array<double,4> ATAN_DEGREE_3 =
{
    2.455098112113773822784423828125e-2,
    0.97382764215581119060516357421875,
    -0.258666165173053741455078125,
    2.37673334777355194091796875e-2
};
inline constexpr double ATAN_LINEAR_DEGREE_3_A = 1.9725882448256015777587890625e-3;
inline constexpr double ATAN_LINEAR_DEGREE_3_B = 1.422767855226993560791015625;

inline constexpr std::array<double,9> ATAN_DEGREE_8 =
{
    -7.31612509080248329951246e-3,
    1.143813058225298884273968,
    -0.44571262228579688891017,
    9.96300443419767534081188e-2,
    -1.327626395338410425495113e-2,
    1.067967536814928841693018e-3,
    -5.070853471655922817262763e-5,
    1.305791235250985948823654e-6,
    -1.404127114579267754379543e-8
};

inline constexpr double ATAN_LINEAR_DEGREE_8_A = 4.99484112940984381623366e-4;
inline constexpr double ATAN_LINEAR_DEGREE_8_B = 1.518475007238157314937155;
// argument value for atan approximation tha correspondes to the change of polinomial used
// for approx. ex: if arg < switch value -> higher degree polynom is used, else linear approx.
constexpr std::size_t ATAN_APPROX_SWITCH_DEGREE_3 = 5;
constexpr std::size_t ATAN_APPROX_SWITCH_DEGREE_8 = 20;

}
