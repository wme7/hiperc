# Makefile for HiPerC code

all: cpu_diffusion gpu_diffusion
.PHONY: all

hiperc-diffusion-flowchart.png: hiperc-diffusion-flowchart.dot
	dot -T png hiperc-diffusion-flowchart.dot -o hiperc-diffusion-flowchart.png

cpu_diffusion_list := cpu-serial-diffusion \
                      cpu-openmp-diffusion \
                      cpu-tbb-diffusion

gpu_diffusion_list := gpu-cuda-diffusion \
                      gpu-openacc-diffusion \
                      gpu-opencl-diffusion

phi_diffusion_list := phi-openmp-diffusion

.PHONY: run
run: run_cpu_diffusion run_gpu_diffusion

.PHONY: cpu_diffusion
cpu_diffusion:
	@$(foreach dir, $(cpu_diffusion_list), $(MAKE) -C $(dir);)

.PHONY: gpu_diffusion
gpu_diffusion:
	@$(foreach dir, $(gpu_diffusion_list), $(MAKE) -C $(dir);)

.PHONY: run_cpu_diffusion
run_cpu_diffusion:
	@$(foreach dir, $(cpu_diffusion_list), $(MAKE) -C $(dir) run;)

.PHONY: run_gpu_diffusion
run_gpu_diffusion:
	@$(foreach dir, $(gpu_diffusion_list), $(MAKE) -C $(dir) run;)

.PHONY: clean
clean:
	@$(foreach dir, $(cpu_diffusion_list), $(MAKE) -C $(dir) clean;)
	@$(foreach dir, $(gpu_diffusion_list), $(MAKE) -C $(dir) clean;)
	$(MAKE) -C doc clean

.PHONY: cleanall
cleanall:
	@$(foreach dir, $(cpu_diffusion_list), $(MAKE) -C $(dir) cleanall;)
	@$(foreach dir, $(gpu_diffusion_list), $(MAKE) -C $(dir) cleanall;)
