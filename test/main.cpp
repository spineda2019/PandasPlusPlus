#include <ppp/Dataframe.hpp>
#include <vector>
int main(void) {
  std::vector<std::vector<float>> small{
      {3, 3},
      {4, 4},
  };

  ppp::Dataframe simple{small};

  std::cout << simple << std::endl;
}
