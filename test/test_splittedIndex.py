from pydoc import splitdoc
import gatherly

Gath = gatherly.SplittedIndex("/home/mabuelanin/dib-dev/gatherly/splitted_index/ok", 51)
Gath.load_all_parts()

# hh = 20236993234229
# x = Gath.get_sources_from_hash(hh);

sig_path = "/home/mabuelanin/dib-dev/gatherly/sigs_100k/DRR083176.sig"
fasta_path = "/home/mabuelanin/dib-dev/gatherly/DRR083176_1.fastq.gz"
# result = Gath.query_sig(sig_path)
result = Gath.query_fastx(fasta_path)

print(result)
