/*
 *  Exceptions.hpp
 *  Definitions for custom fatal exceptions used by the library
 *
 *  Copyright (C) 2024 Sebastian Pineda (spineda.wpi.alum@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License Version 3.0 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License and GNU Lesser General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

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
        return "Dataframe's header row and interacting operation are "
               "incompatible. "
               "Make sure your dataframe has headers if you are expecting or "
               "working with them or vice versa";
    }
};

class HeaderLabelMismatchException : public std::exception {
 public:
    const char* what() const throw() {
        return "Dataframe's header row and interacting operation are "
               "incompatible. "
               "Make sure your dataframe and what it is interacting with have "
               "the "
               "same headers if they have them";
    }
};

class BadDataframeShapeException : public std::exception {
 public:
    const char* what() const throw() {
        return "Vector used to create a dataframe is not a square";
    }
};

class HeaderDataSizeMismatchException : public std::exception {
 public:
    const char* what() const throw() {
        return "Width of vector used to create a dataframe and width of header "
               "row "
               "as not equal";
    }
};

#endif  // CSVLIBRARY_EXCEPTIONS_H
