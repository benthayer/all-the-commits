import asyncio
import concurrent.futures
import time

# Dummy function that simulates varying execution times
def do_some_work(x):
  time.sleep(x)
  print("")
  return f'Completed work {x}'

# Asynchronous wrapper to run the blocking function in a thread
async def run_blocking_task(executor, x):
  loop = asyncio.get_running_loop()
  return await loop.run_in_executor(executor, do_some_work, x)

# Main coroutine that runs all tasks and waits for any to complete
async def main():
  with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
    tasks = [run_blocking_task(executor, i) for i in range(1, 11)]
    done, pending = await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
    
    # Cancel all other tasks that are pending after the first one completes
    for task in pending:
        task.cancel()
    
    # Get the result of the first completed task
    for task in done:
        print(task.result())
        break  # Since we only care about the first result in this example
    print("Event loop done")
    print("Closed")

# Run the main coroutine
asyncio.run(main())
