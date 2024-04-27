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

static PyObject *spam_sytem(PyObject *self, PyObject *args) {
    const char *command;

    int sts;

    if (!PyArg_ParseTuple(args, "S", &command)) {
        return NULL;
    } else {
        printf("Hello from C");
        return PyLong_FromLong(0);
    }
}
