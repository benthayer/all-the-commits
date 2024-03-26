import struct
import zlib
import binascii

import config

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


def parse_pack_file(pack_file_path):
    version, num_objects = read_pack_header(pack_file_path)
    print(f"Reading pack file: {pack_file_path}")
    print(f"Pack file version: {version}")
    print(f"Number of objects: {num_objects}")

    objects = []

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

            if obj_type in [6, 7, 0]:
                print("Unknown or unsupported object type")
                continue

            decompressor = zlib.decompressobj()
            decompressed_data = b''
            while not decompressor.eof:
                decompressed_data += decompressor.decompress(pack_file.read(1))

            objects.append((config.type_codes[obj_type], decompressed_data))

            if obj_type == 1:  # Commit
                print("Decompressed commit object data:", decompressed_data)
            elif obj_type == 2:  # Tree
                print("Decompressed tree object data:", decompressed_data)
            elif obj_type == 3:  # Blob
                print("Decompressed blob object data:", decompressed_data)
            elif obj_type == 4:  # Tag
                print("Decompressed tag object data:", decompressed_data)
        print(binascii.hexlify(pack_file.read()))
    return objects


if __name__ == '__main__':
    import config
    pack_file_path = config.get_pack(config.get_pack_hash(2))
    # objects = parse_pack_file(pack_file_path)
    objects = parse_pack_file('.git/objects/pack/pack-988d40c6b1a43ce769cd5ab2065d162683579592.pack')
    print(objects)
