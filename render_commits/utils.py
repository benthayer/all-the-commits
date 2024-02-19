def compare(file1_path, file2_path):
    with open(file1_path, 'rb') as f1, open(file2_path, 'rb') as f2:
        c1 = f1.read()
        c2 = f2.read()
        return c1 == c2

if __name__ == '__main__':
    import config
    pack_hash = config.get_pack_hash(0)
    pack_file_path = config.get_pack(pack_hash)
    assert compare(pack_file_path, pack_file_path)
