#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <gatherly.hpp>
#include <gtl/phmap_dump.hpp>

using namespace std;


inline uint64_t to_uint64_t(std::string const& value) {
    uint64_t result = 0;
    char const* p = value.c_str();
    char const* q = p + value.size();
    while (p < q) {
        result *= 10;
        result += *(p++) - '0';
    }
    return result;
}

tuple<uint64_t, uint64_t> Gatherly::SplittedIndex::load_metadata(const std::string& filename) {
    gtl::flat_hash_map<string, int> metadata_map;
    string line;
    ifstream fin(string(input_prefix + ".metadata").c_str());
    while (std::getline(fin, line)) {
        std::vector<string> tokens;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ',')) tokens.push_back(token);
        metadata_map[tokens[0]] = stoi(tokens[1]);
    }
    fin.close();
    tuple<uint64_t, uint64_t> tmp_res;
    tmp_res = make_pair(metadata_map["scale"], metadata_map["parts"]);
    return tmp_res;
}

void Gatherly::SplittedIndex::load_id_to_kmer_count(const std::string& filename) {
    gtl::BinaryInputArchive ar_in(filename.c_str());
    this->id_to_kmer_count.phmap_load(ar_in);
    assert(this->id_to_kmer_count.size());
}

void Gatherly::SplittedIndex::load_id_to_name(const std::string& filename) {
    string line;
    ifstream fin(filename.c_str());
    std::getline(fin, line);
    while (std::getline(fin, line)) {
        std::vector<string> tokens;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ',')) tokens.push_back(token);
        this->id_to_name[to_uint64_t(tokens[0])] = tokens[1];
    }
    fin.close();
}

void Gatherly::SplittedIndex::load_color_to_count(const std::string& filename, PHMAP_COLOR_TO_COUNT* map) {
    gtl::BinaryInputArchive ar_in_colorsCount(filename.c_str());
    map->phmap_load(ar_in_colorsCount);
    assert(map->size());
}

// Kinda slow, but noway.
void Gatherly::SplittedIndex::load_colors_to_ids(const std::string& filename, PHMAP_COLOR_TO_IDS* map) {
    gtl::BinaryInputArchive ar_in(filename.c_str());
    size_t size;
    ar_in.loadBinary(&size);
    map->reserve(size);
    while (size--)
    {
        uint64_t k;
        gtl::flat_hash_set<uint32_t> v;
        ar_in.loadBinary(&k);
        ar_in.loadBinary(&v);
        map->insert_or_assign(std::move(k), std::move(v));
    }
}

Gatherly::SplittedIndex::SplittedIndex(string directory, string input_prefix) {
    string _file_id_to_name = input_prefix + "_id_to_name.tsv";
    string _file_id_to_kmer_count = input_prefix + "_groupID_to_kmerCount.bin";
    this->directory_path = directory;
    this->input_prefix = input_prefix;
    this->metadata_file = input_prefix + ".metadata";
    auto metadata_tup = this->load_metadata(this->metadata_file);
    this->scale = get<0>(metadata_tup);
    this->total_parts = get<1>(metadata_tup);
    this->load_id_to_kmer_count(_file_id_to_kmer_count);
    this->load_id_to_name(_file_id_to_name);

    for (int part_id = 1; part_id <= this->total_parts; part_id++) {
        string part_name = "part" + to_string(part_id);
        string _file_color_to_sources = input_prefix + "_color_to_sources." + part_name + ".bin";
        string _file_color_to_count = input_prefix + "_color_count." + part_name + ".bin";
        string _file_kmer_to_color = input_prefix + "kmer_to_color." + part_name + ".phmap";
        IndexStruct tmp_IndexStruct;
        tmp_IndexStruct.color_to_count = _file_color_to_count;
        tmp_IndexStruct.color_to_source = _file_color_to_sources;
        tmp_IndexStruct.kmer_to_color = _file_kmer_to_color;
        this->index_parts.emplace_back(tmp_IndexStruct);
    }

}