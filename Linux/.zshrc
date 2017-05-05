# If you come from bash you might have to change your $PATH.

# PATH=/home/zhifei/App/Qt/5.8/gcc_64/bin:$PATH
PATH=/home/zhifei/App/w3m/usr/bin:$PATH
# PATH=/home/zhifei/App/anaconda3/bin/:$PATH
PATH=/home/zhifei/App/bin/:$PATH
PATH=/home/zhifei/App/netbeans/bin/:$PATH
PATH=/home/zhifei/App/jre1.8.0_131/bin:$PATH
PATH=/home/zhifei/App/gcc63/bin:$PATH
PATH=/home/zhifei/App/zsh/bin:$PATH
PATH=/home/zhifei/App/TexLive/bin/x86_64-linux:$PATH
PATH=/home/zhifei/App/chrome/usr/bin:$PATH
PATH=/home/zhifei/App/pycharm/bin:$PATH
PATH=/home/zhifei/App/zotero:$PATH
PATH=/home/zhifei/App/MATLAB/R2016b/bin:$PATH
PATH=/home/zhifei/App/eclipse/cpp-neon/eclipse/:$PATH
PATH=/home/zhifei/App/lyn/bin:$PATH
PATH=/home/zhifei/App/qt/bin:$PATH
PATH=/home/zhifei/App/w3m/usr/bin:$PATH
PATH=/home/zhifei/App/llvm/bin:$PATH
export PATH

export CXX=/home/zhifei/App/gcc63/bin/g++
export CC=/home/zhifei/App/gcc63/bin/gcc

# Path to your oh-my-zsh installation.
export ZSH=/home/zhifei/.oh-my-zsh

# Set name of the theme to load. Optionally, if you set this to "random"
# it'll load a random theme each time that oh-my-zsh is loaded.
# See https://github.com/robbyrussell/oh-my-zsh/wiki/Themes
ZSH_THEME="robbyrussell"

# Uncomment the following line to use case-sensitive completion.
# CASE_SENSITIVE="true"

# Uncomment the following line to use hyphen-insensitive completion. Case
# sensitive completion must be off. _ and - will be interchangeable.
# HYPHEN_INSENSITIVE="true"

# Uncomment the following line to disable bi-weekly auto-update checks.
# DISABLE_AUTO_UPDATE="true"

# Uncomment the following line to change how often to auto-update (in days).
# export UPDATE_ZSH_DAYS=13

# Uncomment the following line to disable colors in ls.
# DISABLE_LS_COLORS="true"

# Uncomment the following line to disable auto-setting terminal title.
# DISABLE_AUTO_TITLE="true"

# Uncomment the following line to enable command auto-correction.
# ENABLE_CORRECTION="true"

# Uncomment the following line to display red dots whilst waiting for completion.
# COMPLETION_WAITING_DOTS="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# The optional three formats: "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# HIST_STAMPS="mm/dd/yyyy"

# Would you like to use another custom folder than $ZSH/custom?
# ZSH_CUSTOM=/path/to/new-custom-folder

# Which plugins would you like to load? (plugins can be found in ~/.oh-my-zsh/plugins/*)
# Custom plugins may be added to ~/.oh-my-zsh/custom/plugins/
# Example format: plugins=(rails git textmate ruby lighthouse)
# Add wisely, as too many plugins slow down shell startup.
plugins=(git)

source $ZSH/oh-my-zsh.sh

# User configuration

# export MANPATH="/usr/local/man:$MANPATH"

# You may need to manually set your language environment
# export LANG=en_US.UTF-8

# Preferred editor for local and remote sessions
# if [[ -n $SSH_CONNECTION ]]; then
#   export EDITOR='vim'
# else
#   export EDITOR='mvim'
# fi

# Compilation flags
# export ARCHFLAGS="-arch x86_64"

# ssh
# export SSH_KEY_PATH="~/.ssh/dsa_id"

# Set personal aliases, overriding those provided by oh-my-zsh libs,
# plugins, and themes. Aliases can be placed here, though oh-my-zsh
# users are encouraged to define aliases within the ZSH_CUSTOM folder.
# For a full list of active aliases, run `alias`.
#
# Example aliases
# alias zshconfig="mate ~/.zshrc"
# alias ohmyzsh="mate ~/.oh-my-zsh"

