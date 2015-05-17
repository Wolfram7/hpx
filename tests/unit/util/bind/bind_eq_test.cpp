#include <hpx/hpx_init.hpp>

#if defined(BOOST_MSVC)
#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#endif

//  Taken from the Boost.Bind library
//
//  bind_eq_test.cpp - hpx::util::bind equality operator
//
//  Copyright (c) 2004, 2005 Peter Dimov
//  Copyright (c) 2013 Agustin Berge
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <hpx/util/bind.hpp>
#include <boost/ref.hpp>

namespace placeholders = hpx::util::placeholders;

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
# include <boost/function_equal.hpp>
#endif

#include <iostream>

#include <hpx/util/lightweight_test.hpp>

struct X
{
    int i_;

    explicit X(int i): i_(i)
    {
    }

    bool operator==(X const & rhs) const
    {
        return i_ == rhs.i_;
    }
};

// f_*

int f_0()
{
    return 0;
}

int f_1(X)
{
    return 0;
}

int f_2(X, X)
{
    return 0;
}

int f_3(X, X, X)
{
    return 0;
}

int f_4(X, X, X, X)
{
    return 0;
}

int f_5(X, X, X, X, X)
{
    return 0;
}

int f_6(X, X, X, X, X, X)
{
    return 0;
}

int f_7(X, X, X, X, X, X, X)
{
    return 0;
}

int f_8(X, X, X, X, X, X, X, X)
{
    return 0;
}

int f_9(X, X, X, X, X, X, X, X, X)
{
    return 0;
}

// fv_*

void fv_0()
{
}

void fv_1(X)
{
}

void fv_2(X, X)
{
}

void fv_3(X, X, X)
{
}

void fv_4(X, X, X, X)
{
}

void fv_5(X, X, X, X, X)
{
}

void fv_6(X, X, X, X, X, X)
{
}

void fv_7(X, X, X, X, X, X, X)
{
}

void fv_8(X, X, X, X, X, X, X, X)
{
}

void fv_9(X, X, X, X, X, X, X, X, X)
{
}

template<class F> void test_eq(F f1, F f2)
{
#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

    using hpx::function_equal;

#endif

    HPX_TEST( function_equal( f1, f2 ) );
}

template<class F> void test_ne(F f1, F f2)
{
#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

    using hpx::function_equal;

#endif

    HPX_TEST( !function_equal( f1, f2 ) );
}

// 0

template<class F> void test_0(F f)
{
    test_eq( hpx::util::bind(f), hpx::util::bind(f) );
}

// 1

template<class F, class V> void test_1_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1), hpx::util::bind(f, v1) );
    test_ne( hpx::util::bind(f, v1), hpx::util::bind(f, v2) );
}

template<class F> void test_1(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1), hpx::util::bind(f, placeholders::_1) );

    test_1_( f, X(1), X(2) );

    X a(0), b(0);
    test_1_( f, boost::ref(a), boost::ref(b) );
}

// 2

template<class F, class V> void test_2_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1), hpx::util::bind(f, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1), hpx::util::bind(f, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1), hpx::util::bind(f, v2, v1) );
}

template<class F> void test_2(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2), hpx::util::bind(f, placeholders::_1, placeholders::_2) );

    test_2_( f, X(1), X(2) );

    X a(0), b(0);
    test_2_( f, boost::ref(a), boost::ref(b) );
}

// 3

template<class F, class V> void test_3_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1, v1), hpx::util::bind(f, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1), hpx::util::bind(f, v1, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1, v1), hpx::util::bind(f, v1, v2, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1), hpx::util::bind(f, v2, v1, v1) );
}

template<class F> void test_3(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3), hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3) );

    test_3_( f, X(1), X(2) );

    X a(0), b(0);
    test_3_( f, boost::ref(a), boost::ref(b) );
}

// 4

template<class F, class V> void test_4_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v2, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1), hpx::util::bind(f, v1, v2, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1), hpx::util::bind(f, v2, v1, v1, v1) );
}

template<class F> void test_4(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4), hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4) );

    test_4_( f, X(1), X(2) );

    X a(0), b(0);
    test_4_( f, boost::ref(a), boost::ref(b) );
}

// 5

template<class F, class V> void test_5_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v2, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v2, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v2, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1), hpx::util::bind(f, v2, v1, v1, v1, v1) );
}

template<class F> void test_5(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5), hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5) );

    test_5_( f, X(1), X(2) );

    X a(0), b(0);
    test_5_( f, boost::ref(a), boost::ref(b) );
}

// 6

template<class F, class V> void test_6_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v2, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v2, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v2, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v2, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v2, v1, v1, v1, v1, v1) );
}

template<class F> void test_6(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6), hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6) );

    test_6_( f, X(1), X(2) );

    X a(0), b(0);
    test_6_( f, boost::ref(a), boost::ref(b) );
}

// 7

template<class F, class V> void test_7_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v2, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v2, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v2, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v2, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v2, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v2, v1, v1, v1, v1, v1, v1) );
}

template<class F> void test_7(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7), hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7) );

    test_7_( f, X(1), X(2) );

    X a(0), b(0);
    test_7_( f, boost::ref(a), boost::ref(b) );
}

// 8

template<class F, class V> void test_8_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v2, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v2, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v2, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v2, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v2, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v2, v1, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v2, v1, v1, v1, v1, v1, v1, v1) );
}

template<class F> void test_8(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, placeholders::_8), hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, placeholders::_8) );

    test_8_( f, X(1), X(2) );

    X a(0), b(0);
    test_8_( f, boost::ref(a), boost::ref(b) );
}

// 9

template<class F, class V> void test_9_(F f, V v1, V v2)
{
    test_eq( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v2) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v2, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v2, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v1, v2, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v1, v2, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v1, v2, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v1, v2, v1, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v1, v2, v1, v1, v1, v1, v1, v1, v1) );
    test_ne( hpx::util::bind(f, v1, v1, v1, v1, v1, v1, v1, v1, v1), hpx::util::bind(f, v2, v1, v1, v1, v1, v1, v1, v1, v1) );
}

template<class F> void test_9(F f)
{
    test_eq( hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, placeholders::_8, placeholders::_9), hpx::util::bind(f, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, placeholders::_8, placeholders::_9) );

    test_9_( f, X(1), X(2) );

    X a(0), b(0);
    test_9_( f, boost::ref(a), boost::ref(b) );
}

int main()
{
    // 0

    test_0( f_0 );
    test_0( fv_0 );

    // 1

    test_1( f_1 );
    test_1( fv_1 );

    // 2

    test_2( f_2 );
    test_2( fv_2 );

    // 3

    test_3( f_3 );
    test_3( fv_3 );

    // 4

    test_4( f_4 );
    test_4( fv_4 );

    // 5

    test_5( f_5 );
    test_5( fv_5 );

    // 6

    test_6( f_6 );
    test_6( fv_6 );

    // 7

    test_7( f_7 );
    test_7( fv_7 );

    // 8

    test_8( f_8 );
    test_8( fv_8 );

    // 9

    test_9( f_9 );
    test_9( fv_9 );

    return hpx::util::report_errors();
}