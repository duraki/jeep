## Docker Setup

### Raspberry Pi

Do a clean:

```
# => this does NOT work! $ sudo apt install docker

$ curl -fsSL https://get.docker.com -o get-docker.sh
$ sudo sh get-docker.sh
$ sudo usermod -aG docker ${USER}
$ groups ${USER}
$ logout # => required because of adding user to the group
```

Test Docker installation:

```
$ docker version
Client: Docker Engine - Community
 Version:           20.10.17
 API version:       1.41
 ...

$ docker info
```

Setup a sampled container:

```
$ docker run hello-world
$ docker image rm hello-world --force
```

Setup `docker-compose` as well:

```
$ sudo apt-get install libffi-dev libssl-dev
$ sudo apt install python3-dev
$ sudo apt-get install -y python3 python3-pip
$ sudo pip3 install docker-compose
$ sudo systemctl enable docker # => run docker at boot
```

To delete images leftover, volumes, containers, and additional relative data, run this command:

```
$ sudo rm -rf /var/lib/docker
```

Search specific Docker image:

```
$ sudo docker search <KEYWORD>
# => ie. sudo docker search ubuntu
```

#### can4docker

[can4docker](https://gitlab.com/chgans/can4docker) is a Docker NetworkDriver plugin providing CAN connectivity.
