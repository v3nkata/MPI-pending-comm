# MPI-pending-comm
Provoke MPI pending communication between processes. A case study in MPI correctness.

`cancel_pattern.c` describes a pattern in MPI communication that causes pending messages that could cause issues in trace replay.

Compiling and execute the above code without instrumentation. For instance, one might do
```
gcc cancel_pattern.c -o cancel_pattern
mpirun -np 2 ./cancel_pattern

```
works without errors.

However, if you instrument the above code with Score-P,
```
scorep-gcc cancel_pattern.c -o cancel_pattern_scorep
scan -t mpirun -np 2 ./cancel_pattern_scorep

```
Only partial output is produced i.e only trace files are produced but parallel trace analyzer that usually does the post processing of traces to identify wait-states, critical path etc. hangs without producing the final `trace.cubex` file.

A sample output may look like this
![output](images/output_1.png)

Since the trace files are produced, one might visualize them using Vampir. The timeline for this code is as shown below
![timeline](images/trace_np_2.png)

The observed behaviour of pending messages can be viewed as warnings in Vampir
![warning](images/warning.png)
