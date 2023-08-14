#ifndef CSVLIBRARY_EXCEPTIONS_H
#define CSVLIBRARY_EXCEPTIONS_H

#include <exception>

class DataframeVectorSizeMismatchException : public std::exception {
 public:
  const char* what() const throw() {
    return "Size of Dataframe and interacting vector are incompatible";
  }
};

class ColumnNotFoundException : public std::exception {
 public:
  const char* what() const throw() {
    return "Specified column name does not exist";
  }
};

class DataframeIndexOutOfBoundsException : public std::exception {
 public:
  const char* what() const throw() {
    return "Dataframe operation goes out of bounds of Dataframe's data";
  }
};

class HeaderStateMismatchException : public std::exception {
 public:
  const char* what() const throw() {
    return "Dataframe's header row and interacting operation are incompatible. "
           "Make sure your dataframe has headers if you are expecting or "
           "working with them";
  }
};

#endif  // CSVLIBRARY_EXCEPTIONS_H