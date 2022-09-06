#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <gatherly.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
using namespace Gatherly;


PYBIND11_MODULE(gatherly, m) {
    m.doc() = R"pbdoc(
        An ultrafast gather!
        -----------------------

        .. currentmodule:: gatherly

        .. autosummary::
           :toctree: _generate

           gatherly
)pbdoc";
    py::class_<SplittedIndex>(m, "SplittedIndex")
        .def(py::init<const std::string&, int>())
        .def("load_single_part", &SplittedIndex::load_single_part)
        .def("load_all_parts", &SplittedIndex::load_all_parts)
        .def("get_ids_from_hash", &SplittedIndex::get_ids_from_hash)
        .def("kmer_to_part", &SplittedIndex::kmer_to_part)
        .def("get_color_from_hash", &SplittedIndex::get_color_from_hash)
        .def("get_sources_from_hash", &SplittedIndex::get_sources_from_hash)
        .def("get_ids_from_hash", &SplittedIndex::get_ids_from_hash)
        .def("query_sig", &SplittedIndex::query_sig)
        .def("query_fastx", &SplittedIndex::query_fastx);
}
