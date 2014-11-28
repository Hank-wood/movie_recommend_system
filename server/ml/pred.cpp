#include "pred.hpp"

namespace ML {
    const int FEATURE = 10;

    vector<Movie> predicate(const vector<vector<int>>& mtrx, int TOP) {
        vector<Movie> ret;

        ublas::matrix<double> y(mtrx.size(), mtrx[0].size());
        ublas::matrix<bool> present(y.size1(), y.size2());
        for (std::size_t i = 0; i < y.size1(); ++i) {
            for (std::size_t j = 0; j < y.size2(); ++j) {
                y(i, j) = mtrx[i][j];
                present(i, j) = (mtrx[i][j] != 0);
            }
        }
        
        auto pr = train(y, present, 0.000004, 100);
        auto theta = pr.first, x = pr.second;
        ublas::matrix<double> recommends = ublas::prod(theta, ublas::trans(x));

        vector<pair<int, double>> order;
        for (std::size_t i = 0; i < recommends.size2(); ++i)
            order.emplace_back(i, recommends(recommends.size1() - 1, i));
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(order.begin(), order.end(),
                     std::default_random_engine(seed));
        std::sort(order.begin(), order.end(),
                  [](const auto& lhs,
                     const auto& rhs) { return lhs.second < rhs.second; });

        while (TOP-- > 0 && order.size() > 0) {
            Movie m = { order.back().first };
            m.vote = order.back().second + 0.5;
            order.pop_back();
            ret.push_back(std::move(m));
        }
        return std::move(ret);
    }

    pair<ublas::matrix<double>, ublas::matrix<double>>
    train(const ublas::matrix<double>& mtrx, const ublas::matrix<bool>& present, double alpha, int iters) {
        const std::size_t USERS = mtrx.size1(), MOVIES = mtrx.size2();

        ublas::matrix<double> theta(USERS, FEATURE), x(MOVIES, FEATURE);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
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

#ifndef NDEBUG
        std::cout << "Start to train model.\n";
#endif
        while (iters-- > 0) {
            auto res = cost_func(theta, x, present, mtrx, 10);
            double cost = std::get<0>(res);
            ublas::matrix<double> &theta_grad = std::get<1>(res),
                                  &x_grad = std::get<2>(res);
            theta -= alpha * theta_grad;
            x -= alpha * x_grad;
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

        double ret = 0.0;
        for (std::size_t i = 0; i < theta.size1(); ++i) {
            for (std::size_t j = 0; j < theta.size2(); ++j)
                ret += theta(i, j) * theta(i, j);
        }
        for (std::size_t i = 0; i < x.size1(); ++i) {
            for (std::size_t j = 0; j < x.size2(); ++j)
                ret += x(i, j) * x(i, j);
        }
        ret *= lambda / 2.0;
        for (std::size_t i = 0; i < theta.size1(); ++i) {
            for (std::size_t j = 0; j < x.size1(); ++j) {
                if (!present(i, j))
                    continue;
                double t = 0.0;
                for (std::size_t k = 0; k < theta.size2(); ++k)
                    t += theta(i, k) * x(j, k);
                ret += (t - mtrx(i, j)) * (t - mtrx(i, j)) / 2.0;
            }
        }

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
