import struct
import zlib

def write_idx_file():
    header 8
        magic 4 '\xfftOc'
        version 4 '2'
    fanout 256 * 4
    hashes num * 20
    chksum num * 4 => zlib.rc32()
    offset num * 4
    large offset table ?
    packfile checksum => hashlib.sha1(packfile)
    index checksum => hashlib.sha1(idxfile)



def read_idx_file(idx_file_path):
    with open(idx_file_path, 'rb') as f:
        # Skip header and fanout table
        f.seek(8 + 256 * 4)

        # Determine the number of objects (last entry in the fanout table)
        f.seek(8 + 255 * 4)
        num_objects = struct.unpack('>I', f.read(4))[0]

        # Read SHA-1 hashes
        sha1_hashes = [f.read(20) for _ in range(num_objects)]

        # Read CRC-32 checksums (not used in this example, but required to reach offsets)
        crc32_checksums = [f.read(4) for _ in range(num_objects)]

        # Read offsets
        offsets = [struct.unpack('>I', f.read(4))[0] for _ in range(num_objects)]

        return sha1_hashes, offsets

def read_idx_file(idx_file_path):
    sha1_hashes, offsets = read_idx_file(idx_file_path2)
    for sha1, offset in zip(sha1_hashes, offsets):
        print(f"SHA-1: {sha1.hex()}, Offset: {offset}")

def write_pack_file():
    header 12
        PACK 4 'PACK'
        version 4 '2'
        num_obj 4

    objects numobj * variable
        obj header (dynamic 1-indefinite)
            type 3 bits
            size variable
        obj data
            zlib.compressed variable



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
    print(f"Pack file version: {version}")
    print(f"Number of objects: {num_objects}")

    with open(pack_file_path, 'rb') as pack_file:
        pack_file.seek(0, 2)
        file_size = pack_file.tell()
        # Skip past the header (12 bytes)
        pack_file.seek(12)

        for i in range(num_objects):
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

