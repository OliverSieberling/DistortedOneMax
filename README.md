This repository contains the code for conducting the experiments and generating the figures in the paper *Plus Strategies are Exponentially Slower for Planted Optima of Random Height*.
  
# FigureOneRun
- .txt files: Output files of OneRun_1e9.cpp for the $(1+\lambda)$-EA and the $(1,\lambda)$-EA.
- OneRun_1e9.cpp: Simulates one run of $(1+\lambda)$-EA and $(1,\lambda)$-EA on distorted OneMax with distortions sampled from an exponential distribution. Outputs how the total fitness, OneMax fitness and distortion evolve for both simulated runs.
- OneRun_1e9.ipynb: Jupyter notebook to generate the figure based on the .txt files.
- OneRun_1e9.pdf: Figure produced by OneRun_1e9.ipynb.

# FigureExpUnifDistortions
- .txt files: Output files of MedianSimulations_expUnifDistortions.cpp for distortions from exponential and uniform distribution.
- MedianSimulations_expUnifDistortions.cpp: Simulates multiples runs of $(1+\lambda)$-EA and $(1,\lambda)$-EA on distorted OneMax. The distortions are sampled from a distribution defined in line 30. Outputs the number of generations required for each simulated run.
- MedianSimulations_expUnifDistortions.ipynb: Jupyter notebook to generate the figure based on the .txt files.
- MedianSimulations_expUnifDistortions.pdf: Figure produced by MedianSimulations_expUnifDistortions.ipynb.

# FigurePowerLawMutation
- .txt files: Output files of MedianSimulations_powerLawMutation.cpp for number of mutations sampled from a power law distribution. 
- MedianSimulations_powerLawMutation.cpp: Simulates multiples runs of $(1+\lambda)$-EA and $(1,\lambda)$-EA on distorted OneMax. The number of mutations is sampled from a power law distribution distribution with parameter $\beta$ define in line 17. The distortions are always sampled from an exponential distribution here. Outputs the number of generations required for each simulated run.
- MedianSimulations_powerLawMutation.ipynb: Jupyter notebook to generate the figure based on the .txt files. Additionally, an output file from FigureExpUnifDistortions is used for runs where the number of mutations is sampled from a binomial distribution.
- MedianSimulations_powerLawMutation.pdf: Figure produced by MedianSimulations_powerLawMutation.ipynb.

# FigureDifferentP
- .txt files: Contains the output of DifferentP_exp.cpp for different truncated exponential distributions.
- DifferentP_exp.cpp: Simulates multiples runs $(1+\lambda)$-EA on distorted OneMax for different distortion probabilities $p$. The distortions are sampled from a truncated exponential distribution, where the truncation is defined in the main function. Outputs the number of generations required for each simulated run.
- DifferentP_exp.ipynb: Jupyter notebook to generate the figure based on the output files.
- DifferentP_exp.pdf: Figure produced by DifferentP_exp.ipynb.

# Contact
Oliver Sieberling - osieberl@mit.edu
