# f0plugins
plugins for supercollider



for supercollider 3.5 and above (see below for building under 3.6)

source code and compile files for my supercollider plugins.
see http://www.fredrikolofsson.com/pages/code-sc.html#plugins

to build first install cmake and download the supercollider source via git,
then open terminal and cd to the folder containing this README and do...

> mkdir build
> cd build
> cmake -DSC_PATH=~/supercollider/ -DINSTALL_DESTINATION="/Users/???/Library/Application Support/SuperCollider/Extensions" ..
> make install

edit SC_PATH above to point to your supercollider source directory,
and edit INSTALL_DESTINATION to point to where you want the resulting plugins.


NOTE:
Not tested on Windows. Use the old binaries if problems. They are here...
http://www.fredrikolofsson.com/pages/code-sc.html#plugins


IMPORTANT: The supercollider plugin API changed between versions 3.5 and 3.6 so plugins built for 3.5 or earlier will not load under 3.6.
To build f0plugins for 3.6 make sure you have set the supercollider 3.6 branch (master). Like this...
> cd ~/supercollider
> git checkout master
> git submodule init && git submodule update
and then build f0plugins as normal.








DETAILED BUILD INSTRUCTIONS FOR LINUX:
(tested on ubuntu desktop)

//--preparation
sudo apt-get install git
sudo apt-get install cmake
sudo apt-get install build-essential

//--download f0plugins_src source
http://www.fredrikolofsson.com/pages/code-sc.html#plugins

//--get supercollider source
git clone git://supercollider.git.sourceforge.net/gitroot/supercollider/supercollider
cd supercollider
git checkout 3.5
git submodule init && git submodule update

//--build f0plugins
cd /home/redfrik/f0plugins_src	(or where you put them)
mkdir build
cd build
cmake -DSC_PATH=/home/redfrik/supercollider ..
make install

//--move the files to sc extension folder
sudo mv f0plugins/ /usr/share/SuperCollider/Extensions/

//restart sc and test it...
s.boot
{Atari2600.ar}.play

