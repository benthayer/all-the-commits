from hashlib import sha1

tree = "tree 6a0165c2aea6cfc5fba01029ede7a8da6c85f6f6"
author = "author Ben Thayer <ben@benthayer.com> 1599715620 -0500"
committer = "committer Ben Thayer <ben@benthayer.com> 1599715620 -0500"
message = "Initial commit"

def gen_commit_obj(parent, salt):
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
    return obj

def gen_hash(parent, salt):
    obj = gen_commit_obj(parent, salt)
    sha = sha1(obj).hexdigest()
    return sha
