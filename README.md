Just make and run ./reversy, and check output.txt.

This solution just creates 8 threads, which then block on reading, 
reversing, and writing a line. This is obviously not going to get
much benefit from the threads.

If the strings are fixed-length rather than delimited, we could
actually read and write in parallel. Presumably, like a parallel
grep tool, we'd want to read or mmap large chunks (say, 4MB?)
and parallelize on chunks of lines rather than lines.

If not, it makes more sense to do the reading and writing in a
single thread, dispatch just the reversing to worker threads,
and aggregate the results back in the main thread. That would
also allow us to decide whether the outputs have to
be in order (tag each input with an incrementing key, and keep
the outputs by key, and the main thread blocks until the
next key it wants is available). If we were doing this in, say,
Python or Java, that's just calling the ordered vs. unordered
map function on an executor, and even batching the tasks would
be automated. But C++17 doesn't have any of that in the stdlib,
and building one, or even finding an open source futures and
executors library that offers one, would be massive overkill for
this project. So it would be simpler to do the dispatching and
aggregating manually around a queue with a condition (or maybe
a lock-free queue, but that's probably not going to be helpful
here) on each end.

At any rate, because I hadn't heard back on clarifications for
any of these questions, I just built the simplest thing, which
is a bit low-level, but has the advantage of being very easy
to change if we want to go a different way.
