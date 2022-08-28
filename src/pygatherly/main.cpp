#include <pybind11/pybind11.h>
#include <gatherly.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
using namespace Gatherly;


PYBIND11_MODULE(python_example, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: python_example

        .. autosummary::
           :toctree: _generate

           gatherly_test
    )pbdoc";
    py::class_<SplittedIndex>(m, "SplittedIndex")
        .def(py::init<const std::string&>())
        .def("gatherly_test", &SplittedIndex::gatherly_test);
}

