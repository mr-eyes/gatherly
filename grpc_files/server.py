import gatherly
import grpc
import gatherly_pb2
import gatherly_pb2_grpc
from concurrent import futures
import logging

Gath = gatherly.SplittedIndex("/home/mabuelanin/dib-dev/gatherly/splitted_index/ok")
Gath.load_all_parts()

class Gatherly(gatherly_pb2_grpc.GatherlyServicer):
    def gather_kmer(self, request, context):
        result = ','.join(Gath.get_sources_from_hash(request.kmer_hash))
        return gatherly_pb2.KmerSources(sources = result)
    
def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    gatherly_pb2_grpc.add_GatherlyServicer_to_server(Gatherly(), server)
    server.add_insecure_port('[::]:50000')
    server.start()
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig()
    serve()

