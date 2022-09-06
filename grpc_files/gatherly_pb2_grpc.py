# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

import gatherly_pb2 as gatherly__pb2


class GatherlyStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.gather_kmer = channel.unary_unary(
                '/gatherly.Gatherly/gather_kmer',
                request_serializer=gatherly__pb2.KmerHash.SerializeToString,
                response_deserializer=gatherly__pb2.KmerSources.FromString,
                )
        self.gather_sig = channel.unary_unary(
                '/gatherly.Gatherly/gather_sig',
                request_serializer=gatherly__pb2.sourmashSig.SerializeToString,
                response_deserializer=gatherly__pb2.Sources.FromString,
                )
        self.gather_fastx = channel.unary_unary(
                '/gatherly.Gatherly/gather_fastx',
                request_serializer=gatherly__pb2.FASTXFile.SerializeToString,
                response_deserializer=gatherly__pb2.Sources.FromString,
                )


class GatherlyServicer(object):
    """Missing associated documentation comment in .proto file."""

    def gather_kmer(self, request, context):
        """query a single hashed_kmer
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def gather_sig(self, request, context):
        """query a full sourmash sig (gzipped or not)
        TODO use stream here
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def gather_fastx(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_GatherlyServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'gather_kmer': grpc.unary_unary_rpc_method_handler(
                    servicer.gather_kmer,
                    request_deserializer=gatherly__pb2.KmerHash.FromString,
                    response_serializer=gatherly__pb2.KmerSources.SerializeToString,
            ),
            'gather_sig': grpc.unary_unary_rpc_method_handler(
                    servicer.gather_sig,
                    request_deserializer=gatherly__pb2.sourmashSig.FromString,
                    response_serializer=gatherly__pb2.Sources.SerializeToString,
            ),
            'gather_fastx': grpc.unary_unary_rpc_method_handler(
                    servicer.gather_fastx,
                    request_deserializer=gatherly__pb2.FASTXFile.FromString,
                    response_serializer=gatherly__pb2.Sources.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'gatherly.Gatherly', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class Gatherly(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def gather_kmer(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/gatherly.Gatherly/gather_kmer',
            gatherly__pb2.KmerHash.SerializeToString,
            gatherly__pb2.KmerSources.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def gather_sig(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/gatherly.Gatherly/gather_sig',
            gatherly__pb2.sourmashSig.SerializeToString,
            gatherly__pb2.Sources.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def gather_fastx(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/gatherly.Gatherly/gather_fastx',
            gatherly__pb2.FASTXFile.SerializeToString,
            gatherly__pb2.Sources.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
