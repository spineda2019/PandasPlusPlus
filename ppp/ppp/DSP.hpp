#ifndef PPP_PPP_DSP_HPP
#define PPP_PPP_DSP_HPP

#include <algorithm>
#include <complex>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>
namespace ppp {
namespace detail {
template <class T>
inline void CalculateFrequencyDomain(const std::vector<T> &signal,
                                     std::vector<T> &array_to_fill,
                                     std::size_t sample_rate) {
    std::iota(array_to_fill.begin(), array_to_fill.end(), T(0));

    T scaler = static_cast<T>(sample_rate) / signal.size();
    std::transform(std::execution::par, array_to_fill.begin(),
                   array_to_fill.end(), array_to_fill.begin(),
                   [&scaler](T &x) { return x * scaler; });
}

/**
 * @brief Calculate Discrete Fourier Transform of a time domain waveform
 *
 * @param[in] signal: waveform to transform
 * @param[out] domain_to_fill: non-empty pre-filled destination vector
 *
 */
template <class T>
inline void CalculateDiscreteFT(const std::vector<T> &signal,
                                std::vector<std::complex<T>> &domain_to_fill) {
    std::vector<std::vector<std::complex<T>>> k(signal.size());  // Column vec

    // init k w/ 0,0
    constexpr std::complex<T> zero_z(T(0.0), T(0.0));
    std::fill(std::execution::par_unseq, k.begin(), k.end(),
              std::vector<std::complex<T>>{zero_z});

    // multiply k.T * k to create a square matrix
    for (std::size_t row = 0; row < k.size(); row++) {
        for (std::size_t col = 1; col < k.size(); col++) {
            k[row].push_back(std::complex<T>(T(col) * T(row), T(0)));
        }
    }

    // e = exp(-2j * pi * k.T * k / N)
    constexpr std::complex<T> comp(T(0.0), T(-2.0));
    const T N = T(k.size());
    std::for_each(
        std::execution::par_unseq, k.begin(), k.end(), [&comp, &N](auto &x) {
            std::transform(std::execution::par, x.begin(), x.end(), x.begin(),
                           [&comp, &N](std::complex<T> &y) {
                               return std::pow(T(2.71828),
                                               comp * T(3.141592654) * y / N);
                           });
        });

    // e dot signal
    std::transform(
        std::execution::par_unseq, k.begin(), k.end(), domain_to_fill.begin(),
        [&signal, &N](auto &x) {
            const std::complex<T> dot =
                std::inner_product(x.begin(), x.end(), signal.begin(),
                                   std::complex<T>(T(0.0), T(0.0)));
            return dot / (std::complex<T>(N) / std::complex<T>(T(2.0), T(0.0)));
        });
}

/**
 * @brief Extract N frequencies from a signal from highest amplitude to lowest
 *
 * @param[in] signal: waveform to extract frequencies
 * @param[out] dest: empty destination vector for extracted frequencies
 * @param[in] sample_rate: sample rate of signal
 * @param[in] n: desired number of frequencies to get (optional, defaults to
 * 3)
 *
 */
template <class T>
inline void ExtractNFrequencies(const std::vector<T> &signal,
                                std::vector<T> &dest,
                                const std::size_t sample_rate,
                                const std::size_t n) {
    if (dest.size()) {
        std::cout << "Error: Non-empty destination vector received"
                  << std::endl;
        return;
    }

    std::vector<T> frequency_domain(signal.size());
    CalculateFrequencyDomain(signal, frequency_domain, sample_rate);

    std::vector<std::complex<T>> dft_domain(signal.size());
    CalculateDiscreteFT(signal, dft_domain);

    std::vector<T> real(signal.size() / 2);
    std::transform(std::execution::par_unseq, dft_domain.begin(),
                   dft_domain.begin() + real.size(), real.begin(),
                   [](std::complex<T> &x) { return std::abs(x); });

    auto it = std::max_element(real.begin(), real.end());
    for (std::size_t i = 0; i < n; i++) {
        // value on freq domain at index of highest val in dft domain
        dest.push_back(frequency_domain[std::distance(real.begin(), it)]);

        real[std::distance(real.begin(), it)] = 0;

        it = std::max_element(real.begin(), real.end());
    }
}

/**
 * @brief Extract N amplitudes from a signal from highest amplitude to lowest
 *
 * @param[in] signal: waveform to extract amplitudes
 * @param[out] dest: empty destination vector for extracted amplitudes
 * @param[in] n: desired number of amplitudes to get (optional, defaults to 3)
 *
 */
template <class T>
inline void ExtractNAmplitudes(const std::vector<T> &signal,
                               std::vector<T> &dest, const std::size_t n) {
    if (dest.size()) {
        std::cout << "Error: Non-empty destination vector received"
                  << std::endl;
        return;
    }

    std::vector<std::complex<T>> dft_domain(signal.size());
    CalculateDiscreteFT(signal, dft_domain);

    std::vector<T> real(signal.size() / 2);
    std::transform(std::execution::par_unseq, dft_domain.begin(),
                   dft_domain.begin() + real.size(), real.begin(),
                   [](std::complex<T> &x) { return std::abs(x); });

    auto it = std::max_element(real.begin(), real.end());
    for (std::size_t i = 0; i < n; i++) {
        dest.push_back(*it);

        real[std::distance(real.begin(), it)] = 0;

        it = std::max_element(real.begin(), real.end());
    }
}

/**
 * @brief Extract N phases from a signal from highest amplitude to lowest
 *
 * @param[in] signal: waveform to extract phases
 * @param[out] dest: empty destination vector for extracted phases
 * @param[in] n: desired number phases to get (optional, defaults to 3)
 *
 */
template <class T>
inline void ExtractNPhases(const std::vector<T> &signal, std::vector<T> &dest,
                           const std::size_t n) {
    if (dest.size()) {
        std::cout << "Error: Non-empty destination vector received"
                  << std::endl;
        return;
    }

    std::vector<std::complex<T>> dft_domain(signal.size());
    CalculateDiscreteFT(signal, dft_domain);

    std::vector<T> real(signal.size() / 2);
    std::transform(std::execution::par_unseq, dft_domain.begin(),
                   dft_domain.begin() + real.size(), real.begin(),
                   [](std::complex<T> &x) { return std::abs(x); });

    std::complex<T> max_el{};
    auto it = std::max_element(real.begin(), real.end());
    for (std::size_t i = 0; i < n; i++) {
        max_el = dft_domain[std::distance(real.begin(), it)];

        dest.push_back(std::atan2(max_el.imag(), max_el.real()));

        real[std::distance(real.begin(), it)] = 0;

        it = std::max_element(real.begin(), real.end());
    }
}
}  // namespace detail
}  // namespace ppp

#endif  // PPP_PPP_DSP_HPP
