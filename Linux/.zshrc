# =============================================================================
#                     PATH & LD_LIBRARY_PATH setting
# =============================================================================

PATH=/usr/bin:/usr/sbin
LD_LIBRARY_PATH=/usr/lib64:/usr/lib

if [ -d "/home/zhifei/App/llvm" ]; then
    PATH=/home/zhifei/App/llvm/bin:$PATH
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/llvm/lib
fi

if [ -d "/home/zhifei/App/openmpi" ]; then
    PATH=$PATH:/home/zhifei/App/openmpi/bin
    LD_LIBRARY_PATH=/home/zhifei/App/openmpi/lib:$LD_LIBRARY_PATH
fi

if [ -d "/home/zhifei/App/Qt/5.8" ]; then
    PATH=$PATH:/home/zhifei/App/Qt/5.8/gcc_64/bin
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/Qt/5.8/gcc_64/lib
fi

if [ -d "/home/zhifei/App/anaconda3" ]; then
    PATH=/home/zhifei/App/anaconda3/bin:$PATH
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/anaconda3/lib
fi


PATH=/home/zhifei/App/gcc72/bin:$PATH
LD_LIBRARY_PATH=/home/zhifei/App/gcc72/lib64:/home/zhifei/App/gcc72/lib:$LD_LIBRARY_PATH

PATH=/home/zhifei/App/bin/:$PATH
LD_LIBRARY_PATH=/home/zhifei/App/lib:/home/zhifei/App/lib64:$LD_LIBRARY_PATH

PATH=/home/zhifei/App/zsh/bin:$PATH
PATH=$PATH:/home/zhifei/App/jre1.8.0_91/bin
PATH=$PATH:/home/zhifei/.cargo/bin

if [ -d "/home/zhifei/App/w3m" ]; then
    PATH=$PATH:/home/zhifei/App/w3m/usr/bin
fi

if [ -d "/home/zhifei/App/COIN" ]; then
    PATH=$PATH:/home/zhifei/App/COIN/bin
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/COIN/lib
fi

if [ -d "/home/zhifei/App/texlive" ]; then
    PATH=$PATH:/home/zhifei/App/texlive/bin/x86_64-linux
fi

if [ -d "/home/zhifei/App/R" ]; then
    PATH=$PATH:/home/zhifei/App/R/bin
fi

if [ -d "/home/zhifei/App/rstudio/lib/rstudio" ]; then
    PATH=$PATH:/home/zhifei/App/rstudio/lib/rstudio/bin
fi

if [ -d "/home/zhifei/App/gnome" ]; then
    PATH=$PATH:/home/zhifei/App/gnome/bin
fi
if [ -d "/home/zhifei/App/doublecmd" ]; then
    PATH=$PATH:/home/zhifei/App/doublecmd
fi
if [ -d "/home/zhifei/App/Typora" ]; then
    PATH=$PATH:/home/zhifei/App/Typora
fi

if [ -d "/home/zhifei/App/gtk3-devel" ]; then
    PATH=$PATH:/home/zhifei/App/gtk3-devel/bin
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/gtk3-devel/lib64
fi
if [ -d "/home/zhifei/App/glib2-devel" ]; then
    PATH=$PATH:/home/zhifei/App/glib2-devel/bin
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/glib2-devel/lib64
fi
if [ -d "/home/zhifei/App/X11" ]; then
    PATH=$PATH:/home/zhifei/App/X11/bin
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/X11/lib64
fi
if [ -d "/home/zhifei/App/meld" ]; then
    PATH=$PATH:/home/zhifei/App/meld/bin
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/zhifei/App/meld/lib
fi

PATH=$PATH:/usr/local/bin:/usr/local/sbin

export PATH
export LD_LIBRARY_PATH

export CXX=/home/zhifei/App/gcc72/bin/g++
export CC=/home/zhifei/App/gcc72/bin/gcc

if [ -d "$HOME/.pyenv" ]; then
    export PYENV_ROOT="$HOME/.pyenv"
    export PATH="$PATH:$PYENV_ROOT/bin"
    eval "$(pyenv init -)"
fi



# =============================================================================
#                               other settings
# =============================================================================

# Path to your oh-my-zsh installation.
export ZSH=/home/zhifei/.oh-my-zsh
[ -z "$TMUX" ] && export TERM=xterm-256color
export EDITOR='vim'

# Set name of the theme to load. Optionally, if you set this to "random"
# it'll load a random theme each time that oh-my-zsh is loaded.
# See https://github.com/robbyrussell/oh-my-zsh/wiki/Themes
#ZSH_THEME="robbyrussell"
ZSH_THEME="powerlevel9k/powerlevel9k"
plugins=(git zsh-syntax-highlighting )
source $ZSH/oh-my-zsh.sh

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
eval "$(fasd --init auto)"
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


# =============================================================================
#                            alias
# =============================================================================


