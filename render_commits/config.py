GIT_DIR = '../.git/'
OBJ_DIR = GIT_DIR + 'objects/'
PACK_DIR = OBJ_DIR + 'pack/'


packs = [
    '3df1506eca64b35ed8ff7def3b990ff6cdd968ae',
    '06aabca37f547a4e06a6166446a3087678999659',
]

def get_pack_hash(i):
    return packs[i]

def get_idx(pack_hash):
    return PACK_DIR + f'pack-{pack_hash}.idx'

def get_pack(pack_hash):
    return PACK_DIR + f'pack-{pack_hash}.pack'

def get_pack_and_idx(pack_hash):
    return get_pack(pack_hash), get_idx(pack_hash)


type_codes = {
    1: 'commit',
    2: 'tree',
    3: 'blob',
    'commit': 1,
    'tree': 2,
    'blob': 3,
}
