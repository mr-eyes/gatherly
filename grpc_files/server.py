import gatherly
import grpc
import gatherly_pb2
import gatherly_pb2_grpc
from concurrent import futures
import logging
import json
import sys
import argparse
import time

class Gatherly(gatherly_pb2_grpc.GatherlyServicer):
    def gather_kmer(self, request, context):
        result = ','.join(Gath.get_sources_from_hash(request.kmer_hash))
        return gatherly_pb2.KmerSources(sources=result)

    def gather_sig(self, request, context):
        result = json.dumps(Gath.query_sig(
            request.sig_path), indent=4)
        return gatherly_pb2.Sources(sources=result)
    
    def gather_fastx(self, request, context):
        result = json.dumps(Gath.query_fastx(
            request.fastx_path), indent=4)
        return gatherly_pb2.Sources(sources=result)


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    gatherly_pb2_grpc.add_GatherlyServicer_to_server(Gatherly(), server)
    server.add_insecure_port('[::]:50000')
    server.start()
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig()
    parser = argparse.ArgumentParser()
    parser.add_argument('--index', type=str, required=True, help="index_prefix")
    parser.add_argument('-k', type=int, required=True, help="kSize")    
    args = parser.parse_args()
    index_prefix = args.index
    kSize = args.k
    print("initializing the server", file = sys.stderr)
    Gath = gatherly.SplittedIndex(index_prefix, kSize)
    print("Loading all parts of the index", file = sys.stderr)
    now = time.time()
    Gath.load_all_parts()
    print(f"Loaded all parts in {time.time() - now} seconds")
    print("server is ready.")
    serve()
