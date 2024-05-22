/*
 *  pppmodule.c
 *  The top level c file for binding the ppp lib to python
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

/* Special Python Header */
#define PY_SSIZE_T_CLEAN
#include <Python.h>

/* Everything Else */
#include <stdio.h>

#include <cstddef>

#include "listobject.h"
#include "longobject.h"
#include "methodobject.h"
#include "modsupport.h"
#include "moduleobject.h"
#include "object.h"
#include "ppp/Column.hpp"
#include "ppp_column.h"
#include "pyerrors.h"
#include "pyport.h"
#include "pytypedefs.h"

/* ************************ FColumn Object & Methods ************************ */

static void DeallocFColumn(PyObject *self) {
    DeleteFColumn(reinterpret_cast<FColumn>(self));
}

static int InitFColumn(PyObject *self, PyObject *args, PyObject *kwds) {
    (void)kwds;

    float *list_start;
    const char *key_str;

    if (!PyArg_ParseTuple(args, "[f]s", list_start, key_str)) {
        return -1;
    }

    PyObject *float_list = PyList_GetItem(args, 0);
    if (float_list == NULL) {
        return -1;
    }

    if (!PyList_Check(float_list)) {
        return -1;
    }

    PyErr_Clear();
    size_t length = static_cast<size_t>(PyList_Size(float_list));
    if (PyErr_Occurred()) {
        return -1;
    }

    self = (PyObject *)NewFColumn(list_start, length, key_str);
    return 0;
}

static PyObject *FSumFunction(PyObject *self, PyObject *args) {
    (void)args;
    return (PyObject *)10;
}

static struct PyMethodDef FSum = {
    "sum",
    FSumFunction,
    METH_NOARGS,
    "Sum the contents of a column of floats",
};

static PyMethodDef FColumnMethods[] = {
    {
        "sum",
        FSumFunction,
        METH_NOARGS,
        "Sum the float columns entries",
    },
    {
        NULL,
        NULL,
        0,
        NULL,
    },
};

static PyTypeObject PyFColumn = {
    .tp_name = "ppp.FColumn",
    .tp_basicsize = sizeof(ppp::Column<float>),
    .tp_dealloc = (destructor)DeallocFColumn,
    .tp_doc = "PPP Column of Floats",
    .tp_methods = FColumnMethods,
    .tp_init = (initproc)InitFColumn,
};

static struct PyModuleDef ppp_module = {
    .m_name = "ppp",
    .m_doc = "PandasPlusPlus, a knockoff Pandas written in C++",
};

PyMODINIT_FUNC PyInit_ppp(void) {
    PyObject *m = PyModule_Create(&ppp_module);
    if (m == NULL) {
        return NULL;
    };

    if (PyModule_AddType(m, &PyFColumn) < 0) {
        return NULL;
    }

    return m;
}

static PyObject *spam_system(PyObject *self, PyObject *args) {
    const char *command;

    int sts;

    if (!PyArg_ParseTuple(args, "S", &command)) {
        return NULL;
    } else {
        printf("Hello from C");
        return PyLong_FromLong(0);
    }
}
