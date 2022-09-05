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


def run():
    with grpc.insecure_channel('localhost:50000') as channel:
        stub = gatherly_pb2_grpc.GatherlyStub(channel)
        response = stub.gather_kmer(gatherly_pb2.KmerHash(kmer_hash=20236993234229))
    print("Kmer Sources: " + response.sources)


if __name__ == '__main__':
    logging.basicConfig()
    run()
