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

void Gatherly::SplittedIndex::load_color_to_count(int part_id) {
    string filePath = this->index_parts[part_id].color_to_count;
    gtl::BinaryInputArchive ar_in_colorsCount(filePath.c_str());
    this->index_parts[part_id].color_to_count_map->phmap_load(ar_in_colorsCount);
    assert(this->index_parts[part_id].color_to_count_map->size());
}

// Kinda slow, but no alternative.
void Gatherly::SplittedIndex::load_colors_to_ids(int part_id) {
    string filePath = this->index_parts[part_id].color_to_sources;
    gtl::BinaryInputArchive ar_in(filePath.c_str());
    size_t size;
    ar_in.loadBinary(&size);
    this->index_parts[part_id].color_to_ids_map->reserve(size);
    while (size--)
    {
        uint64_t k;
        gtl::flat_hash_set<uint32_t> v;
        ar_in.loadBinary(&k);
        ar_in.loadBinary(&v);
        this->index_parts[part_id].color_to_ids_map->insert_or_assign(std::move(k), std::move(v));
    }
}

void Gatherly::SplittedIndex::load_kmer_to_color(int part_id) {
    string filePath = this->index_parts[part_id].kmer_to_color;
    gtl::BinaryInputArchive ar_in(filePath.c_str());
    this->index_parts[part_id].kmer_to_color_map->phmap_load(ar_in);
}


void Gatherly::SplittedIndex::load_single_part(int part_id) {
    this->load_colors_to_ids(part_id);
    this->load_kmer_to_color(part_id);
}


void Gatherly::SplittedIndex::load_all_parts() {
    for (int part_id = 1; part_id <= this->total_parts; part_id++)
        this->load_single_part(part_id);
}

uint64_t Gatherly::SplittedIndex::get_kmer_count(int & source_id) {
    return this->id_to_kmer_count[source_id];
}


void Gatherly::SplittedIndex::splitted_ranges(uint64_t scale, int parts) {
    uint64_t max_hash = UINT64_MAX / (uint64_t)scale;
    uint64_t from_hash = 0;
    uint64_t to_hash = 0;
    uint64_t step = (uint64_t)(max_hash / parts);
    for (int i = 1; i <= parts; i++) {
        to_hash += step;
        this->hash_ranges[i] = { from_hash, to_hash };
        from_hash += step;
    }

    if (to_hash < max_hash)
        this->hash_ranges[parts] = { get<0>(this->hash_ranges[parts]), max_hash };
}

int Gatherly::SplittedIndex::kmer_to_part(uint64_t& kmer_hash) {
    for (auto& [part_id, hash_interval] : this->hash_ranges)
        if (get<0>(hash_interval) <= kmer_hash && kmer_hash < get<1>(hash_interval))
            return part_id;
    return 0;
}


uint64_t Gatherly::SplittedIndex::get_color_from_hash(uint64_t& kmer_hash) {
    return this->index_parts[kmer_to_part(kmer_hash)].kmer_to_color_map->operator[](kmer_hash);
}

gtl::flat_hash_set<uint32_t> Gatherly::SplittedIndex::get_ids_from_color(uint64_t &kmer_color, int part_id) {
    return this->index_parts[part_id].color_to_ids_map->operator[](kmer_color);
}

gtl::flat_hash_set<uint32_t> Gatherly::SplittedIndex::get_ids_from_hash(uint64_t &kmer_hash) {
    int inferred_part_id = this->kmer_to_part(kmer_hash);
    uint64_t kmer_color = this->index_parts[inferred_part_id].kmer_to_color_map->operator[](kmer_hash);
    return this->get_ids_from_color(kmer_color, inferred_part_id);
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
    this->splitted_ranges(this->scale, this->total_parts);


    for (int part_id = 1; part_id <= this->total_parts; part_id++) {
        string part_name = "part" + to_string(part_id);
        string _file_color_to_sources = input_prefix.append("_color_to_sources." + part_name + ".bin");
        string _file_color_to_count = input_prefix.append("_color_count." + part_name + ".bin");
        string _file_kmer_to_color = input_prefix.append("kmer_to_color." + part_name + ".phmap");
        IndexStruct tmp_IndexStruct;
        tmp_IndexStruct.color_to_count = _file_color_to_count;
        tmp_IndexStruct.color_to_sources = _file_color_to_sources;
        tmp_IndexStruct.kmer_to_color = _file_kmer_to_color;
        this->index_parts[part_id] = tmp_IndexStruct;
    }

}


