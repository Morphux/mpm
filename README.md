#MPM
### Morphux Package Manager
*Version*: 1.0
### Dependencies
```
autotools
libarchive
libcurl
```

### Install
```
./configure 
make
make install
```

### Configuration

Location in /etc/mpm/config.json

## What MPM should do:
Basicly, MPM is just a package manager. But we want MPM to be a little more than that:

### Profiles

Profiles are a computer type / style. For example, morphux can be used as a web-server, or a home-computer.
So, when you ask mpm to install the program X, the manager looks first if a another program isnt better for your configuration. Simple Example:

```sudo mpm install apache2```

 - Profile Server: No question asked, let's install apache.
 - Profile Low-Performance computer: lighttpd is maybe more adapted for your config ?
 - Profile Home-Computer: You really want to install a web server (Link to what a web server is) ? If so, maybe is better to change your profile to *-dev.
 
Note the *-dev* here. All the profiles tagged with -dev install the developers libs when they are available.

__Important Note__: Of course, anytime you want, you can tell the program to fuck off and listen to you.

### Profiles equivalence

As you can see, all of the concept is based on a equivalence principle between a program A and a program B.

So, we can make equivalence between profile: Let's say you got a server with all your config on it, and a work environment you like, and you want to copy that environment at home. But your computer home is not that fast, and you are afraid that this-program-who-makes-coffee does not work / way too high-performance for your little home-computer.

With the mpm database, we can export your environment and create an equivalence between this profile and something more adapted at your home computer.

### Managing everything

Mpm is a package manager, yes, but it handle everything. For examples:

- Vim plugins
- NPM / Pip / composer

The goal is that MPM handle your all system, and be more efficient at dependencies, and export your all configuration / work-env.

### Interface

Mpm have 3 interfaces:

- A text-terminal one: Like apt or merge
![Image](http://i.imgur.com/iW0WnHg.jpg)

- A ncurses terminal one: For users less confortable with the terminal.
- GTK full graphic one: For basic users, who don't know anything about terminal.

### Distribution

MPM handle every form of package distribution:

- Full sources, Gentoo like: The API distribute the sources, and the compilation is on the host.
- Sources & Binary, Debian like: The API distribute the binary of the package, and some sources / docs for the host.
- Full binary, Windows Like: The API distribute only the binary(ies) of the package.

Of course, you can choose anytime the more adapted solution for your config.

#### Currently in development
