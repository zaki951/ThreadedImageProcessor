# Multithreaded Image Processing (C++)

This small project is a practical exercise to learn and experiment with multithreading in C++.  
It implements a basic thread pool and uses it to process a set of images in parallel.  
The goal is mainly educational and focuses on comparing performance between:

- single-thread execution
- multithreaded execution using a fixed number of worker threads

---

## Objectives

- Implement a minimal thread pool  
- Process multiple images in parallel (CPU-only)  
- Measure and compare execution time vs. a single-thread version  
- Observe the speedup on a small real workload

---

## How it works

1. The program scans an input folder and creates one task per image.  
2. Tasks are pushed into a thread-safe queue.  
3. Worker threads pop tasks and run the image processing function.  
4. Results are written to an output folder.  
5. Execution time is measured for both single-thread and multithread modes.

---

## Example results (machine with 8 CPU threads)

These numbers are only indicative and will vary depending on the machine and the processing steps used:

| Mode          | Time   
|---------------|--------
| 8 threads     | 2,5 s   
| 1 thread      | 10 s


---

## Usage

``./build.sh && ./run.sh``

