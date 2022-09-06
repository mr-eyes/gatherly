# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""The Python implementation of the GRPC helloworld.Greeter client."""
import gatherly
import grpc
import gatherly_pb2
import gatherly_pb2_grpc
from concurrent import futures
import logging
import sys
import argparse


def run(file_path, file_type, kSize):
    # kmer hash
    # with grpc.insecure_channel('localhost:50000') as channel:
    #     stub = gatherly_pb2_grpc.GatherlyStub(channel)
    #     response = stub.gather_kmer(gatherly_pb2.KmerHash(kmer_hash=20236993234229))
    # print("Kmer Sources: " + response.sources)

    with grpc.insecure_channel('localhost:50000') as channel:
        stub = gatherly_pb2_grpc.GatherlyStub(channel)
        if file_type == "smash":
            response = stub.gather_sig(
                gatherly_pb2.sourmashSig(sig_path=file_path, kSize=kSize))
            print(response.sources)
        elif file_type == "fastx":
            response = stub.gather_fastx(
                gatherly_pb2.FASTXFile(fastx_path=file_path, kSize=kSize))
            print(response.sources)
        else:
            print("something went wrong!")



if __name__ == '__main__':
    logging.basicConfig()
    parser = argparse.ArgumentParser()
    parser.add_argument('--fastx', type=str, required=False,
                        help="FASTA/Q file path")
    parser.add_argument('--sig', type=str, required=False,
                        help="sourmash sig file path")
    parser.add_argument('-k', type=int, required=True,
                        help="kSize")
    args = parser.parse_args()
    sourmash_sig_path = args.sig
    fastx_path = args.fastx
    kSize = args.k

    file_type = ""
    file_path = ""
    if(sourmash_sig_path):
        file_type = "smash"
        file_path = sourmash_sig_path
    elif(fastx_path):
        file_type = "fastx"
        file_path = fastx_path
    else:
        sys.exit("no files are being passed.")
        
    run(file_path, file_type, kSize)
