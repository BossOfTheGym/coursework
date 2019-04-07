#pragma once


#include "Common.h"

namespace Num
{
    namespace Ivp
    {
        template<
            class ScalarType
            , int ORDER
            , template<class Scalar, int ROWS, int COLS> class MatrixType
            , template<class Scalar, int SIZE> class VectorType
        >
        struct ButcherTableau;
    }
}


namespace
{
    using Num::Ivp::ButcherTableau;


	//+++++ exploicit methods +++++

    //------------------
            //  0  |  0   0   0  
            // 1/3 | 1/3  0   0
            // 2/3 |  0  2/3  0
            //-----|------------
            //     | 1/4  0  3/4
            //s = 3
    template<class ScalarType>
    const auto initHeun3()
    {
        ButcherTableau<ScalarType, 3> tableau;

        tableau.mat[1][0] = 1.0 / 3;
        tableau.mat[2][1] = 2.0 / 3;

        tableau.c[1] = 1.0 / 3;
        tableau.c[2] = 2.0 / 3;

        tableau.b[0] = 1.0 / 4;
        tableau.b[2] = 3.0 / 4;

        return tableau;
    }


    //----------------------
    //  0  |  0   0   0   0  
    // 1/2 | 1/2  0   0   0
    // 1/2 |  0  1/2  0   0
    //  1  |  0   0   1   0
    //-----|----------------
    //     | 1/6 1/3 1/3 1/6
    //s = 4
    template<class ScalarType>
    const auto initClassic4()
    {
        ButcherTableau<ScalarType, 4> tableau;

        tableau.mat[1][0] = 1.0 / 2;
        tableau.mat[2][1] = 1.0 / 2;
        tableau.mat[3][2] = 1.0;

        tableau.c[1] = 1.0 / 2;
        tableau.c[2] = 1.0 / 2;
        tableau.c[3] = 1.0;

        tableau.b[0] = 1.0 / 6;
        tableau.b[1] = 1.0 / 3;
        tableau.b[2] = 1.0 / 3;
        tableau.b[3] = 1.0 / 6;

        return tableau;
    }


    //+++++ implicit methods +++++

    //----------
    // 1/2 | 1/2
    //-----|----
    //     |  1
    //s = 2
    template<class ScalarType>
    const auto initGaussLegendre2()
    {
        ButcherTableau<ScalarType, 1> tableau;

        tableau.mat[0][0] = 1.0 / 2;

        tableau.c[0] = 1.0 / 2;

        tableau.b[0] = 1.0;

        return tableau;
    }


    // c1 = 1/2 - 1/6 * sqrt(3)
    // c2 = 1/2 + 1/6 * sqrt(3)
    //
    // a12 = 1/4 - 1/6 * sqrt(3)
    // a21 = 1/4 + 1/6 * sqrt(3)
    //
    //--------------
    // c1 | 1/4 a12
    // c2 | a21 1/4
    //----|---------
    //    | 1/2 1/2
    //s = 4
    template<class ScalarType>
    const auto initGaussLegendre4()
    {
        ButcherTableau<ScalarType, 2> tableau;

        tableau.mat[0][0] = 1.0 / 4;
        tableau.mat[0][1] = 1.0 / 4 - 1.0 / 6 * sqrt(3);
        tableau.mat[1][0] = 1.0 / 4 + 1.0 / 6 * sqrt(3);
        tableau.mat[1][1] = 1.0 / 4;

        tableau.c[0] = 1.0 / 2 - 1.0 / 6 * sqrt(3);
        tableau.c[1] = 1.0 / 2 + 1.0 / 6 * sqrt(3);

        tableau.b[0] = 1.0 / 2;
        tableau.b[1] = 1.0 / 2;

        return tableau;
    }


