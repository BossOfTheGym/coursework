#pragma once

#include "../LinAlg/Gauss.h"
#include "../Arg/ArgN.h"
#include "../Common/Common.h"
#include "../Utils/Utils.h"

#include "Common.h"


//TODO : add different jacobians as functors
namespace Num
{
    namespace Equ
    {
        template<class Argument, class Value = Argument>
        class NeutonScalar : public IterativeSolverBase<Value>
        {
        public:
            using Base = IterativeSolverBase<Value>;
            using Base::Base;
            using Base::limit;
            using Base::eps;

            //using FunctionType = Utils::OneArgument<Argument, Value>;

			template<class Function, class Derivative>
            Root<Argument> solve(
                  Function&& func
                , Derivative&& deriv
                , const Argument& start
            )
            {
                Argument xPrev;
                Argument xNext;

                int iterations = 0;

                xNext = start;
                do
                {
                    iterations++;

                    xPrev = xNext;

                    xNext = xPrev - func(xPrev) / deriv(xPrev);
                } while (abs(xNext - xPrev) > eps() && iterations < limit());

                return {xNext, iterations};
            }
        };




        template<
              class ScalarType
            , int N
            , template<class Scalar = ScalarType, int ROWS = N, int COLS = N> class MatrixType = Arg::MatNxM
            , template<class Scalar = ScalarType, int SIZE = N> class VectorType = Arg::VecN
            , class Solver = LinAlg::GaussEliminationSingle<ScalarType, N>
            , class Norm   = DefaultNorm<VectorType<ScalarType, N>>
        >
        class NeutonSystem : public IterativeSolverBase<ScalarType>
        {
        public:
            using Base = IterativeSolverBase<ScalarType>;
            using Base::limit;
            using Base::eps;

            using Scalar = ScalarType;
            using Matrix = MatrixType<Scalar, N, N>;
            using Vector = VectorType<Scalar, N>;


            NeutonSystem() : Base(), m_solver(), m_norm()
            {}

            NeutonSystem(int iterationsLimit, Scalar eps = EPS<Scalar>) 
                : Base(iterationsLimit, eps)
                , m_norm()
            {}

            NeutonSystem(const NeutonSystem& ns) = default;

            NeutonSystem(NeutonSystem&& ns) = default;

			template<class Function, class Jacobian>
            Root<Vector> solve(
                  Function&& func
                , Jacobian&& jacobian
                , Vector&& start
            )
            {
                Vector xPrev;
                Vector xNext;
                Vector delta;

                Matrix mat;

                int iterations = 0;


                xNext = start;
                do
                {
                    iterations++;

                    xPrev = xNext;

                    mat = jacobian(xPrev);
                    delta = -func(xPrev);

                    m_solver.solve(mat, delta);

                    xNext = xPrev + delta;
                } while (m_norm(delta) > eps() && iterations < limit());

                return {xNext, iterations};
            }


			Norm& getNorm()
			{
				return m_norm;
			}


        private:
            Solver m_solver;
            Norm   m_norm;
        };


		//Jacobians
		template< 
			  class ScalarType
			, int N
			, template<class Scalar = ScalarType, int ROWS = N, int COLS = N> class MatrixType = Arg::MatNxM
			, template<class Scalar = ScalarType, int SIZE = N> class VectorType = Arg::VecN
		>
		class DiffJacobian
		{
		public:
			using Scalar = ScalarType;
			using Vector = VectorType<Scalar, N>;
			using Matrix = MatrixType<Scalar, N, N>;

			using FunctionType = Utils::OneArgument<Vector, Matrix>;


		public:
			DiffJacobian(const FunctionType& function, Scalar eps) 
				: m_func(function)
				, m_eps(eps)
			{}

			DiffJacobian(const DiffJacobian& dj) = default;

			DiffJacobian(DiffJacobian&& dj) = default;

			DiffJacobian& operator = (const DiffJacobian& dj) = default;

			DiffJacobian& operator = (DiffJacobian&& dj) = default;



			Matrix operator() (const Vector& arg)
			{
				Matrix mat;

				for(int i = 0; i < N; i++)
				{
					Vector rightArg = arg;
					Vector  leftArg = arg;
					Vector right;
					Vector left;

					rightArg[i] += m_eps;
					right = m_func(rightArg);

					leftArg[i] -= m_eps;
					left = m_func(leftArg);

					for (int j = 0; j < N; j++)
					{
						mat[i][j] = (right[j] - left[j]) / (2 * m_eps);
					}
				}

				return mat;
			}


		private:
			FunctionType m_func;
			Scalar m_eps;
		};
    }
}