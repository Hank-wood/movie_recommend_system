#include "pred.hpp"

namespace ML {
    const int FEATURE = 10;

    vector<Movie> predicate(const vector<vector<int>>& mtrx, int TOP) {
        vector<Movie> ret;
        ublas::matrix<double> y(mtrx.size(), mtrx[0].size());
        
        auto pr = train(y, 0.0075, 15);
        auto theta = pr.first, x = pr.second;
        ublas::matrix<double> recommends = ublas::prod(theta, ublas::trans(x));
        vector<pair<int, double>> order;
        for (std::size_t i = 0; i < recommends.size2(); ++i)
            order.emplace_back(i, recommends(recommends.size1() - 1, i));
        std::sort(order.begin(), order.end(),
                  [](const auto& lhs,
                     const auto& rhs) { return lhs.second < rhs.second; });

        while (TOP-- > 0 && order.size() > 0) {
            Movie m = { order.back().first };
            order.pop_back();
            ret.push_back(std::move(m));
        }
        return std::move(ret);
    }

    pair<ublas::matrix<double>, ublas::matrix<double>>
    train(const ublas::matrix<double>& mtrx, double alpha, int iters) {
        const std::size_t USERS = mtrx.size1(), MOVIES = mtrx.size2();

        ublas::matrix<double> theta(USERS, FEATURE), x(MOVIES, FEATURE);
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0, 1);
        auto rnd = std::bind(distribution, generator);
        for (std::size_t i = 0; i < theta.size1(); ++i) {
            for (std::size_t j = 0; j < theta.size2(); ++j)
                theta(i, j) = rnd();
        }
        for (std::size_t i = 0; i < x.size1(); ++i) {
            for (std::size_t j = 0; j < x.size2(); ++j)
                x(i, j) = rnd();
        }

        ublas::matrix<bool> present(USERS, MOVIES);
        for (std::size_t i = 0; i < USERS; ++i) {
            for (std::size_t j = 0; j < MOVIES; ++j)
                present(i, j) = (mtrx(i, j) != 0);
        }
#ifndef NDEBUG
        std::cout << "Start to train model.\n";
#endif
        while (iters-- > 0) {
            auto res = cost_func(theta, x, present, mtrx, 10);
            double cost = std::get<0>(res);
            ublas::matrix<double> &theta_grad = std::get<1>(res),
                                  &x_grad = std::get<2>(res);
#ifndef NDEBUG
            //std::cout << theta_grad << std::endl;
            //std::cout << x_grad << std::endl;
#endif
            theta -= alpha * theta_grad;
            x -= alpha * x_grad;
#ifndef NDEBUG
            std::cout << theta << std::endl;
            std::cout << x << std::endl;
#endif
#ifndef NDEBUG
            std::cout << "remain " << iters
                      << " times, the cost value = " << cost << "\n";
#endif
        }
        return std::move(std::make_pair(theta, x));
    }

    tuple<double, ublas::matrix<double>, ublas::matrix<double>>
    cost_func(const ublas::matrix<double>& theta, ublas::matrix<double>& x,
              const ublas::matrix<bool>& present,
              const ublas::matrix<double>& mtrx, double lambda) {
        const std::size_t USERS = theta.size1(), MOVIES = x.size1();

        auto dot_pow2 = [](const auto& x) { return ublas::element_prod(x, x); };
        auto sum = [](const auto& x) {
            return ublas::sum(std::move(
                ublas::prod(ublas::scalar_vector<double>(x.size1()), x)));
        };

        double ret =
            sum(std::move(ublas::prod(
                std::move(dot_pow2(std::move(
                    std::move(ublas::prod(theta, ublas::trans(x))) - mtrx))),
                std::move(ublas::trans(present))))) / 2.0 +
            lambda / 2.0 *
                (sum(std::move(dot_pow2(theta))) + sum(std::move(dot_pow2(x))));

        // Compute the gradient of theta and x.
        ublas::matrix<double> theta_grad(theta.size1(), theta.size2()),
            x_grad(x.size1(), x.size2());
#ifndef NDEBUG
        std::cout << "initialize theta_grad\n";
#endif
        for (std::size_t i = 0; i < theta.size1(); ++i) {
            for (std::size_t j = 0; j < theta.size2(); ++j) {
                double& ans = theta_grad(i, j);
                ans = lambda * theta(i, j);
                for (std::size_t k = 0; k < MOVIES; ++k) {
                    if (present(i, k))
                        ans += (theta(i, j) * x(k, j) - mtrx(i, k)) * x(k, j);
                }
            }
        }
#ifndef NDEBUG
        std::cout << "initialize x_grad\n";
#endif
        for (std::size_t i = 0; i < x.size1(); ++i) {
            for (std::size_t j = 0; j < x.size2(); ++j) {
                double& ans = x_grad(i, j);
                ans = lambda * x(i, j);
                for (std::size_t k = 0; k < USERS; ++k) {
                    if (present(k, i))
                        ans += (theta(k, j) * x(i, j) - mtrx(k, i)) * theta(k, j);
                }
            }
        }
#ifndef NDEBUG
        std::cout << "cost_func done\n";
#endif
        return std::move(std::make_tuple(ret, theta_grad, x_grad));
    }
}
