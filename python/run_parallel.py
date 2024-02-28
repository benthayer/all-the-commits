import asyncio
import concurrent.futures
import time
import random
import sys

from hash_utils import gen_hash

def wrap_gen_hash(parent, salt):
  return salt, gen_hash(parent, str(salt))

# Asynchronous wrapper to run the blocking function in a thread
async def run_sha1_async(executor, parent, salt):
  loop = asyncio.get_running_loop()
  return await loop.run_in_executor(executor, wrap_gen_hash, parent, salt)

async def run_iteration(executor, workerPoolSize, all_commits, num_unique, parent):
  tasks = [run_sha1_async(executor, parent, salt+1) for salt in range(workerPoolSize)]
  salt = workerPoolSize+1

  found_solution = False
  while True:
    done, pending = await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
    
    for task in done:
      salt, new_hash = task.result()
      index = int(new_hash[:num_unique], 16)
      if not all_commits[index]:
        all_commits[index] = True
        return salt, new_hash

    tasks = pending

    for i in range(len(done)):
      tasks.add(run_sha1_async(executor, parent, salt))
      salt += 1

# Main coroutine that runs all tasks and waits for any to complete
async def run_parallel(first_commit_hash, num_unique):
  all_commits = [False for _ in range(16**num_unique)]
  parent_hash = first_commit_hash
  commit_hash = parent_hash
  num_commits = 1
  all_commits[int(first_commit_hash[:num_unique], 16)] = True

  workerPoolSize = 10
  with concurrent.futures.ThreadPoolExecutor(max_workers=workerPoolSize) as executor, \
       open('commits', 'w') as commit_file:
    while num_commits < 16**num_unique:
      salt, new_hash = await run_iteration(executor, workerPoolSize, all_commits, num_unique, parent_hash)

      num_commits += 1
      commit_file.write(parent_hash + ' ' + str(salt) + '\n')
      parent_hash = new_hash

    

async def main():
  first_commit_hash = 'f9a17849fe28dff34647f698a392be2a9ce3617b'
  num_unique = int(sys.argv[1])

  await run_parallel(first_commit_hash, num_unique)


if __name__ == '__main__':
  asyncio.run(main())
