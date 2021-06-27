# Get docker and docker-compose

## Docker

  * Full Instructions for Windows 10 (20H2) available on: https://docs.docker.com/docker-for-windows/wsl/
  * Full instructions for Linux available on: https://docs.docker.com/engine/install/ubuntu/#install-using-the-repository \
    (Thanks to Erik de la Cruz Trujillo) 

**docker on Ubuntu 20.04**
```
sudo apt update
sudo apt install -y apt-transport-https ca-certificates curl software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable"
sudo apt update
sudo apt -y install docker-ce
sudo usermod -aG docker ${USER}
```

## Docker-compose

* Full instructions available on: https://docs.docker.com/compose/install/

**docker-compose on Ubuntu 20.04**

```
sudo curl -L "https://github.com/docker/compose/releases/download/1.28.3/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
sudo ln -s /usr/local/bin/docker-compose /usr/bin/docker-compose
```
