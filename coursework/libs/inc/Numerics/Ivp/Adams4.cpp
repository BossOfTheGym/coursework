//#include "Adams4.h"
//
//#include "PRCR4.h"
//#include "Neuton.h"
//
//
//namespace Adams4
//{
//    using FWrapper = Utils::FuncWrapper <double, double, double>;
//    using DWrapper = Utils::DerivWrapper<double, double, double>;
//
//
//    int next(
//          Utils::TwoArgument<double, double, double>& f
//        , Utils::TwoArgument<double, double, double>& d
//        , Iterations<double, double>& prev
//        , double arg
//        , double h
//    )
//    {
//        //Yn+1 = Yn + h/24(9Fn+1 + 19Fn - 5Fn-1 + Fn-2)
//
//        double term = prev[N].val + h / 24 * (
//            +19 * f(prev[N].arg, prev[N].val)
//            - 5 * f(prev[N - 1].arg, prev[N - 1].val)
//            + 1 * f(prev[N - 2].arg, prev[N - 2].val)
//            );
//
//        FWrapper fWrap(f, arg + h, 3 * h / 8, term);
//        DWrapper dWrap(d, arg + h, 3 * h / 8);
//        auto next = Neuton::solveScalar(fWrap, dWrap, prev[N].val);
//
//        std::swap(prev[N - 2], prev[N - 1]);
//        std::swap(prev[N - 1], prev[N]);
//
//        prev[N].arg = arg + h;
//        prev[N].val = next.first;
//
//        return next.second;
//    }
//
//
//    Utils::Solution<double, double> solve(
//          Utils::TwoArgument<double, double, double>& f
//        , Utils::TwoArgument<double, double, double>& d
//        , double a
//        , double b
//        , double arg0
//        , double val0
//        , int n
//    )
//    {
//        const double h = (b - a) / n;
//
//        Utils::Solution<double, double>  solution;
//        Iterations<double, double> prev;
//
//        solution.reserve(n + 1);
//
//        double arg;
//        double val;
//
//        arg = arg0 + 0 * h;
//        val = val0;
//        prev[N - 2].arg = arg;
//        prev[N - 2].val = val;
//        solution.push_back(prev[N - 2]);
//
//
//        val = PRCR4::nextConst(f, d, arg, val, h).second;
//        arg = arg0 + 1 * h;
//        prev[N - 1].arg = arg;
//        prev[N - 1].val = val;
//        solution.push_back(prev[N - 1]);
//
//
//        val = PRCR4::nextConst(f, d, arg, val, h).second;
//        arg = arg0 + 2 * h;
//        prev[N].arg = arg;
//        prev[N].val = val;
//        solution.push_back(prev[N]);
//
//
//        for (int j = N; j < n; j++)
//        {
//            arg = arg0 + j * h;
//
//            next(f, d, prev, arg, h);
//
//            solution.push_back(prev[N]);
//        }
//
//        return solution;
//    }
//}