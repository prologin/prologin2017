#!/usr/bin/env python3

import argparse
import gzip
import logging

from state import DumpReader
from server import Server

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)

    parser = argparse.ArgumentParser(description="Replay a stechec match from "
                                                 "a dump file")
    parser.add_argument('dump', metavar='dump-file', type=argparse.FileType('r'),
                        help="The dump file (plain JSON or gzipped JSON)")
    parser.add_argument('-t', '--tv', action='store_true', help="TV mode")

    args = parser.parse_args()

    if args.dump.name.endswith('.gz'):
        args.dump.close()
        dump = gzip.open(args.dump.name, 'rt')
    else:
        dump = args.dump

    reader = DumpReader(dump)
    server = Server(reader, tv_mode=args.tv)
    server.start()
