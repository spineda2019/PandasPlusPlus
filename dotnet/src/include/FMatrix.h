#ifndef DOTNET_SRC_INCLUDE_FMATRIX_H_
#define DOTNET_SRC_INCLUDE_FMATRIX_H_

#include <Windows.h>
#include <vcclr.h>

#include "ppp/Matrix.hpp"
#using < System.dll>

namespace ppp {
public
ref class FMatrix {
 public:
    FMatrix(float** beginning, std::size_t row, std::size_t columns);
    ~FMatrix();

 private:
    Matrix<float>* inner_;
};
}  // namespace ppp

#endif  // DOTNET_SRC_INCLUDE_FMATRIX_H_
