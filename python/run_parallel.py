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

async def run_iteration(executor, workerPoolSize, all_commits, num_commits, num_unique, parent):
  print("running iteration")
  num_task_target = min(len(all_commits) // (len(all_commits) - num_commits), 1000)
  num_pending_target = 1
  tasks = [run_sha1_async(executor, parent, salt+1) for salt in range(num_task_target)]
  salt = workerPoolSize+1

  found_solution = False
  while True:
    print(num_task_target)
    a = time.time_ns()
    done, pending = await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
    print(len(done))
    print(time.time_ns()-a)
    
    for task in done:
      salt, new_hash = task.result()
      index = int(new_hash[:num_unique], 16)
      if not all_commits[index]:
        all_commits[index] = True
        return salt, new_hash
    print(time.time_ns()-a)

    tasks = pending

    for i in range(len(pending), num_task_target):
      tasks.add(run_sha1_async(executor, parent, salt))
      salt += 1
    
    print(time.time_ns()-a)

async def run_parallel(first_commit_hash, num_unique, all_commits=None, num_commits=None, num_to_generate=None):
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

  workerPoolSize = 10
  with concurrent.futures.ThreadPoolExecutor(max_workers=workerPoolSize) as executor, \
       open('commits', write_mode) as commit_file:
    while num_commits < 16**num_unique:
      salt, new_hash = await run_iteration(executor, workerPoolSize, all_commits, num_commits, num_unique, parent_hash)

      num_commits += 1
      commit_file.write(parent_hash + ' ' + str(salt) + '\n')
      parent_hash = new_hash
  return all_commits, num_commits, parent_hash


async def main():
  first_commit_hash = 'f9a17849fe28dff34647f698a392be2a9ce3617b'
  num_unique = int(sys.argv[1])

  await run_parallel(first_commit_hash, num_unique)


if __name__ == '__main__':
  asyncio.run(main())
