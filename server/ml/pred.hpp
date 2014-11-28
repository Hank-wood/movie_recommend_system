#ifndef _PRED_HPP_
#define _PRED_HPP_

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <tuple>
#include <random>
#include <chrono>
#include <cstdint>
#include <cstring>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "../../common/movie.hpp"

namespace ML {
    extern const int FEATURE;

    namespace ublas = boost::numeric::ublas;

    using std::vector;
    using std::pair;
    using std::tuple;

    vector<Movie> predicate(const vector<vector<int>>&, int);

    // The first of pair is theta, and the second is x.
    // theta is a USER * FEATURE matrix, x is a MOIVE * FEATURE matrix.
    pair<ublas::matrix<double>, ublas::matrix<double>>
    train(const ublas::matrix<double>&, const ublas::matrix<bool>&, double,
          int);

    // The first matrix is theta, and the second is x.
    // Same as above.
    tuple<double, ublas::matrix<double>, ublas::matrix<double>>
    cost_func(const ublas::matrix<double>&, ublas::matrix<double>&,
              const ublas::matrix<bool>&, const ublas::matrix<double>&,
              double);
}

#endif
