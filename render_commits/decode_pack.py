import struct
import zlib


def read_pack_header(pack_file):
    with open(pack_file, 'rb') as f:
        # Read the 4-byte signature ('PACK')
        signature = f.read(4)
        if signature != b'PACK':
            raise ValueError("This is not a valid pack file.")

        # Read the 4-byte version number (big-endian)
        version = struct.unpack('>I', f.read(4))[0]

        # Read the 4-byte number of objects (big-endian)
        num_objects = struct.unpack('>I', f.read(4))[0]

        return version, num_objects

def read_object_header(pack_file):
    byte = ord(pack_file.read(1))
    obj_type = (byte >> 4) & 7  # Object type is in the higher bits
    size = byte & 15  # Initial size is in the lower bits

    shift = 4
    while byte & 0x80:  # Continue if the high bit is set
        byte = ord(pack_file.read(1))
        size |= (byte & 0x7f) << shift
        shift += 7

    return obj_type, size

import zlib

def parse_pack_file(pack_file_path):
    version, num_objects = read_pack_header(pack_file_path)
    print(f"Pack file version: {version}")
    print(f"Number of objects: {num_objects}")

    with open(pack_file_path, 'rb') as pack_file:
        pack_file.seek(0, 2)
        file_size = pack_file.tell()
        # Skip past the header (12 bytes)
        pack_file.seek(12)

        for i in range(num_objects):
            # Read the first byte of the object entry, which contains the type in the higher bits
            # Note: This is a simplified approach and might not correctly handle all cases
            print(file_size - pack_file.tell())
            obj_type, size = read_object_header(pack_file)
            print(f"\nObject {i+1}: Type {obj_type}, Size {size}")

            decompressor = zlib.decompressobj()
            decompressed_data = b''
            while not decompressor.eof:
                decompressed_data += decompressor.decompress(pack_file.read(1))

            if obj_type == 1:  # Commit
                print("Decompressed commit object data:", decompressed_data)
            elif obj_type == 2:  # Tree
                print("Decompressed tree object data:", decompressed_data)
            elif obj_type == 3:  # Blob
                print("Decompressed blob object data:", decompressed_data)
            else:
                print("Unknown or unsupported object type")

# Example usage
pack_file_path1 = '.git/objects/pack/pack-3df1506eca64b35ed8ff7def3b990ff6cdd968ae.pack'
pack_file_path2 = '.git/objects/pack/pack-06aabca37f547a4e06a6166446a3087678999659.pack'
parse_pack_file(pack_file_path1)
parse_pack_file(pack_file_path2)

