#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <gatherly.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
using namespace Gatherly;


PYBIND11_MODULE(splitted_index, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: splitted_index

        .. autosummary::
           :toctree: _generate

           splitted_index
    )pbdoc";
    py::class_<SplittedIndex>(m, "SplittedIndex")
        .def(py::init<const std::string&>())
        .def("load_single_part", &SplittedIndex::load_single_part)
        .def("load_all_parts", &SplittedIndex::load_all_parts)
        .def("get_ids_from_hash", &SplittedIndex::get_ids_from_hash)
        .def("kmer_to_part", &SplittedIndex::kmer_to_part)
        .def("get_color_from_hash", &SplittedIndex::get_color_from_hash)
        .def("get_sources_from_hash", &SplittedIndex::get_sources_from_hash)
        .def("get_ids_from_hash", &SplittedIndex::get_ids_from_hash);
}
