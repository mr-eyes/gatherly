#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <utils.hpp>
#include <gtl/phmap_dump.hpp>

using namespace std;

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


Gatherly::SplittedIndex::SplittedIndex(string directory, string input_prefix) {
    this->directory_path = directory;
    this->input_prefix = input_prefix;
    this->metadata_file = input_prefix + ".metadata";
    auto metadata_tup = this->load_metadata(this->metadata_file);
    this->scale = get<0>(metadata_tup);
    this->total_parts = get<1>(metadata_tup);

    for (int part_id = 1; part_id <= this->total_parts; part_id++) {
        string part_name = "part" + to_string(part_id);
        string _file_id_to_name = input_prefix + "_id_to_name.tsv";
        string _file_id_to_kmer_count = input_prefix + "_groupID_to_kmerCount.bin";
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