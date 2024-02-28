import sys

from hash_utils import gen_hash


def run_linear(first_commit_hash, num_unique, all_commits=None, num_commits=None, num_to_generate=None):
  if not num_to_generate:
    num_to_generate = 16**num_unique
  if all_commits or num_commits:
    write_mode = 'a'
    assert all_commits
    assert num_commits
  else:
    write_mode = 'w'
    num_commits = 1
    all_commits = [False for _ in range(16**num_unique)]
    all_commits[int(first_commit_hash[:num_unique], 16)] = True

  parent_hash = first_commit_hash
  commit_hash = parent_hash

  with open('commits', write_mode) as commit_file:
    while num_commits != num_to_generate:
      salt = 0
      while all_commits[int(commit_hash[:num_unique], 16)] == True:
        salt += 1
        commit_hash = gen_hash(parent_hash, str(salt))
      num_commits += 1
      all_commits[int(commit_hash[:num_unique], 16)] = True
      commit_file.write(parent_hash + ' ' + str(salt) + '\n')
      parent_hash = commit_hash
  return all_commits, num_commits, parent_hash


def main():
  first_commit_hash = 'f9a17849fe28dff34647f698a392be2a9ce3617b'
  num_unique = int(sys.argv[1])

  # run_linear(first_commit_hash, num_unique)

  all_commits, num_commits, commit_hash = run_linear(
    first_commit_hash, num_unique, num_to_generate=10)
  all_commits, num_commits, commit_hash = run_linear(
    commit_hash, num_unique, all_commits=all_commits, num_commits=num_commits)
  with open('commits', 'a') as commit_file:
    commit_file.write(commit_hash + '\n')


if __name__ == '__main__':
  main()