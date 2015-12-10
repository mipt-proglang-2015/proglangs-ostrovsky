#include <iostream>
#include <algorithm>
#include <vector>

extern "C" {
#include <Python.h>
}

using std::vector;
using std::cout;
using std::endl;

static vector<vector<double> > pyobject_to_cxx(PyObject * p_matrix)
{
	vector<vector<double> > result(PyObject_Length(p_matrix));
	for (int i = 0; i < result.size(); ++i) {
		PyObject * py_row = PyList_GetItem(p_matrix, i);
		vector<double> c_row (PyObject_Length (py_row));
        for (int j = 0; j < c_row.size(); ++j) {
            PyObject * py_elem = PyList_GetItem(py_row, j);
            c_row[j] = PyFloat_AsDouble(py_elem);
        }
		result[i] = c_row;
	}
	return result;
}

static PyObject * cxx_to_pyobject(const vector<vector<double> >& matrix)
{
	PyObject * result = PyList_New(matrix.size());
	for (int i = 0; i < matrix.size(); ++i) {
		
		PyObject * py_row = PyList_New(matrix.size());
		PyObject * res_row = PyList_New(matrix[i].size());
		for (int j = 0; j < matrix[i].size(); ++j) {
            PyObject * res_elem = PyFloat_FromDouble(matrix[i][j]);
            PyList_SetItem(res_row, j, res_elem);
		}
		PyList_SetItem(result, i, res_row);
	}
	return result;
}


static PyObject * FloydAlgorithm(PyObject* module, PyObject* args)
{ 


	vector<vector<double> > matrix = pyobject_to_cxx(PyTuple_GetItem(args, 0));
	for (int k = 0; k < matrix.size(); k++) {
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix.size(); j++) {
				if (i != j && matrix[i][k]+matrix[k][j] != 0)
				{
					double resistance = matrix[i][k] + matrix[k][j];
					matrix[i][j] = resistance * matrix[i][j]/(resistance + matrix[i][j]);
				}
			}
		}
	}
	
	return cxx_to_pyobject(matrix);
}


PyMODINIT_FUNC PyInit_floyd()
{
	static PyMethodDef ModuleMethods[] = {
		{ "FloydAlgorithm", FloydAlgorithm, METH_VARARGS, NULL },
		{ NULL }
	};
	static PyModuleDef ModuleDef = {
		PyModuleDef_HEAD_INIT,
		"floyd",
		"algorithm",
		-1, ModuleMethods, 
		NULL, NULL, NULL, NULL
	};
	PyObject * module = PyModule_Create(&ModuleDef);
	return module;
}

