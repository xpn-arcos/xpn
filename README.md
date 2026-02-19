![Logo for the Expand Parallel File System.](https://github.com/xpn-arcos/xpn/blob/master/info/xpn-logo.svg)





<div align="center">
 
  <h2 align="center">Expand Project</h2>

  ![Workflow](https://github.com/xpn-arcos/xpn-docker/actions/workflows/xpn_docker_workflow.yml/badge.svg)
  ![Release](https://img.shields.io/github/v/release/xpn-arcos/xpn)
  ![License](https://img.shields.io/github/license/xpn-arcos/xpn)
  [![Codacy Badge](https://app.codacy.com/project/badge/Grade/c1bda08c0c68491cb331cc280f754a99)](https://app.codacy.com/gh/xpn-arcos/xpn/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
  ![Stars](https://img.shields.io/github/stars/xpn-arcos/xpn?style=social)
  ![Forks](https://img.shields.io/github/forks/xpn-arcos/xpn?style=social)

  |                                             | URL                                          |
  |:-------------------------------------------:|:--------------------------------------------:|
  | :globe_with_meridians: Project Website      | https://xpn-arcos.github.io                  |
  | :package: Docker Hub                        | https://hub.docker.com/repositories/expandfs |
  | :email: Email                               | xpn.arcos.inf.uc3m.es@gmail.com              |

  <h3 align="center">Source Code</h3>
  
  ![C](https://img.shields.io/badge/language-C-00599C)
  ![MPI](https://img.shields.io/badge/parallel-MPI-005493)
  ![Shell](https://img.shields.io/badge/script-Shell-4EAA25)

  |                              | URL                                     |
  |:----------------------------:|:---------------------------------------:|
  | :clipboard: Source Code      | https://github.com/xpn-arcos/xpn        |
  | :package: XPN Docker         | https://github.com/xpn-arcos/xpn-docker |
  | :microscope: Nightly Build   | https://github.com/xpn-arcos/xpn-beta   |
  
</div>



<div align="center">
  <h2 align="center">Content</h2>
</div>

1. [IO500 Ranking](#io500-ranking)
2. [Publications](#publications)
3. [Authors](#authors)
4. [ChangeLog](#changelog)
5. [Usage](#using-expand)
6. [Sponsors](#sponsors)



<div align="center">
  <h2 align="center">IO500 Ranking</h2>

  | Supercomputer | 10 Node Research List                              | Full List                                            |
  |:-------------:|:--------------------------------------------------:|:----------------------------------------------------:|
  | Leonardo      | 47/113 ([SC24](https://io500.org/list/sc24/ten))   | 114/268 ([SC24](https://io500.org/list/sc24/full))   |
  | Marenostrum 4 | 69/101 ([SC23](https://io500.org/list/sc23/ten))   | 184/236 ([SC23](https://io500.org/list/sc23/full))   |
  | C3-UC3M       | 78/118 ([ISC25](https://io500.org/list/isc25/ten)) | 205/284 ([ISC25](https://io500.org/list/isc25/full)) |

</div>



<div align="center">
  <h2 align="center">Publications</h2>
</div>

### 2025

<details>
<summary>:newspaper: Improving I/O performance in HPC environments using the Expand Ad-Hoc file system</summary>
 
  * Journal paper: The Journal of Supercomputing
  * Authors: Diego Camarmas-Alonso, Felix Garcia-Carballeira, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](https://doi.org/10.1007/s11227-025-08015-x)
  ```bibtex
  @article{camarmas2025improving,
    title={Improving I/O performance in HPC environments using the Expand Ad-Hoc file system},
    author={Camarmas-Alonso, Diego and Garcia-Carballeira, Felix and Calderon-Mateos, Alejandro and Carretero, Jesus},
    journal={The Journal of Supercomputing},
    volume={81},
    number={16},
    pages={1--42},
    year={2025},
    publisher={Springer}
  }
  ```
</details>

<details>
<summary>:newspaper: Malleability and fault tolerance in ad-hoc parallel file systems</summary>

  * Journal paper: Cluster Computing
  * Authors: Dario Muñoz-Muñoz, Felix Garcia-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](https://doi.org/10.1007/s10586-025-05575-8)
  ```bibtex
  @article{munoz2025malleability,
    title={Malleability and fault tolerance in ad-hoc parallel file systems},
    author={Mu{\~n}oz-Mu{\~n}oz, Dario and Garcia-Carballeira, Felix and Camarmas-Alonso, Diego and Calderon-Mateos, Alejandro and Carretero, Jesus},
    journal={Cluster Computing},
    volume={28},
    number={13},
    pages={860},
    year={2025},
    publisher={Springer}
  }
  ```
</details>

<details>
<summary>:newspaper: Hierarchical and distributed data storage for Computing Continuum</summary>

  * Journal paper: Future Generation Computer Systems
  * Authors: Elias Del-Pozo-Puñal, Felix Garcia-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos
  * [:link: Open publication](https://doi.org/10.1016/j.future.2025.107931)
  ```bibtex
  @article{DELPOZOPUNAL2026107931,
    title = {Hierarchical and distributed data storage for computing continuum},
    journal = {Future Generation Computer Systems},
    volume = {174},
    pages = {107931},
    year = {2026},
    issn = {0167-739X},
    doi = {https://doi.org/10.1016/j.future.2025.107931},
    url = {https://www.sciencedirect.com/science/article/pii/S0167739X25002262},
    author = {Elias Del-Pozo-Puñal and Felix Garcia-Carballeira and Diego Camarmas-Alonso and Alejandro Calderon-Mateos}
  }
  ```
</details>

<details>
<summary>:newspaper: Evaluación del sistema de ficheros Expand Ad-Hoc con aplicaciones de uso intensivo de datos</summary>
  
  * Conference paper: XXXV Jornadas de Paralelismo (JP25)
  * Authors: Diego Camarmas-Alonso, Felix Garcia-Carballeira, Alejandro Calderon-Mateos, Darío Muñoz-Muñoz, Jesus Carretero
  * [:link: Open publication](https://doi.org/10.5281/zenodo.15773123)
  ```bibtex
  @article{DIEGOCAMARMASALONSO_JP25,
    title = {Evaluación del sistema de ficheros Expand Ad-Hoc con aplicaciones de uso intensivo de datos},
    conference = {XXXV Jornadas de Paralelismo (JP25)},
    volume = {1},
    pages = {305-314},
    year = {2025},
    doi = {https://doi.org/10.5281/zenodo.15773123},
    url = {https://zenodo.org/records/15773123},
    author = {Diego Camarmas-Alonso, Felix Garcia-Carballeira, Alejandro Calderon-Mateos, Darío Muñoz-Muñoz, Jesus Carretero}
  }
  ```
</details>

<details>
<summary>:newspaper: Sistema de almacenamiento para computing continuum: aplicación a sistemas de información ferroviaria</summary>

  * Conference paper: XXXV Jornadas de Paralelismo (JP25)
  * Authors: Elías Del-Pozo-Puñal, Félix García-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos
  * [:link: Open publication](https://doi.org/10.5281/zenodo.15773179)
  ```bibtex
  @article{DELPOZOPUNAL_JP25,
    title = {Sistema de almacenamiento para computing continuum: aplicación a sistemas de información ferroviaria},
    conference = {XXXV Jornadas de Paralelismo (JP25)},
    volume = {1},
    pages = {315-324},
    year = {2025},
    doi = {https://doi.org/10.5281/zenodo.15773179},
    url = {https://zenodo.org/records/15773179},
    author = {Elías Del-Pozo-Puñal, Félix García-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos}
  }
  ```
</details>

<details>
<summary>:newspaper: LFI: una librería de comunicaciones tolerante a fallos para redes de alto rendimiento</summary>

  * Conference paper:XXXV Jornadas de Paralelismo (JP25)
  * Authors: Darío Muñoz-Muñoz, Félix García-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos
  * [:link: Open publication](https://doi.org/10.5281/zenodo.15773016)
  ```bibtex
  @article{DARIOMUNOZMUNOZ_JP25,
    title = {LFI: una librería de comunicaciones tolerante a fallos para redes de alto rendimiento},
    conference = {XXXV Jornadas de Paralelismo (JP25)},
    volume = {1},
    pages = {205-214},
    year = {2025},
    doi = {https://doi.org/10.5281/zenodo.15773016},
    url = {https://zenodo.org/records/15773016},
    author = {Darío Muñoz-Muñoz, Félix García-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos}
  }
  ```
</details>

<details>
<summary>:newspaper: Optimización de entornos de Big Data Analytics mediante sistemas de ficheros paralelos Ad-hoc</summary>

  * Conference paper: XXXV Jornadas de Paralelismo (JP25)
  * Authors: Gabriel Sotodosos-Morales, Félix García-Carballeira, Diego Camarmas-Alonso, Alejandro Calderón-Mateos, Darío Muñoz-Muñoz, Jesús Carretero
  * [:link: Open publication](https://doi.org/10.5281/zenodo.15773242)
  ```bibtex
  @article{SOTODOSOSMORALES_JP25,
    title = {Optimización de entornos de Big Data Analytics mediante sistemas de ficheros paralelos Ad-hoc},
    conference = {XXXV Jornadas de Paralelismo (JP25)},
    volume = {1},
    pages = {833-842},
    year = {2025},
    doi = {https://doi.org/10.5281/zenodo.15773242},
    url = {https://zenodo.org/records/15773242},
    author = {Gabriel Sotodosos-Morales, Félix García-Carballeira, Diego Camarmas-Alonso, Alejandro Calderón-Mateos, Darío Muñoz-Muñoz, Jesús Carretero}
  }
  ```
</details>


### 2024

<details>
<summary>:newspaper: Fault tolerant in the Expand Ad-Hoc parallel file system</summary>

  * Conference paper: 30th International European Conference on Parallel and Distributed Computing (Euro-Par)
  * Authors: Dario Muñoz-Muñoz, Felix Garcia-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](https://doi.org/10.1007/978-3-031-69766-1_5)
  ```bibtex
  @InProceedings{10.1007/978-3-031-69766-1_5,
    author="Mu{\~{n}}oz-Mu{\~{n}}oz, Dario and Garcia-Carballeira, Felix and Camarmas-Alonso, Diego and Calderon-Mateos, Alejandro and Carretero, Jesus",
    title="Fault Tolerant in the Expand Ad-Hoc Parallel File System",
    booktitle="Euro-Par 2024: Parallel Processing",
    year="2024",
    publisher="Springer Nature Switzerland",
    address="Cham",
    pages="62--76",
    isbn="978-3-031-69766-1"
  }
  ```
</details>

<details>
<summary>:newspaper: Malleability in the Expand Ad-Hoc parallel file system</summary>

  * Conference paper: 3rd EuroHPC Workshop on Dynamic Resources in HPC. Euro-Par 2024
  * Authors: Dario Muñoz-Muñoz, Felix Garcia-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](https://doi.org/10.1007/978-3-031-90200-0_26)
  ```bibtex
  @InProceedings{10.1007/978-3-031-90200-0_26,
    author="Mu{\~{n}}oz-Mu{\~{n}}oz, Dario and Garcia-Carballeira, Felix and Camarmas-Alonso, Diego and Calderon-Mateos, Alejandro and Carretero, Jesus",
    title="Malleability in the Expand Ad-Hoc Parallel File System",
    booktitle="Euro-Par 2024: Parallel Processing Workshops",
    year="2025",
    publisher="Springer Nature Switzerland",
    address="Cham",
    pages="322--333",
    isbn="978-3-031-90200-0"
  }
  ```
</details>

<details>
<summary>:newspaper: Tolerancia a fallos en el sistema de ficheros paralelo Expand Ad-Hoc</summary>

  * Conference paper: XXXIV Jornadas de Paralelismo (JP24)
  * Authors: Dario Muñoz-Muñoz, Diego Camarmas-Alonso, Felix Garcia-Carballeira, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](http://dx.doi.org/10.5281/zenodo.12743582)
  ```bibtex
  @inproceedings{munoz_munoz_2024_12743583,
    author       = {Muñoz-Muñoz, Dario and  Camarmas Alonso, Diego and  Garcia-Carballeira, Felix and Calderon-Mateos, Alejandro and Carretero, Jesus},
    title        = {Tolerancia a fallos en el sistema de ficheros paralelo Expand Ad-Hoc},
    booktitle    = {Avances en Arquitectura y Tecnología de Computadores. Actas de las Jornadas SARTECO},
    year         = 2024,
    pages        = {271-279},
    publisher    = {Zenodo},
    month        = jul,
    venue        = {A Coruña, España},
    doi          = {10.5281/zenodo.12743583},
    url          = {https://doi.org/10.5281/zenodo.12743583},
  }
  ```
</details>

<details>
<summary>:newspaper: Evaluación del rendimiento de un sistema de ficheros para sistemas IoT</summary>

  * Conference paper: XXXIV Jornadas de Paralelismo (JP24)
  * Authors: Elias Del-Pozo-Puñal, Felix Garcia-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos
  * [:link: Open publication](http://dx.doi.org/10.5281/zenodo.12094741)
  ```bibtex
  @inproceedings{del_pozo_punal_2024_12094742,
    author       = {Del-Pozo-Puñal, Elias and  Garcia-Carballeira, Felix and Camarmas-Alonso, Diego and Calderon-Mateos, Alejandro},
    title        = {Evaluación del rendimiento de un sistema de ficheros para sistemas IoT},
    booktitle    = {Avances en Arquitectura y Tecnología de Computadores. Actas de las Jornadas SARTECO},
    year         = 2024,
    pages        = {289-298},
    publisher    = {Zenodo},
    month        = jun,
    venue        = {A Coruña, Galicia, España},
    doi          = {10.5281/zenodo.12094742},
    url          = {https://doi.org/10.5281/zenodo.12094742},
  }
  ```
</details>


### 2023

<details>
<summary>:newspaper: A new Ad-Hoc parallel file system for HPC environments based on the Expand parallel file system</summary>

  * Conference paper: 22nd International Symposium on Parallel and Distributed Computing (ISPDC)
  * Authors: Felix Garcia-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](http://dx.doi.org/10.1109/ISPDC59212.2023.00015)
  ```bibtex
  @INPROCEEDINGS{10272428,
    author={Garcia-Carballeira, Felix and Camarmas-Alonso, Diego and Caderon-Mateos, Alejandro and Carretero, Jesus},
    booktitle={2023 22nd International Symposium on Parallel and Distributed Computing (ISPDC)}, 
    title={A new Ad-Hoc parallel file system for HPC environments based on the Expand parallel file system}, 
    year={2023},
    volume={},
    number={},
    pages={69-76},
    keywords={File systems;Distributed computing;Ad-Hoc File System;HPC;Parallel I/O},
    doi={10.1109/ISPDC59212.2023.00015}
  }
  ```
</details>

<details>
<summary>:newspaper: Evaluación de rendimiento del sistema de ficheros paralelo Expand Ad-Hoc en MareNostrum 4</summary>

  * Conference paper: XXXIII Jornadas de Paralelismo (JP23)
  * Authors: Diego Camarmas-Alonso, Felix Garcia-Carballeira, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](http://dx.doi.org/10.5281/zenodo.8378956)
  ```bibtex
  @misc{diego_camarmas_alonso_2023_8378956,
    author       = {Diego Camarmas-Alonso and Felix Garcia-Carballeira and Alejandro Calderon-Mateos and  Jesus Carretero},
    title        = {Evaluación de rendimiento del sistema de ficheros paralelo Expand Ad-Hoc en MareNostrum 4},
    month        = sep,
    year         = 2023,
    publisher    = {Zenodo},
    doi          = {10.5281/zenodo.8378956},
    url          = {https://doi.org/10.5281/zenodo.8378956},
  }
  ```
</details>

<details>
<summary>:newspaper: Sistema de Ficheros Distribuido para IoT basado en Expand</summary>

  * Conference paper: XXXIII Jornadas de Paralelismo (JP23)
  * Authors:  Elias Del-Pozo-Puñal, Felix Garcia-Carballeira, Diego Camarmas-Alonso, Alejandro Calderon-Mateos
  * [:link: Open publication](http://dx.doi.org/10.5281/zenodo.10706248)
  ```bibtex
  @misc{del_pozo_punal_2023_10706248,
    author       = {Del-Pozo-Puñal, Elías and Garcia-Carballeira, Felix andCamarmas-Alonso, Diego andCalderon-Mateos, Alejandro},
    title        = {Sistema de Ficheros Distribuido para IoT basado en Expand},
    month        = sep,
    year         = 2023,
    publisher    = {Zenodo},
    version      = {Version v1},
    doi          = {10.5281/zenodo.10706248},
    url          = {https://doi.org/10.5281/zenodo.10706248},
  }
  ```
</details>


### 2022

<details>
<summary>:newspaper: Sistema de almacenamiento Ad-Hoc para entornos HPC basado en el sistema de ficheros paralelo Expand</summary>

  * Conference paper: XXXII Jornadas de Paralelismo (JP22)
  * Authors:  Diego Camarmas-Alonso, Felix Garcia-Carballeira, Alejandro Calderon-Mateos, Jesus Carretero
  * [:link: Open publication](http://dx.doi.org/10.5281/zenodo.6862882)
  ```bibtex
  @misc{diego_camarmas_alonso_2021_14258796,
    author       = {Diego Camarmas-Alonso and Felix Garcia-Carballeira and Alejandro Calderon Mateos and Jesus Carretero Perez},
    title        = {Sistema de almacenamiento Ad-Hoc para entornos HPC basado en el sistema de ficheros paralelo Expand},
    month        = sep,
    year         = 2021,
    publisher    = {Zenodo},
    doi          = {10.5281/zenodo.14258796},
    url          = {https://doi.org/10.5281/zenodo.14258796},
  }
  ```
</details>



<div align="center">
  <h2 align="center">Authors</h2>
</div>

  * :technologist: [Félix García Carballeira](https://github.com/fgcarbal)
  * :technologist: [Alejandro Calderón Mateos](https://github.com/acaldero)
  * :technologist: Luis Miguel Sanchez Garcia
  * :technologist: Borja Bergua Guerra
  * :technologist: [Diego Camarmas Alonso](https://github.com/dcamarmas)
  * :technologist: [Elías Del Pozo Puñal](https://github.com/edelpozop)
  * :technologist: [Dario Muñoz Muñoz](https://github.com/dariomnz)
  * :technologist: [Gabriel Sotodosos Morales](https://github.com/gsotodos)
  * :technologist: Jesús Carretero
    
</details>



<div align="center">
  <h2 align="center">ChangeLog</h2>
</div>

<details open>
<summary>:atom: 3.3.1</summary>

  - [x] Expand proxy added
  - [x] Support for Apache Spark text and image Dataframes
  - [x] C3 Platform added
  - [x] Minor updates

</details>

<details>
<summary>:atom: 3.3.0</summary>

  - [x] Spark connector added
  - [x] IPv6 support added
  - [x] Added support for 32-bits version of Expand

</details>

<details>
<summary>:atom: 3.2.0</summary>

  - [x] MQTT support added: mpi_server, sck_server and now mq_server available
  - [x] Cleanup and minor improvements on socket support
  - [x] Improvements on the configuration file reader
  - [x] Minor updates on examples

</details>

<details>
<summary>:atom: 3.1.0</summary>

  - [x] Metadata operations improved
  - [x] Minor bug fixes and improvements

</details>

<details>
<summary>:atom: 3.0.0</summary>

  - [x] Fault tolerance support based on replication added
  - [x] Maleability support improved
  - [x] Expand deployment based on docker added
  - [x] MXML dependency removed
  - [x] Simplified user experience: more user-friendly way to start and stop ad hoc server
  - [x] Minor bug fixes and improvements

</details>

<details>
<summary>:atom: 2.2.2</summary>

  - [x] TCP server rebuilt from scratch
  - [x] New system calls intercepted
  - [x] Preload and flush data operations added
  - [x] Minor bug fixes and improvements

</details>

<details>
<summary>:atom: 2.2.1</summary>

  - [x] Minor bug fixes and improvements

</details>

<details>
<summary>:atom: 2.2.0</summary>

  - [x] Maleability support

</details>

<details>
<summary>:atom: 2.1.0</summary>

  - [x] Spack support added
  - [x] Simplified user experience: now it is easier to start and stop expand ad-hoc servers
  - [x] Code refactoring

</details>

<details>
<summary>:atom: 2.0.0</summary>

  - [x] First XPN Ad-Hoc release
  - [x] This version provides:
     * Simplifiyed build system based on build-me script
     * Platforms: MPICH and Intel MPI
     * Benchmarks supported: IOR, MdTest and IO500
     * API: POSIX (through syscall interception library) and native XPN
     * Features: data locality, MPI Ad-Hoc servers, thread on-demand or thread pool for MPI servers

</details>



<div align="center">
  <h2 align="center">Using Expand</h2>
</div>

See [`docs/RUNNING.md`](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md).

  * [1. To deploy Ad-Hoc XPN...](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md#1-to-deploy-ad-hoc-xpn)
    * [1.1 Deploying on a cluster/supercomputer](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md#11-deploying-ad-hoc-expand-on-a-clustersupercomputer)
    * [1.2 Deploying on a IoT distributed system](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md#12-deploying-ad-hoc-expand-on-a-iot-distributed-system)
  * [2. Executing Ad-Hoc XPN...](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md#2-executing-ad-hoc-xpn)
    * [2.1 Executing Ad-Hoc Expand using MPICH](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md#21-executing-ad-hoc-expand-using-mpich)
    * [2.2 Executing Ad-Hoc Expand using OpenMPI (experimental)](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md#22-executing-ad-hoc-expand-using-openmpi-experimental-alpha)
  * [3. To use Apache Spark connector...](https://github.com/xpn-arcos/xpn/blob/master/docs/RUNNING.md#3-to-use-apache-spark-connector)



<div align="center">
  <h2 align="center">Sponsors</h2>
</div>

Expand - High performance storage system for HPC and Big Data environments (TED2021-131798B-I00) funded by MICIU/AEI/ 10.13039/501100011033 and by the European Union NextGenerationEU/PRTR. https://expand-arcos.github.io
![sponsor logos](https://github.com/user-attachments/assets/a37ba843-dd07-4014-be3c-ee105bd2d463)
