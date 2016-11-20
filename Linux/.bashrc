# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
#source ~/.bash_profile

export SHELL=/home/zhifei/App/zsh52/bin/zsh
[ -z "$ZSH_VERSION" ] && exec /home/zhifei/App/zsh52/bin/zsh -l

