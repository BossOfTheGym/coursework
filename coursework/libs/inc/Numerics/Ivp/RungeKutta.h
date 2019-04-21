#pragma once


#include "Common.h"


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


		template<class Scalar>
        struct Methods
        {
            //+++++ explicit methods +++++

            //tableau 3x3
			//------------------
			//  0  |  0   0   0  
			// 1/3 | 1/3  0   0
			// 2/3 |  0  2/3  0
			//-----|------------
			//     | 1/4  0  3/4
			//s = 3
			static ButcherTableau<Scalar, 3> heun3();


            //tableau 4x4
			//----------------------
			//  0  |  0   0   0   0  
			// 1/2 | 1/2  0   0   0
			// 1/2 |  0  1/2  0   0
			//  1  |  0   0   1   0
			//-----|----------------
			//     | 1/6 1/3 1/3 1/6
			//s = 4
			static ButcherTableau<Scalar, 4> classic4();


            //+++++ implicit methods +++++

            //tableau 1x1
			//----------
			// 1/2 | 1/2
			//-----|----
			//     |  1
			//s = 2
			static ButcherTableau<Scalar, 1> gaussLegendre2();


            //tableau 2x2
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
			static ButcherTableau<Scalar, 2> gaussLegendre4();


            //tableau 3x3
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
			static ButcherTableau<Scalar, 3> gaussLegendre6();
        };

		template<class Scalar>
		ButcherTableau<Scalar, 3> Methods<Scalar>::heun3()
		{
			ButcherTableau<Scalar, 3> tableau;

			tableau.mat[1][0] = 1.0 / 3;
			tableau.mat[2][1] = 2.0 / 3;

			tableau.c[1] = 1.0 / 3;
			tableau.c[2] = 2.0 / 3;

			tableau.b[0] = 1.0 / 4;
			tableau.b[2] = 3.0 / 4;

			return tableau;
		}
		
		template<class Scalar>
		ButcherTableau<Scalar, 4> Methods<Scalar>::classic4()
		{
			ButcherTableau<Scalar, 4> tableau;

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

		template<class Scalar>
		ButcherTableau<Scalar, 1> Methods<Scalar>::gaussLegendre2()
		{
			ButcherTableau<Scalar, 1> tableau;

			tableau.mat[0][0] = 1.0 / 2;

			tableau.c[0] = 1.0 / 2;

			tableau.b[0] = 1.0;

			return tableau;
		}

		template<class Scalar>
		ButcherTableau<Scalar, 2> Methods<Scalar>::gaussLegendre4()
		{
			ButcherTableau<Scalar, 2> tableau;

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

		template<class Scalar>
		ButcherTableau<Scalar, 3> Methods<Scalar>::gaussLegendre6()
		{
			ButcherTableau<Scalar, 3> tableau;

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
            using NextNode = Node<Argument, Value>;
			using Vector = VectorType<Value, Tableau::ORDER>;


			RungeKuttaExplicit(Tableau&& tableau) : m_tableau(std::forward<Tableau>(tableau))
			{}

			template<class Function>
			NextNode solve(
				  Function&& func
				, const Argument& arg0
				, const Value& val0
				, const Argument& h
			)
			{
				const auto&[mat, cVec, bVec] = m_tableau;

				Vector kVec = Vector();
				for (int i = 0; i < Tableau::ORDER; i++)
				{
					for (int j = 0; j < i; j++)
					{
						kVec[i] += kVec[j] * mat[i][j];
					}

					kVec[i] = func(arg0 + h * cVec[i], val0 + h * kVec[i]);
				}

				Value sum = Value();
				for (int i = 0; i < Tableau::ORDER; i++)
				{
					sum += bVec[i] * kVec[i];
				}

				return {arg0 + h, val0 + h * sum};
			}
		

		private:
			Tableau m_tableau;
        };



        //unified for both Scalar & System
		template<
			  int SYSTEM_ORDER
			, class Argument
			, class Value = Argument
			, class Tableau = ButcherTableau<Argument, 2>
			, class Solver  = Equ::NeutonSystem<Argument, Tableau::ORDER * SYSTEM_ORDER>
			, template<class Scalar, int SIZE> class VectorType = Arg::VecN
			, template<class Scalar, int COLS, int ROWS> class MatrixType = Arg::MatNxM
		>
        class RungeKuttaImplicit
        {
        public:
			using NextNode = Node<Argument, Value>;

			RungeKuttaImplicit(Tableau&& tableau) 
				: m_tableau(std::forward<Tableau>(tableau))
				, m_solver()
			{}

			template<class Function, class Jacobian>
			NextNode solve(
				  Function&& func
				, Jacobian&& jacobian
				, const Argument& arg0
				, const Value& val0
				, const Argument& h
			)
			{
				union
				{
					MatrixType<Argument, Tableau::Order , SYSTEM_ORDER> kVecsMat;
					VectorType<Argument, Tableau::Order * SYSTEM_ORDER> kVecsVec;
				};



				return {};
			}


		public:
			Solver& solver()
			{
				return m_solver;
			}

			Tableau& tableau()
			{
				return m_tableau;
			}

		private:
			Solver   m_solver;
			Tableau m_tableau;
        };
    }
}