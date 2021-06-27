# Ubuntu 20.04 LTS in Docker (v1.5)

## Prerequisites

 * [Docker + Docker-compose](./doc/DOCKER.md) 

## Getting u20-docker

```
git clone https://github.com/acaldero/u20-docker.git
cd u20-docker
./u20.sh build
```

## Using u20-docker

<html>
 <table>
  <tr>
  <th>Summary</th>
  <th>Example of work session</th>
  </tr>
  <tr>
  <td>
</html>

  * First time + "each time u20-dockerfile is updated":
    * ./u20.sh build

  * To start **3** containers:
    *  ./u20.sh start **3**

  * To get into container **2**:
    *  ./u20.sh bash **2**

  * Being within **2**, to exit:
    *  exit

  * To stop the containers:
    *  ./u20.sh stop

  * Available options for debugging:
    *  ./u20.sh status
    *  ./u20.sh network

<html>
  </td>
  <td>
</html>

  * To start:
    * To start a work session with **2** containers, please execute:
      *  ./u20.sh start **2**
    * To check the containers are running please use:
      *  ./u20.sh status
    * To get the containers internal IP address please use:
      *  ./u20.sh network

  * To work with some container:
    * To get into container **1** out of 2 please execute:
      *  ./u20.sh bash **1**
    * <some work inside container **1** at /work directory>
    * To exit from container **1** please use:
      *  exit

  * To stop:
    * To stop the work session please use:
      *  ./u20.sh stop

<html>
  </td>
  </tr>
 </table>
</html>


**Please beware of**:
  * Any modification outside /work will be discarded on container stopping.
  * Please make a backup of your work "frequently".
  * You might need to use "sudo" before ./u20.sh if your user doesn't belong to the docker group (could be solved by using "sudo usermod -aG docker ${USER}")


## Example using u20-docker

![Example of using 2 containers in Linux, and using each one with netcat](doc/u20-example1.gif)


