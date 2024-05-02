#include "include/FMatrix.h"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "ppp/Matrix.hpp"

namespace ppp {

FMatrix::FMatrix(float** beginning, std::size_t rows, std::size_t columns)
    : inner_{nullptr} {
    std::vector<std::vector<float>> data{rows, std::vector<float>(columns, 0)};

    // HACK: :(
    for (std::size_t row{0}; row < rows; row++) {
        for (std::size_t col{0}; col < columns; columns++) {
            data[row][col] = beginning[row][col];
        }
    }

    std::optional<ppp::Matrix<float>> matrix = Matrix<float>::New(data);
    if (matrix.has_value()) {
        inner_ = new Matrix<float>(std::move(matrix.value()));
    }
}

FMatrix::~FMatrix() {
    if (inner_) {
        delete inner_;
    }
}

}  // namespace ppp
