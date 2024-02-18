import binascii

def decode_tree_object(data):
    i = 0
    entries = []
    while i < len(data):
        # Find the end of the mode/name segment (null byte separator)
        end_of_entry = data.index(b'\x00', i)

        # Extract mode and name
        mode_name = data[i:end_of_entry].decode('ascii')
        mode, name = mode_name.split(' ', 1)

        # SHA-1 hash starts immediately after the null byte
        sha1_start = end_of_entry + 1
        sha1_end = sha1_start + 20  # SHA-1 hash is 20 bytes long

        # Extract and convert SHA-1 hash to hexadecimal string
        sha1_hash = binascii.hexlify(data[sha1_start:sha1_end]).decode('ascii')

        entries.append({'mode': mode, 'name': name, 'sha1': sha1_hash})

        # Move to the next entry
        i = sha1_end

    return entries

import zlib

with open('.git/objects/68/21ba6db6140b28e42490d98354b9d6ec4badc4', 'rb') as f:
    compressed_data = f.read()
    data = zlib.decompress(compressed_data)
    first_00 = data.index(b'\x00')
    tree_data = data[first_00+1:]

    entries = decode_tree_object(tree_data)
    for entry in entries:
        print(entry)

