#pragma once


#include <functional>

#include "../Common/Common.h"
#include "../Arg/ArgN.h"


namespace Num
{
    namespace Utils
    {
        template<class Argument, class Value = Argument>
        using OneArgument = std::function<Value(const Argument&)>;

        template<class Argument1, class Argument2, class Value = Argument2>
        using TwoArgument = std::function<Value(const Argument1&, const Argument2&)>;



        template<class Argument1, class Argument2, class Value = Argument2>
        class BindFirst
        {
        public:
            using FunctionType = TwoArgument<Argument1, Argument2, Value>;

        protected:
            FunctionType& m_func;
            Argument1 m_bound;

        public:
            BindFirst(FunctionType& func, Argument1 bound)
                : m_func(func)
                , m_bound(bound)
            {}

            BindFirst(const BindFirst& bind) : m_func(bind.m_func), m_bound(bind.m_bound)
            {}

            BindFirst(BindFirst&& bind) : m_func(bind.m_func), m_bound(bind.m_bound)
            {};


            BindFirst& operator = (const BindFirst& bind) = delete;
            BindFirst& operator = (BindFirst&& bind)      = delete;


            Value operator() (const Argument2& arg) 
            {
                return m_func(m_bound, arg);
            }
        };

        template<class Argument1, class Argument2, class Value = Argument2>
        class BindSecond
        {
        public:
            using FunctionType = TwoArgument<Argument1, Argument2, Value>;

        protected:
            FunctionType& m_func;
            Argument2 m_bound;

        public:
            BindSecond(FunctionType& func, Argument2 bound)
                : m_func(func)
                , m_bound(bound)
            {}

            BindSecond(const BindSecond& bind) : m_func(bind.m_func), m_bound(bind.m_bound)
            {}

            BindSecond(BindSecond&& bind) : m_func(bind.m_func), m_bound(bind.m_bound)
            {};


            BindSecond& operator = (const BindSecond& bind) = delete;
            BindSecond& operator = (BindSecond&& bind)      = delete;


            virtual Value operator() (const Argument1& arg) 
            {
                return m_func(arg, m_bound);
            }
        };



        enum NormType : int
        {
              CUBE
            , OCTO
            , SPHERE
        };

        template<class Vector, NormType type>
        class Norm;

        template<class Vector>
        class Norm<Vector, CUBE>
        {
        public:
            using Scalar = typename Vector::Scalar;


            Scalar operator ()(const Vector& vec)
            {
                Scalar value = abs(vec[0]);

                for (int i = 1; i < vec.size(); i++)
                {
                    Scalar next  = abs(vec[i]);
                    value = (value > next ? value : next);
                }

                return value;
            }
        };

        template<class Vector>
        class Norm<Vector, OCTO>
        {
        public:
            using Scalar = typename Vector::Scalar;


            Scalar operator()(const Vector& vec)
            {
                Scalar value = Scalar(0);

                for (int i = 0; i < vec.size(); i++)
                {
                    value += abs(vec[i]);
                }

                return value;
            }
        };

        template<class Vector>
        class Norm<Vector, SPHERE>
        {
        public:
            using Scalar = typename Vector::Scalar;


            Scalar operator()(const Vector& vec)
            {
                Scalar value = Scalar(0);

                for (int i = 0; i < vec.size(); i++)
                {
                    value += vec[i] * vec[i];
                }

                return sqrt(value);
            }
        };
    }
}