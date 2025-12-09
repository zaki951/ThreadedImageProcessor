# CPU-bound Multithreading Benchmark

This program launches many CPU‑bound tasks (prime counting) concurrently to benchmark different threading strategies.  
It compares execution time for single-thread, `std::thread`, `std::async`, and a thread pool.  
Each task performs heavy integer computation (prime checking) to fully utilize the CPU.

## Build and run

```bash
./build.sh && ./main
```
