# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
#source ~/.bash_profile


# User specific environment and startup programs
# PATH=$PATH:$HOME/bin
PATH=/home/zhifei/App/anaconda2/bin/:$PATH
PATH=/home/zhifei/App/anaconda3/bin/:$PATH
PATH=/home/zhifei/App/bin/:$PATH
PATH=/home/zhifei/App/jre1.8.0_91/bin:$PATH
PATH=/home/zhifei/App/gcc47/bin:$PATH

# Alias
alias ls="ls -l --color=always"
alias lm="ls -l --color=always|more"
alias vim="/home/zhifei/App/bin/vim"
alias p3="/home/zhifei/App/anaconda3/bin/python3"
alias p2="/home/zhifei/App/anaconda2/bin/python2.7"
alias pycharm="/home/zhifei/App/pycharm-community-2016.1.4/bin/pycharm.sh"
alias tmux="tmux -2"
alias grep="grep --color=auto"
eval "$(thefuck --alias)"
eval "$(thefuck --alias FUCK)"


# Other variables
setterm -regtabs 4
export TERM=xterm-256color
export LD_LIBRARY_PATH=/home/zhifei/App/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/zhifei/App/gcc62/lib:/home/zhifei/App/gcc62/lib64:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH=/home/zhifei/App/anaconda3/lib:/home/zhifei/App/anaconda3/lib64:$LD_LIBRARY_PATH
#LD_LIBRARY_PATH=:$LD_LIBRARY_PATH

#export LDFLAGS="-L/home/zhifei/App/gcc62/lib"
#export CPPFLAGS="-I/home/zhifei/App/gcc62/include"
export JAVA_HOME=/home/zhifei/App/jre1.8.0_91
#export CC=/home/zhifei/App/gcc47/bin/gcc
#export CXX=/home/zhifei/App/gcc47/bin/g++
#export CPP=/home/zhifei/App/gcc47/bin/g++
export DISPLAY=localhost:10.0
export EDITOR=vim
export BASH4=/home/zhifei/App/bash4/bin/bash


export PATH
