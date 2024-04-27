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
