from pydoc import splitdoc
import splitted_index

Gath = splitted_index.SplittedIndex("/home/mabuelanin/dib-dev/gatherly/splitted_index/ok")

print(dir(Gath))

Gath.load_all_parts()

hh = 20236993234229
x = Gath.get_sources_from_hash(hh);

print(x)