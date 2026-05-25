# Container

This is the sample code for the blog post I wrote on how to create a your own [containerization engine](https://cesarvr.io/post/2018-05-22-create-containers/) (similar to Podman / Docker). 

To run this example just need to make sure you have this components: 

- g++
- gcc
- make
- And of course a Linux 2.5.+ or above. 

```sh
git clone https://github.com/cesarvr/container
cd container 

# run Make 
make 

sudo ./container 
```

This code creates a container for the bash process, by creating a new process and modifying the namespaces, cgroup rules, etc.

![](https://raw.githubusercontent.com/cesarvr/cesarvr.github.io/master/static/containers/setup_root.gif)
