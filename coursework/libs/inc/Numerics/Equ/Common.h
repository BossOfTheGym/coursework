#pragma once

#include <cmath>
#include <utility>

#include "../Common/Common.h"


namespace Num
{
    namespace Equ
    {
        template<class Argument>
        using Root = std::pair<Argument, int>;


        const int DEFAULT_ITERATIONS_LIMIT = 10;


        template<class Vector>
        using DefaultNorm = Utils::Norm<Vector, Utils::NormType::OCTO>;


        template<class ScalarType>
        class IterativeSolverBase
        {
        public:
            using Scalar = ScalarType;


        public:
            IterativeSolverBase() : IterativeSolverBase(DEFAULT_ITERATIONS_LIMIT, EPS<Scalar>)
            {}

            IterativeSolverBase(int iterationsLimit, Scalar eps = EPS<Scalar>)
                : m_iterationsLimit(iterationsLimit)
                , m_eps(eps)
            {}

            IterativeSolverBase(const IterativeSolverBase& isb) = default;

            IterativeSolverBase(IterativeSolverBase&& isb) = default;



            int& limit()
            {
                return m_iterationsLimit;
            }

            const int& limit() const
            {
                return m_iterationsLimit;
            }

            Scalar& eps()
            {
                return m_eps;
            }

            const Scalar& eps() const
            {
                return m_eps;
            }


        private:
            int m_iterationsLimit;
            Scalar m_eps;
        };
    }
}