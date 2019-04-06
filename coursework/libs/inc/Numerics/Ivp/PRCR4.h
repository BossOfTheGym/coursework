//#pragma once
//
//
//#include "Utils.h"
//
//
//namespace PRCR4
//{
//    const int M = 3;
//
//    const double INC = 2.0;
//    const double DEC = 1.0 / 2;
//
//    const double H_MIN = 1e-6;
//    const double H_MAX = 1e-4;
//
//
//    std::pair<double, double> nextConst(
//          Utils::TwoArgument<double, double, double>& func
//        , Utils::TwoArgument<double, double, double>& deriv
//        , double arg
//        , double val
//        , double h
//    );
//
//
//    Utils::Solution<double, double> solveConst(
//          Utils::TwoArgument<double, double, double>& func
//        , Utils::TwoArgument<double, double, double>& deriv
//        , double a
//        , double b
//        , double arg0
//        , double val0
//        , int n
//    );
//
//
//
//    std::pair<double, double> nextAuto(
//          Utils::TwoArgument<double, double, double>& func
//        , Utils::TwoArgument<double, double, double>& deriv
//        , double arg
//        , double val
//        , double h
//        , double eps
//    );
//
//
//    Utils::Solution<double, double> solveAuto(
//          Utils::TwoArgument<double, double, double>& func
//        , Utils::TwoArgument<double, double, double>& deriv
//        , double a
//        , double b
//        , double arg0
//        , double val0
//        , double hInit
//        , double eps
//    );
//}
