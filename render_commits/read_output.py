import struct
def read_output(file_path):
    with open(file_path, 'rb') as commit_file:
        x = commit_file.read()

    # Structure:
    # hash (20)
    # salt (8)

    num_commits = len(x) - 20) // 28

    parent = x[:20].hex()

    for i in range(num_commits):
        print(struct.unpack('>Q', x[48:56])[0])
        print(x[28: 48].hex())

    print(x[:20].hex())
    print(struct.unpack('>Q', x[20:28])[0])



    print(x)

if __name__ == '__main__':
    read_output('output.bin')
