#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdint>
#include <tuple>
#include <parallel_hashmap/phmap.h>
#include <set>
#include <unordered_map>

using namespace std;
using phmap::flat_hash_map;
using phmap::flat_hash_set;
using phmap::parallel_flat_hash_map;



using PHMAP_COLOR_TO_COUNT = flat_hash_map<uint64_t, uint64_t>;
using PHMAP_ID_TO_NAME = flat_hash_map<uint64_t, string>;
using PHMAP_ID_TO_KMER_COUNT = flat_hash_map<uint32_t, uint32_t>;
using PHMAP_COLOR_TO_IDS = flat_hash_map<uint64_t, flat_hash_set<uint32_t>>;
using PHMAP_KMER_TO_COLOR = parallel_flat_hash_map<uint64_t, uint64_t,
    std::hash<uint64_t>, std::equal_to<uint64_t>, std::allocator<std::pair<const uint64_t, uint64_t>>, 2>;

namespace Gatherly {

    struct IndexStruct {
        string color_to_sources;
        string kmer_to_color;
        string color_to_count;
        PHMAP_COLOR_TO_IDS* color_to_ids_map;
        PHMAP_KMER_TO_COLOR* kmer_to_color_map;
        PHMAP_COLOR_TO_COUNT* color_to_count_map;
    };

    class SplittedIndex {
    public:

        string directory_path;
        string input_prefix;
        string metadata_file;
        int total_parts;
        int kSize;

        // Scale param used while indexing
        int scale;

        // stores the split index (complete or partial).
        flat_hash_map<int, IndexStruct> index_parts;

        // Map source id to its original name.
        PHMAP_ID_TO_NAME id_to_name;

        // Map source id to its unique canonical kmer count.
        PHMAP_ID_TO_KMER_COUNT id_to_kmer_count;

        // map that maps the part id to hash intervals<from_hash, to_hash>
        flat_hash_map<int, tuple<uint64_t, uint64_t>> hash_ranges;

        // Class constructor
        SplittedIndex(string input_prefix, int kSize);

        // Function to deserialize phmap of color to set of ids
        void load_colors_to_ids(int part_id);

        // deserialize phmap of colors to count of colors.
        void load_color_to_count(int part);

        // deserialize phmap of id to name (namesmap)
        void load_id_to_name(const std::string& filename);

        // deserialize phmap of id to kmer count (used in containment calculation)
        void load_id_to_kmer_count(const std::string& filename); // done

        // deserialize index-part hashed kmer to color phmap
        void load_kmer_to_color(int part_id);

        // parse metadata file
        tuple<uint64_t, uint64_t> load_metadata(const std::string& filename);

        // load single index part (color->count, color->sources, kmer->color)
        void load_single_part(int part_id);

        // load all parts
        void load_all_parts();

        // given a source ID, return the kmer count
        uint64_t get_kmer_count(int& source_id);

        // given a hashed_kmer, return its color
        uint64_t get_color_from_hash(uint64_t& kmer_hash);

        // given a hashed_kmer, return its associated sources (ids)
        set<uint32_t> get_ids_from_hash(uint64_t& kmer_hash);

        // given a hashed_kmer, return its associated sources (names)
        vector<string> get_sources_from_hash(uint64_t& kmer_hash);

        // given a hashed_kmer, return its associated sources
        flat_hash_set<uint32_t> get_ids_from_color(uint64_t& kmer_hash, int part_id);

        // given a scale value and number of pars, split the hash-space into equal hash intervals.
        void splitted_ranges(uint64_t scale, int parts);

        // TODO: Optimize to interval search
        int kmer_to_part(uint64_t& kmer_hash);


        /*
            QUERY
        */

       // TODO: allow searching with multiple kmer sizes
       // TODO: implement multithreading
       unordered_map<string, int> query_sig(string sig_path);


        // ~SplittedIndex();
    };

};