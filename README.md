# Container

This is the sample code from the [How to create your own containers](https://cesarvr.io/post/2018-05-22-create-containers/) blog post. 

To run this example just need g++, gcc, make and of course Linux 2.5.+. 

```sh
git clone https://github.com/cesarvr/container
cd container 

# run Make 
make 

sudo ./container 
```

This code creates a container for the bash process, by creating a new process and modifying the namespaces, cgroup rules, etc.

![](https://raw.githubusercontent.com/cesarvr/cesarvr.github.io/master/static/containers/setup_root.gif)
