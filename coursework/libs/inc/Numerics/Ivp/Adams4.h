//#pragma once
//
//#include "Utils.h"
//
//
//
//namespace Adams4
//{
//    const int ADAMS_4 = 4;
//    const int ADAMS_4_PREV = 3;
//    const int N = 2;
//
//
//    template<class Argument, class Value>
//    using Iterations = std::array<Utils::Node<Argument, Value>, ADAMS_4_PREV>;
//
//
//    int next(
//          Utils::TwoArgument<double, double, double>& func
//        , Utils::TwoArgument<double, double, double>& deriv
//        , Iterations<double, double>& prev
//        , double h
//    );
//
//
//    Utils::Solution<double, double> solve(
//          Utils::TwoArgument<double, double, double>& func
//        , Utils::TwoArgument<double, double, double>& deriv
//        , double a
//        , double b
//        , double x0
//        , double u0
//        , int n
//    );
//}