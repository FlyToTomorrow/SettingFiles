# .bash_profile

# Get the aliases and functions
#if [ -f ~/.bashrc ]; then
	#. ~/.bashrc
#fi

export LD_LIBRARY_PATH=/home/zhifei/App/lib:$LD_LIBRARY_PATH

preferred_shell=
if [ -x /home/zhifei/App/zsh/bin/zsh ]; then
  preferred_shell=/home/zhifei/App/zsh/bin/zsh
fi

if [ -n "$preferred_shell" ]; then
  case $- in
    *i*) SHELL=$preferred_shell; export SHELL; exec "$preferred_shell";;
  esac
fi
