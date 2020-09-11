from hashlib import sha1
import sys

tree = "tree 6a0165c2aea6cfc5fba01029ede7a8da6c85f6f6"
author = "author Ben Thayer <ben@benthayer.com> 1599715620 -0500"
committer = "committer Ben Thayer <ben@benthayer.com> 1599715620 -0500"
message = "Initial commit"

def gen_hash(parent, salt):
    parent = 'parent ' + parent

    commit = '\n'.join([
        tree,
        parent,
        author,
        committer,
        '',
        message,
        '',
        salt,
        ''
    ])

    prefix = 'commit ' + str(len(commit)) + '\x00'
    obj = prefix + commit
    obj = obj.encode('utf-8')
    sha = sha1(obj).hexdigest()
    return sha

first_commit_hash = 'f9a17849fe28dff34647f698a392be2a9ce3617b'
print(first_commit_hash)
num_unique = int(sys.argv[1])
all_commits = set()
parent_hash = first_commit_hash
commit_hash = parent_hash
all_commits.add(parent_hash[:num_unique])

while len(all_commits) != 16**num_unique:
    salt = 0
    while commit_hash[:num_unique] in all_commits:
        salt += 1
        commit_hash = gen_hash(parent_hash, str(salt))
    print(commit_hash, salt)
    all_commits.add(commit_hash[:num_unique])
    parent_hash = commit_hash
