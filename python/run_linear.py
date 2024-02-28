import sys

from hash_utils import gen_hash

first_commit_hash = 'f9a17849fe28dff34647f698a392be2a9ce3617b'

num_unique = int(sys.argv[1])
all_commits = set()
all_commits.add(first_commit_hash[:num_unique])

parent_hash = first_commit_hash
commit_hash = parent_hash

with open('commits', 'w') as commit_file:
    while len(all_commits) != 16**num_unique:
        salt = 0
        while commit_hash[:num_unique] in all_commits:
            salt += 1
            commit_hash = gen_hash(parent_hash, str(salt))
        all_commits.add(commit_hash[:num_unique])
        parent_hash = commit_hash
        commit_file.write(parent_hash + ' ' + str(salt) + '\n')