# ======================= customed alias
alias cloc="/home/zhifei/App/cloc/cloc"
alias rssowl="/home/zhifei/App/rssowl/rssowl"
alias zotero="/home/zhifei/App/zotero/zotero"
alias vscode="/home/zhifei/App/vscode/code"
alias a="/home/zhifei/App/bin/aria2c"
alias syncthing="/home/zhifei/App/syncthing/syncthing"
alias chrome="/home/zhifei/App/chrome/opt/google/chrome/google-chrome --no-sandbox --always-authorize-plugins --allow-outdated-plugins %U"
alias ls="ls -lh --color=always"
alias lm="ls -l --color=always|more"
alias vim="/home/zhifei/App/bin/vim"
alias RStudio="/home/zhifei/App/RStudio/lib/rstudio/bin/rstudio"
alias matlab="/home/zhifei/App/MATLAB/R2016b/bin/matlab"
alias pycharm="/home/zhifei/App/pycharm/bin/pycharm.sh"
alias mode="/opt/lumerical/mode/bin/mode-solutions"
alias fdtd="/opt/lumerical/fdtd/bin/fdtd-solutions"
alias interconnect="/opt/lumerical/interconnect/bin/interconnect"
alias tmux="tmux -2"
alias ta="tmux -2 a"
alias grep="grep --color=auto"
# eval "$(thefuck --alias)"
# eval "$(thefuck --alias FUCK)"
# ======================= 

# Other variables
setterm -regtabs 4
[ -z "$TMUX" ] && export TERM=xterm-256color

LD_LIBRARY_PATH=/home/zhifei/App/lib:/home/zhifei/App/lib64:$LD_LIBRARY_PATH
LD_LIBRARY_PATH=/home/zhifei/App/qt4/lib:/home/zhifei/App/qt4/lib64:$LD_LIBRARY_PATH
LD_LIBRARY_PATH=/home/zhifei/App/gcc63/lib:/home/zhifei/App/gcc63/lib64:$LD_LIBRARY_PATH
# LD_LIBRARY_PATH=/home/zhifei/App/qt/lib:/home/zhifei/App/qt/lib64:$LD_LIBRARY_PATH
LD_LIBRARY_PATH=/home/zhifei/App/Qt/5.8/gcc_64/lib:/home/zhifei/App/Qt/5.8/gcc_64/lib64:$LD_LIBRARY_PATH
LD_LIBRARY_PATH=/home/zhifei/App/llvm/lib:/home/zhifei/App/llvm/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

# export PYTHON_INCLUDE_DIR=/home/zhifei/App/anaconda3/include/python3.6m
# export PYTHON_LIBRARY=/home/zhifei/App/anaconda3/lib
# export PYTHONPATH=/home/zhifei/App/anaconda3/lib/python3.6/site-packages
export PYENV_ROOT="$HOME/.pyenv"
export PATH="$PYENV_ROOT/bin:$PATH"
eval "$(pyenv init -)"

# GTK_IM_MODULE=gtk-im-context-simple

export EDITOR=vim
export MATLABPATH=/home/zhifei/Matlab/Add_on/export_fig/:/home/zhifei/Matlab/Add_on/CustomizedFunctions/

# hstr related
# add this configuration to ~/.bashrc
export HH_CONFIG=hicolor         # get more colors
# shopt -s histappend              # append new history items to .bash_history
export HISTCONTROL=ignorespace   # leading space hides commands from history
export HISTFILESIZE=10000        # increase history file size (default is 500)
export HISTSIZE=${HISTFILESIZE}  # increase history size (default is 500)
export HISTFILE=~/.zsh_history
#export PROMPT_COMMAND="history -a; history -n; ${PROMPT_COMMAND}"   # mem/file sync
# if this is interactive shell, then bind hh to Ctrl-r (for Vi mode check doc)
bindkey -s "\C-r" "\C-ahh -- \C-j"

# fasd related
# eval "$(fasd --init auto)"


plugins=( git bundler zsh-syntax-highlighting)