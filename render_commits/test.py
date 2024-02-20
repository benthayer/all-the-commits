import os
import zlib

from write_pack import create_pack_and_index

def generate_objects():
    # Goal is to have a large offset for the second object
    # So, we make the first object large
    # Second object can be any size

    # Object 1 data should be larger than 2^31 to use large offset table
    obj1 = 'blob', os.urandom(2**31 * 3)
    obj2 = 'blob', b'Hello :)'
    return [obj1, obj2]


def test_custom_objects():
    objects = [
        ('blob', b'Hello!'),
        ('blob', b'Hello World!')
    ]
    create_pack_and_index('data', objects)

def test_large_data():
    data = os.urandom(2**31 * 3)
    print('Randomness generated')
    compressed = zlib.compress(data)
    assert len(compressed) >= 2**32


def test_large_pack():
    create_pack_and_index('data', generate_objects())

from decode_idx import summarize_idx
import config

def test_new_idx():
    idx_path = '../.git/objects/pack/pack-74909342a2e77e20efe61901d9e9eeb3d37c916d.idx'
    summarize_idx(idx_path)


if __name__ == '__main__':
    test_custom_objects()