alias lm="/usr/bin/ls -l --color=always|more"
alias l="/usr/bin/ls -t"
# alias ipython="/home/zhifei/App/anaconda3/bin/ipython"
# alias spider="/home/zhifei/App/anaconda3/bin/spider"
alias tmux="tmux -2"
alias ta="tmux -2 a"
# alias mode="/opt/lumerical/mode/bin/mode-solutions"
# alias fdtd="/opt/lumerical/fdtd/bin/fdtd-solutions"
# alias interconnect="/opt/lumerical/interconnect/bin/interconnect"
alias grep="grep --color=auto"
# exa:
alias ls="exa -abghl --git --color=automatic"
alias lst="exa -abghl --git --color=automatic --sort newest"
# `cat` with beautiful colors. requires: pip install -U Pygments
alias c='pygmentize -O style=borland -f console256 -g'

if [ -f "/home/zhifei/App/pycharm-2018.1.1/bin/pycharm.sh" ]; then
    alias pycharm="/home/zhifei/App/pycharm-2018.1.1/bin/pycharm.sh"
fi
if [ -f "/home/zhifei/App/firefox/firefox" ]; then
    alias firefox="/home/zhifei/App/firefox/firefox"
fi
if [ -f "/home/zhifei/App/vim/bin/vim" ]; then
    alias vim="/home/zhifei/App/vim/bin/vim"
fi
if [ -f "/home/zhifei/App/vscode/code" ]; then
    alias code="/home/zhifei/App/vscode/code"
fi
if [ -f "/home/zhifei/App/syncthing/syncthing" ]; then
    alias syncthing="/home/zhifei/App/syncthing/syncthing"
fi
if [ -f "/home/zhifei/App/zotero/zotero" ]; then
    alias zotero="/home/zhifei/App/zotero/zotero"
fi
if [ -f "/home/zhifei/App/clion/bin/clion.sh" ]; then
    alias clion="/home/zhifei/App/clion/bin/clion.sh"
fi
if [ -f "/home/zhifei/App/rslsync/rslsync" ]; then
    alias rslsync="/home/zhifei/App/rslsync/rslsync"
fi
# [ -d "/home/zhifei/App/googler-3.5" ] && alias googler="/home/zhifei/App/googler-3.5/googler"

alias ss='svn status'
alias sa='svn add'
alias sc='svn commit -m'

alias gs='git status'
alias ga='git add'
alias gc='git commit -m'
alias gpc='git push zhifei@fantasy.ece.ust.hk:/home/zhifei/GitRepositories/CLOSO master'

alias v='f -e vim'  # quick opening files with vim
# =============================================================================
#                                Key Bindings
# =============================================================================

# Common CTRL bindings.
bindkey "^a" beginning-of-line
bindkey "^e" end-of-line
# bindkey "^f" forward-word
# bindkey "^b" backward-word
bindkey "^k" kill-line
bindkey "^w" backward-kill-word
bindkey "^u" backward-kill-line

# Do not require a space when attempting to tab-complete.
bindkey "^i" expand-or-complete-prefix

# Fixes for alt-backspace and arrows keys
bindkey '^[^?' backward-kill-word
# bindkey '^[[3;3~' forward-kill-word
bindkey "^[[1;3C" forward-word
bindkey "^[[1;3D" backward-word


# =============================================================================
#                                   Powerlevel
# =============================================================================
POWERLEVEL9K_PROMPT_ON_NEWLINE=true
POWERLEVEL9K_RPROMPT_ON_NEWLINE=false
POWERLEVEL9K_LEFT_PROMPT_ELEMENTS=(dir)
POWERLEVEL9K_RIGHT_PROMPT_ELEMENTS=(status vcs time)
POWERLEVEL9K_STATUS_VERBOSE=false
POWERLEVEL9K_SHORTEN_STRATEGY="truncate_middle"
POWERLEVEL9K_SHORTEN_DIR_LENGTH=10
POWERLEVEL9K_TIME_FORMAT="%D{%H:%M:%S %d/%m/%Y}"
POWERLEVEL9K_LEFT_SEGMENT_SEPARATOR=''
POWERLEVEL9K_RIGHT_SEGMENT_SEPARATOR=''

# =============================================================================
#                           Time command output format
# =============================================================================
TIMEFMT=$'\n''----------------------statics---------------------------'$'\n'\
'command: %J'$'\n'\
'Time: %U user| %S system| %P cpu| %*E total'$'\n'\
'Ave MEM:                %K KB'$'\n'\
'MAX MEM:                %M MB'$'\n'\
'------------------------END-----------------------------'

[ -f ~/.fzf.zsh ] && source ~/.fzf.zsh

# =============================================================================
#                           Customized functions
# =============================================================================
# --------better csv viewer ----
function pretty_csv {
    perl -pe 's/((?<=,)|(?<=^)),/ ,/g;' "$@" | column -t -s, | less  -F -S -X -K
}
