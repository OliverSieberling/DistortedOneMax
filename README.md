This repository contains the code for conducting the experiments and generating the figures in the paper 
- **Plus Strategies are Exponentially Slower for Planted Optima of Random Height**.
  
# Figure 2
- output_files: Contains the output of OneRun_1e9.cpp in two seperate files for $(1+\lambda)$-EA and $(1,\lambda)$-EA.
- OneRun_1e9.cpp: Simulates one run of $(1+\lambda)$-EA and $(1,\lambda)$-EA on distorted OneMax with distortions sampled from an exponential distribution. Outputs how the total fitness, OneMax fitness and distortion evolve for both simulated runs.
- OneRun_1e9.ipynb: Jupyter notebook to generate figure 2 using the files in output_files.
- OneRun_1e9.pgf: Figure produced by OneRun_1e9.ipynb.

# Figure 3
- output_files: Contains the output of MedianSimulations_exp_unif.cpp for exponential and uniform distribution.
- MedianSimulations_exp_unif.cpp: Simulates multiples runs of $(1+\lambda)$-EA and $(1,\lambda)$-EA on distorted OneMax. The distortions are sampled from a distribution defined in line 34. Outputs the number of generations required for each simulated run.
- MedianSimulations_exp_unif.ipynb: Jupyter notebook to generate figure 3 using the files in output_files.
- MedianSimulations_exp_unif.pgf: Figure produced by MedianSimulations_exp_unif.ipynb.

# Figure 4
- output_files: Contains the output of DifferentP_exp.cpp for different truncated exponential distributions.
- DifferentP_exp.cpp: Simulates multiples runs $(1+\lambda)$-EA on distorted OneMax for different distortion probabilities $p$. The distortions are sampled from a truncated exponential distribution, where the truncation is defined in line 36. Outputs the number of generations required for each simulated run.
- DifferentP_exp.ipynb: Jupyter notebook to generate figure 4 using the files in output_files.
- DifferentP_exp.pgf: Figure produced by DifferentP_exp.ipynb.

# Contact
Oliver Sieberling - osieberling@ethz.ch