    // c1 = 1/2 - 1/10 * sqrt(15)
    // c3 = 1/2 + 1/10 * sqrt(15)
    //
    // a12 = 2/9 - 1/15 * sqrt(15)
    // a21 = 2/9 + 1/15 * sqrt(15)
    //
    // a13 = 5/36 - 1/30 * sqrt(15)
    // a31 = 5/36 + 1/30 * sqrt(15)
    //
    // a23 = 5/36 - 1/24 * sqrt(15)
    // a32 = 5/36 + 1/24 * sqrt(15)
    //
    //--------------------
    // c1  | 5/36 a12 a13  
    // 1/2 | a21  2/9 a23
    // c3  | a31  a32 5/36
    //-----|---------------
    //     | 5/18 4/9 5/18
    template<class ScalarType>
    const auto initGaussLegendre6()
    {
        ButcherTableau<ScalarType, 3> tableau;

        tableau.mat[0][0] = 5.0 / 36;
        tableau.mat[0][1] = 2.0 / 9 - 1.0 / 15 * sqrt(15);
        tableau.mat[0][2] = 5.0 / 36 - 1.0 / 30 * sqrt(15);

        tableau.mat[1][0] = 2.0 / 9 + 1.0 / 15 * sqrt(15);
        tableau.mat[1][1] = 2.0 / 9;
        tableau.mat[1][2] = 5.0 / 36 - 1.0 / 24 * sqrt(15);

        tableau.mat[2][0] = 5.0 / 36 + 1.0 / 30 * sqrt(15);
        tableau.mat[2][1] = 5.0 / 36 + 1.0 / 24 * sqrt(15);
        tableau.mat[2][2] = 5.0 / 18;

        tableau.c[0] = 1.0 / 2 - 1.0 / 10 * sqrt(15);
        tableau.c[1] = 1.0 / 2;
        tableau.c[2] = 1.0 / 2 + 1.0 / 10 * sqrt(15);

        tableau.b[0] = 5.0 / 18;
        tableau.b[1] = 4.0 / 9;
        tableau.b[2] = 5.0 / 18;

        return tableau;
    }
}


namespace Num
{
    namespace Ivp
    {
        template<
            class ScalarType
            , int N
            , template<class Scalar, int ROWS, int COLS> class MatrixType = Arg::MatNxM
            , template<class Scalar, int SIZE> class VectorType = Arg::VecN
        >
        struct ButcherTableau
        {
			template<class scalar, int rows, int cols>
			using MatTemplate = MatrixType<scalar, rows, cols>;

			template<class scalar, int size>
			using VecTemplate = VectorType<scalar, size>;


            using Scalar = ScalarType;
            using Vector = VectorType<Scalar, N>;
            using Matrix = MatrixType<Scalar, N, N>;

            static const int ORDER = N;


            Matrix mat; // coef matrix
            Vector c;   // time coefs
            Vector b;   // final coefs
        };


        struct Methods
        {
            //+++++ explicit methods +++++

            template<class Scalar>//tableau 3x3
            static const auto heun3 = initHeun3<Scalar>();


            template<class Scalar>//tableau 4x4
            static const auto classic4 = initClassic4<Scalar>();


            //+++++ implicit methods +++++

            template<class Scalar>//tableau 1x1
            static const auto gaussLegendre2 = initGaussLegendre2<Scalar>();


            template<class Scalar>//tableau 2x2
            static const auto gaussLegendre4 = initGaussLegendre4<Scalar>();


            template<class Scalar>//tableau 3x3
            static const auto gaussLegendre6 = initGaussLegendre6<Scalar>();
        };



        //unified for both Scalar & System
        template<
			  int SYSTEM_ORDER
            , class Argument
            , class Value = Argument
			, class Tableau = ButcherTableau<Argument, 4>
			, template<class Scalar, int SIZE> class VectorType = Arg::VecN
        >
        class RungeKuttaExplicit
        {
        public:
            using FunctionType = Utils::TwoArgument<Argument, Value>;
            using NextNode = Node<Argument, Value>;


            NextNode solve(
                  const FunctionType& func
                , const Tableau& tableau
                , const Argument& arg0
                , const Value& val0
                , const Argument& h
            )
            {
                const int ORDER = Tableau::ORDER;

                using Vector = VectorType<Value, ORDER>;


                const auto&[mat, cVec, bVec] = tableau;

                Vector kVec = Vector();
                for (int i = 0; i < ORDER; i++)
                {
                    for (int j = 0; j < i; j++)
                    {
                        kVec[i] += kVec[j] * mat[i][j];
                    }

                    kVec[i] = func(arg0 + h * cVec[i], val0 + h * kVec[i]);
                }

                Value sum = Value();
                for (int i = 0; i < ORDER; i++)
                {
                    sum += bVec[i] * kVec[i];
                }

                return {arg0 + h, val0 + h * sum};
            }
        };
    }
}