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
import socket


class Gatherly(gatherly_pb2_grpc.GatherlyServicer):
    def gather_kmer(self, request, context):
        result = ','.join(Gath.get_sources_from_hash(request.kmer_hash))
        return gatherly_pb2.KmerSources(sources=result)

    def gather_sig(self, request, context):
        print(f"received a sourmash sig query", file = sys.stderr)
        result = json.dumps(Gath.query_sig(
            request.sig_path), indent=4)
        return gatherly_pb2.Sources(sources=result)


def serve(IP_ADDRESS, PORT):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    gatherly_pb2_grpc.add_GatherlyServicer_to_server(Gatherly(), server)
    server.add_insecure_port(f'{IP_ADDRESS}:{PORT}')
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
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    IP_ADDRESS = s.getsockname()[0]
    s.close()
    PORT = 50000
    print(f"Hosting at {IP_ADDRESS}::{PORT}", file = sys.stderr)
    serve("0.0.0.0", PORT)
