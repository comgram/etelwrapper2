# cython: language_level=3
# distutils: language = c++
# distutils: sources = interface.cpp

import copy
import numpy as np

# header start
ctypedef long long __int64
cdef extern from "interface.h":
    void _close()
    void _open(const char *url)
    void etel_wrapper()
# header end

def open(url="etb:USB:0"):
    _open(url)

def close():
    _close()
