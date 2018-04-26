# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
#source ~/.bash_profile

# export LD_LIBRARY_PATH=/home/zhifei/App/lib:$LD_LIBRARY_PATH

preferred_shell=
if [ -x /home/zhifei/App/zsh/bin/zsh ]; then
  preferred_shell=/home/zhifei/App/zsh/bin/zsh
fi

if [ -n "$preferred_shell" ]; then
  case $- in
    *i*) SHELL=$preferred_shell; export SHELL; exec "$preferred_shell";;
  esac
fi

# added by Anaconda3 installer
export PATH="/home/zhifei/App/anaconda3/bin:$PATH"
