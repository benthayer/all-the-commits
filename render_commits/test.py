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

example_tag = b'tree 496d6428b9cf92981dc9495211e6e1120fb6f2ba\nauthor Ben Thayer <ben@benthayer.com> 1710803887 -0600\ncommitter Ben Thayer <ben@benthayer.com> 1710803887 -0600\n\na\n'
# 2b42af579bc1f11eb139801b9a7d14ec59ce1f3e

def test_pack_tag():
    # Warning: This is for annotated tags only.
    # You still need /refs/tags/tagname to reference the commit or annotated tag hash
    objects = [
        ('tag', example_tag)
    ]
    create_pack_and_index('data', objects)


if __name__ == '__main__':
    test_pack_tag()
