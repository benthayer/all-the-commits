import os

import struct
import zlib

import hashlib

import config
import decode_pack
import decode_idx


def write_header(pack_file, num_objects):
    signature = b'PACK'
    version = struct.pack('>I', 2)
    num_objects = struct.pack('>I', num_objects)
    header = signature + version + num_objects

    pack_file.write(header)
    return header


def get_object_header(obj_type, obj_size):
    if type(obj_type) != int:
        obj_type = config.type_codes[obj_type]

    type_mask = obj_type << 4
    size_mask = obj_size & 15

    current_byte = type_mask | size_mask
    obj_size = obj_size >> 4

    object_header = b''
    while obj_size:
        current_byte |= 1 << 7
        object_header += struct.pack('>B', current_byte)
        current_byte = obj_size & 127
        obj_size = obj_size >> 7
    object_header += struct.pack('>B', current_byte)

    return object_header


def write_sha1(pack_file):
    pack_file.seek(0)
    sha1 = hashlib.sha1()
    written_data = pack_file.read()
    sha1.update(written_data)
    pack_file.write(sha1.digest())
    return sha1.digest()


def get_object_sha1(obj_type, obj_data):
    sha1 = hashlib.sha1()
    prefix = obj_type + ' ' + str(len(obj_data))
    hash_string = prefix.encode() + b'\0' + obj_data
    sha1.update(hash_string)
    return sha1.digest()


def write_pack_file(output_dir, objects):
    # Init metadata
    metadata = {}

    # Create temporary name
    file_path = os.path.join(output_dir, 'pack.pack.tmp')

    with open(file_path, 'wb+') as pack_file:
        write_header(pack_file, len(objects))

        for obj_type, obj_data in objects:
            object_header = get_object_header(obj_type, len(obj_data))
            compressed_data = zlib.compress(obj_data)

            data_to_write = object_header + compressed_data
            sha1 = get_object_sha1(obj_type, obj_data)
            crc32 = struct.pack('>I', zlib.crc32(data_to_write))

            metadata[sha1] = (crc32, pack_file.tell())

            pack_file.write(data_to_write)

        sha1 = write_sha1(pack_file)
        print(f'Created pack file with sha1: {sha1.hex()}')

    os.rename(file_path, os.path.join(output_dir, f'pack-{sha1}.pack'))
    return metadata, sha1


def copy_pack(old_path, output_dir):
    objects = decode_pack.parse_pack_file(old_path)
    return write_pack_file(output_dir, objects)


def create_index(output_dir, pack_metadata, pack_sha1):
    # Prepare data
    sorted_hashes = sorted(pack_metadata.keys())  # Sort hashes for fanout table and entries
    num_objects = len(pack_metadata)

    idx_file_path = os.path.join(output_dir, f'pack-{pack_sha1}.idx')

    with open(idx_file_path, 'wb+') as idx_file:
        # Write header
        idx_file.write(b'\xfftOc')  # Magic number
        idx_file.write(struct.pack('>I', 2))  # Version 2

        # Write fanout table
        fanout = [0] * 256
        for hash in sorted_hashes:
            fanout[hash[0]] += 1
        for i in range(1, 256):
            fanout[i] += fanout[i - 1]
        for count in fanout:
            idx_file.write(struct.pack('>I', count))

        # Write hashes
        for hash in sorted_hashes:
            idx_file.write(hash)

        # Write CRC32 checksums
        for hash in sorted_hashes:
            crc32 = pack_metadata[hash][0]
            idx_file.write(crc32)

        # Init large offsets list
        large_offsets = []

        # Write 32-bit offsets
        for hash in sorted_hashes:
            offset = pack_metadata[hash][1]
            if offset < 2**31:
                idx_file.write(struct.pack('>I', offset))
            else:
                # Mark this as a large offset
                idx_file.write(struct.pack('>I', 0x80000000 | len(large_offsets)))
                large_offsets.append(offset)

        # Write large offset table (if necessary)
        if large_offsets:
            for offset in large_offsets:
                idx_file.write(struct.pack('>Q', offset))

        # Write packfile checksum (placeholder, replace with actual packfile hash)
        idx_file.write(pack_sha1)

        # Compute and write index file checksum
        idx_file.seek(0)
        index_content = idx_file.read()
        index_sha1 = hashlib.sha1(index_content).digest()
        idx_file.write(index_sha1)


def create_pack_and_index(output_dir, objects):
    pack_metadata, pack_sha1 = write_pack_file(output_dir, objects)
    create_index(output_dir, pack_metadata, pack_sha1)


def compare_files(path1, path2):
    with open(path1, 'rb') as f1, open(path2, 'rb') as f2:
        c1 = f1.read()
        c2 = f2.read()

    # Test 1: File Header + obj header
    assert c1[:5] == c2[:5]

    # Test 2: Contents are the same
    i = 0
    while i < min(len(c1), len(c2)):
        try:
            assert c1[:i] == c2[:i]
            i += 1
        except:
            print(i)
            break

    # test 3: Lengths are equal
    assert len(c1) == len(c2)
    print('pack files match')


if __name__ == '__main__':
    import config

    pack_file_path = config.get_pack(config.get_pack_hash(0))
    idx_file_path = config.get_idx(config.get_pack_hash(0))

    # TODO Get name from pack_sha1
    new_pack_path = 'data/new_pack.pack'
    new_idx_path = 'data/new_pack.idx'

    pack_metadata, pack_sha1 = copy_pack(pack_file_path, 'data')
    compare_files(pack_file_path, new_pack_path)
    print(pack_metadata)

    decode_idx.summarize_idx(idx_file_path)
    create_index(new_idx_path, pack_metadata, pack_sha1)
    compare_files(idx_file_path, new_idx_path)

