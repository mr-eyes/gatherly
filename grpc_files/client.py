import gatherly
import grpc
import gatherly_pb2
import gatherly_pb2_grpc
from concurrent import futures
import logging
import sys
import argparse


def run(file_path, file_type, kSize):
    with grpc.insecure_channel('localhost:50000', options=[('grpc.max_receive_message_length', 1000 * 1024 * 1024)]) as channel:
        stub = gatherly_pb2_grpc.GatherlyStub(channel)
        if file_type == "smash":
            response = stub.gather_sig(
                gatherly_pb2.sourmashSig(sig_path=file_path, kSize=kSize))
            print(response.sources)
        else:
            print("something went wrong!")


if __name__ == '__main__':
    logging.basicConfig()
    parser = argparse.ArgumentParser()
    parser.add_argument('--sig', type=str, required=False,
                        help="sourmash sig file path")
    parser.add_argument('-k', type=int, required=True,
                        help="kSize")
    args = parser.parse_args()
    sourmash_sig_path = args.sig
    kSize = args.k

    if(sourmash_sig_path):
        file_type = "smash"
        file_path = sourmash_sig_path
    else:
        sys.exit("no files are being passed.")

    run(file_path, file_type, kSize)
