#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdint>
#include <tuple>
#include <gtl/phmap.hpp>


using namespace std;

using PHMAP_COLOR_TO_ID = gtl::flat_hash_map<uint64_t, gtl::flat_hash_set<uint32_t>>;
using PHMAP_COLOR_TO_COUNT = gtl::flat_hash_map<uint64_t, uint64_t>;
using PHMAP_ID_TO_NAME = gtl::flat_hash_map<string, uint64_t>;
using PHMAP_COLOR_TO_IDS = gtl::flat_hash_map<uint64_t, gtl::flat_hash_set<uint32_t>>;


namespace Gatherly {

    struct IndexStruct {
        string color_to_source;
        string kmer_to_color;
        string color_to_count;
    };

    class SplittedIndex {
    public:

        string directory_path;
        string input_prefix;
        string metadata_file;
        int total_parts;
        int scale;
        vector<IndexStruct> index_parts;
        PHMAP_ID_TO_NAME name_to_id;

        SplittedIndex(string directory, string input_prefix);
        void load_colors_to_ids(const std::string& filename, PHMAP_COLOR_TO_IDS* map);
        void load_color_to_count(const std::string& filename, PHMAP_COLOR_TO_COUNT* map);
        void load_id_to_name(const std::string& filename);
        tuple<uint64_t, uint64_t> load_metadata(const std::string& filename);

    };

};