import struct

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


def summarize_idx(idx_file_path):
    sha1_hashes, offsets = read_idx_file(idx_file_path)
    for sha1, offset in zip(sha1_hashes, offsets):
        print(f"SHA-1: {sha1.hex()}, Offset: {offset}")


if __name__ == '__main__':
    import config
    idx_file_path = config.get_idx(config.get_pack_hash(0))
    summarize_idx(idx_file_path)
