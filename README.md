# TCP-Benchmark
This repository is dedicated for TCP-throughput benchmarking

Report:
In the project there are three parts: server , client and common when server and client are as required and common helps to build an effective code. We will note that after researching different types of clocks we chose to use the “clock_gettime” method which is very accurate.
 
From the results we can see that there is a large increase in the throughput in the beginning as expected and then there is a convergence because the overhead is less effective. In addition it seems that the amount of samples does provide us more reliable information.
