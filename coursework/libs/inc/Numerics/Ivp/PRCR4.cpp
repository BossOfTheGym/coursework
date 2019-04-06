//#include "PRCR4.h"
//
//#include "Neuton.h"
//
//
//namespace PRCR4
//{
//
//    using FWrapper = Utils::FuncWrapper <double, double, double>;
//    using DWrapper = Utils::DerivWrapper<double, double, double>;
//
//
//    std::pair<double, double> nextConst(
//          Utils::TwoArgument<double, double, double>& f
//        , Utils::TwoArgument<double, double, double>& d
//        , double arg
//        , double val
//        , double h
//    )
//    {
//        //f5: Fn[5]
//
//        //y0:  Yn+1/m[2] = Yn[5] + h / m * Fn[5]
//        //
//        //y1:  Yn+1/3[4] = Yn[5] + h / 18 * ((6-m) * Fn[5] + m * Fn+1/m[2])
//        //
//        //y2:  Yn+1/2[4] = Yn[5] + h / 8 * (Fn[5] + 3 * Fn+1/3[3])
//        //
//        //y3:  Yn+1[4]   = Yn[5] + h / 2 * (Fn[5] - 3 * Fn+1/3[3] + 4 * Fn+1[4]) - pred
//        //
//        //y4:  Yn+1[5]   = Yn[5] + h / 6 * (Fn[5] + 4 * Fn+1/2[4] + Fn+1[4]) - corr
//
//        double f5 = f(arg, val);
//
//
//        double y0 = val + h / M * f5;
//
//        double y1 = val + h / 18 * ((6 - M) * f5 + M * f(arg + h / M, y0));
//
//
//        double f3 = f(arg + h / 3, y1);
//
//
//        double y2 = val + h / 8 * (f5 + 3 * f3);
//
//        FWrapper fWrap(f, arg + h, 2 * h, val + h / 2 * (f5 - 3 * f3));
//        DWrapper dWrap(d, arg + h, 2 * h);
//
//        double y3 = Neuton::solveScalar(fWrap, dWrap, y2).first;
//
//        double y4 = val + h / 6 * (f5 + 4 * f(arg + h / 2, y2) + f(arg + h, y3));
//
//        return {y3, y4};
//    }
//
//
//    Utils::Solution<double, double> solveConst(
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
//        std::vector<Utils::Node<double, double>> solution;
//
//        solution.reserve(n + 1);
//        solution.push_back({arg0, val0});
//
//        double arg = arg0;
//        double val = val0;
//        for (int j = 0; j < n; j++)
//        {
//            auto next = nextConst(f, d, arg, val, h);
//
//            arg = arg0 + h * j;
//            val = next.second;
//
//            solution.push_back({arg, val});
//        }
//
//        return solution;
//    }
//
//
//
//    std::pair<double, double> nextAuto(
//          Utils::TwoArgument<double, double, double>& f
//        , Utils::TwoArgument<double, double, double>& d
//        , double arg
//        , double val
//        , double h
//        , double eps
//    )
//    {
//        std::pair<double, double> node;
//
//        while(true)
//        {
//            node = nextConst(f, d, arg, val, h);
//
//            if (abs(node.first - node.second) > eps)
//            {
//                h *= DEC;
//
//                if (h < H_MIN)
//                {
//                    h = H_MIN;
//                    break;
//                }
//            }
//            else
//            {
//                break;
//            }
//        }
//
//        return {node.second, h};
//    }
//
//
//    Utils::Solution<double, double> solveAuto(
//          Utils::TwoArgument<double, double, double>& f
//        , Utils::TwoArgument<double, double, double>& d
//        , double a
//        , double b
//        , double arg0
//        , double val0
//        , double hInit
//        , double eps
//    )
//    {
//        Utils::Solution<double, double> solution;
//
//        double arg = arg0;
//        double val = val0;
//
//        double h = hInit;
//
//
//        while (arg <= b)
//        {
//            solution.push_back({arg, val});
//
//            auto next = nextAuto(f, d, arg, val, h, eps);
//
//            val = next.first;
//            h   = next.second;
//
//            arg += h;
//
//            h *= INC;
//        }
//
//        auto last = solution.back();
//        h = b - last.arg;
//
//        auto next = nextConst(f, d, last.arg, last.val, h);
//        solution.push_back({b, next.second});
//
//        return solution;
//    }
//}