---
marp: true
#size: 4:3
style: |
    img[alt~="center"] {
      display: block;
      margin: 0 auto;
    }
---

# **Laboratory 14**
## Package managers & containerization in HPC  

### Paolo Joseph Baioni

### 23/05/2025


---
# Outline
- package managers and HPC
- introduction to spack
- containers & HPC: podman, apptainer
- hpccm

---
# Some Alternatives
 - OS package managers: dnf/yum (rpm), apt (deb) ...
 - python environments
 - EasyBuild
 - Spack

---
# Spack
Spack is a multi-platform package manager that builds and installs multiple versions and configurations of software. It works on Linux, macOS, Windows, and many supercomputers. Spack is non-destructive: installing a new version of a package does not break existing installations, so many configurations of the same package can coexist.

Spack offers a simple "spec" syntax that allows users to specify versions and configuration options. Package files are written in pure Python, and specs allow package authors to write a single script for many different builds of the same package. 

---
# Installing

Clone the repository
```
git clone -c feature.manyFiles=true --depth=1 --branch \
releases/v0.23 https://github.com/spack/spack spack-0.23
```

---
# First steps

setup the environment
```
source spack-0.23/share/spack/setup-env.sh
```

list available packages (slow first time)
```
spack list
```

optional: tune configuration
```
find spack-0.23 -iname "config.yaml"
nano -liST 2 spack-0.23/etc/spack/defaults/config.yaml 
```
(eg, edit spack-stage, stage, test and cache dirs; see spack.diff)

---
# Basic usage

```
spack info gcc
spack install gcc@14.2.0
which gcc
spack load gcc
which gcc 
spack unload --all
```

---
# Specs

Default ones
```
spack spec -ll gcc
```
Setting specs
```
$ spack install mpileaks                           :  unconstrained
$ spack install mpileaks@3.3                       @: custom version
$ spack install mpileaks@3.3 %gcc@4.7.3            %: custom compiler
$ spack install mpileaks@3.3 %gcc@4.7.3 +threads   +/- build option
$ spack install mpileaks@3.3 cppflags="-O3 –g3"        set compiler flags
$ spack install mpileaks@3.3 target=cascadelakeset     target microarchitecture
$ spack install mpileaks@3.3 ^mpich@3.2 %gcc@4.9.3 ^: dependency constraints
```

---
# Compilers & toolchains

Get compilers list: `spack compilers` or
```
spack config get compilers
```
equivalently
```
cat ~/.spack/linux/compilers.yaml 
```
After having installed and loaded a new compiler, update compilers list and check the result
```
spack load gcc@14.2.0
spack compiler find
spack config get compilers
```

---
# Compilers & toolchains
Check installed packages with `spack find`
Now you can use that compiler to build new packages, eg
```
spack install intel-tbb%gcc14.2.0
```
Check again installed packages with `spack find`

Thus, your installation is independent from OS compiler version and more portable and reproducible. 
Packages compiled with a specific compiler can be found with 
```spack find %<compiler>@version```

---
# Example of application
```
git clone https://github.com/pacs-course/pacs-Labs.git
cd pacs-Labs/Labs/2025/04-algorithms_and_execution_policies
spack load intel-oneapi-tbb@2022.0.0
spack find --loaded
g++ -std=c++23 bandwidth.cpp -O3 -ltbb -o bandwidth
./bandwidth 100000000&
top
spack unload --all 
```
(exit `top` with `q`)
Specific find usage: `spack find -ldv intel-oneapi-tbb`

---
# Build your own module system

Requires either
 - [https://github.com/envmodules/modules](https://github.com/envmodules/modules)
 - [https://github.com/TACC/Lmod](https://github.com/TACC/Lmod)   

Here we refer to `apt info environment-modules`, but they can be installed via spack too.
```
sudo apt install environment-modules
source /etc/profile.d/modules.sh 
module avail
source spack-0.23/share/spack/setup-env.sh
spack module tcl refresh -y 
module avail
module load gcc/14.2.0<...> && module load intel-oneapi-tbb/
module list
```

---
# Final words
where to go from here:
 - [https://spack-tutorial.readthedocs.io/en/latest/tutorial_configuration.html](https://spack-tutorial.readthedocs.io/en/latest/tutorial_configuration.html)
 - [https://spack-tutorial.readthedocs.io/en/latest/tutorial_packaging.html](https://spack-tutorial.readthedocs.io/en/latest/tutorial_packaging.html)

 For the next time, it might be useful to `spack install openmpi%gcc@14.2.0` too

---
# Containers in HPC - refresh from Lab 0
 - podman: OCI (as Docker) compatible, no sudo, sees the shared folder only
 - apptainer: HPC oriented, no sudo,  uses FUSE to manage container file systems
 - alternatives: docker (creation) + sarus (execution) [https://github.com/eth-cscs/sarus](https://github.com/eth-cscs/sarus)

---
# Podman hello world
See hello-world.dockerfile
```
podman build -t hello-world:v1 -f hello-world.dockerfile .
podman run --rm -it hello-world:v1 /usr/local/bin/hello
```

---
# Podman hello world - multistage
See hello-world-multistage.dockerfile
```
podman build -t hello-world:v2 -f hello-world-multistage.dockerfile .
podman run --rm -it hello-world:v2 /usr/local/bin/hello
podman images
```

---
# Repositories & Apptainer
You can upload the container image to repositories such as [https://quay.io/repository/](https://quay.io/repository/) (free registration required to upload, and to follow the [https://quay.io/tutorial/](https://quay.io/tutorial/) ).

Then, you can build apptainer containers from podman ones, eg
```
apptainer pull docker://quay.io/pjbaioni/mk:2024
```

---
# Apptainer from recipe
```
apptainer build ada.sif ada.apptainer_recipe
apptainer exec ada.sif gnatmake hello.adb && \
apptainer exec ada.sif gnatbind -x hello.ali && \
apptainer exec ada.sif gnatlink hello.ali && \
apptainer exec ada.sif ./hello
```

---
# HPCCM & Python Envs
High-Performace Computing Container Maker provides a python based higher level interface to container definitions, already following best practices for docker (default, podman compatible) and singularity (apptainer compatible)
It can be installed in the **recommended** way to install python packages 
```
python3 -m venv hpccm
source /work/hpccm/bin/activate
pip install --upgrade pip
pip install hpccm
```

---
# HPCCM & Python Envs
Example usage
```
hpccm --recipe compilers.hpccm --format singularity > compilers.apptainer
hpccm --recipe compilers.hpccm --format docker > compilers.podman
deactivate
apptainer build compilers.sif compilers.apptainer
apptainer shell compilers.sif
```

---
# References
 - [https://gitlab.hpc.cineca.it/training/build_system_pkg_manager_hpc](https://gitlab.hpc.cineca.it/training/build_system_pkg_manager_hpc)
 - [https://github.com/spack/spack](https://github.com/spack/spack)
 - [https://github.com/NVIDIA/hpc-container-maker](https://github.com/NVIDIA/hpc-container-maker)
 - [https://podman.io/](https://podman.io/)
 - [https://apptainer.org/](https://apptainer.org/)
