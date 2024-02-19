import struct
import zlib

import hashlib

import config
import decode_pack


def write_header(pack_file, num_objects):
    signature = b'PACK'
    version = struct.pack('>I', 2)
    num_objects = struct.pack('>I', num_objects)

    pack_file.write(signature)
    pack_file.write(version)
    pack_file.write(num_objects)

def write_object_header(pack_file, obj_type, obj_size):
    if type(obj_type) != int:
        obj_type = config.type_codes[obj_type]

    type_mask = obj_type << 4
    size_mask = obj_size & 15

    current_byte = type_mask | size_mask
    obj_size = obj_size >> 4
    while obj_size:
        current_byte |= 1 << 7
        pack_file.write(struct.pack('>B', current_byte))
        current_byte = obj_size & 127
        obj_size = obj_size >> 7
    pack_file.write(struct.pack('>B', current_byte))


def write_sha1(pack_file):
    pack_file.seek(0)
    sha1 = hashlib.sha1()
    written_data = pack_file.read()
    sha1.update(written_data)
    pack_file.write(sha1.digest())


def write_pack_file(file_path, objects):
    with open(file_path, 'wb+') as pack_file:
        write_header(pack_file, len(objects))

        for obj_type, obj_data in objects:
            write_object_header(pack_file, obj_type, len(obj_data))
            pack_file.write(zlib.compress(obj_data))

        write_sha1(pack_file)


def copy_pack(old_path, new_path):
    objects = decode_pack.parse_pack_file(old_path)
    write_pack_file(new_path, objects)

def compare_files(path1, path2):
    with open(pack_file_path, 'rb') as f1, open(new_path, 'rb') as f2:
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
    print('they\'re the same...')


if __name__ == '__main__':
    import config

    pack_file_path = config.get_pack(config.get_pack_hash(0))
    idx_file_path = config.get_idx(config.get_pack_hash(0))

    new_pack_path = 'data/new_pack.pack'
    new_idx_path = 'data/new_pack.idx'

    copy_pack(pack_file_path, new_pack_path)
    compare_files(pack_file_path, new_pack_path)

    create_index_from_pack(new_path, new_idx_path)
    compare_files(idx_file_path, new_idx_path)

