#ifndef CSVLIBRARY_EXCEPTIONS_H
#define CSVLIBRARY_EXCEPTIONS_H

#include <exception>

class DataframeRowSizeMismatchException : public std::exception {
 public:
  const char* what() const throw() {
    return "Size of Dataframe and interacting vector are incompatible";
  }
};

#endif  // CSVLIBRARY_EXCEPTIONS_H