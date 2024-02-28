import asyncio
import sys

from run_linear import run_linear
from run_parallel import run_parallel

async def main():
  first_commit_hash = 'f9a17849fe28dff34647f698a392be2a9ce3617b'
  num_unique = int(sys.argv[1])

  all_commits, num_commits, commit_hash = run_linear(
    first_commit_hash, num_unique, num_to_generate=16**num_unique-200)
  all_commits, num_commits, commit_hash = await run_parallel( \
    commit_hash, num_unique, all_commits=all_commits, num_commits=num_commits)

  with open('commits', 'a') as commit_file:
    commit_file.write(commit_hash + '\n')


if __name__ == '__main__':
  asyncio.run(main())
