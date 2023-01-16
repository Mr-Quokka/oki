#!/bin/bash

_oki_init() {
  case "$prev" in
    --name)
      return 0
    ;;
  esac
  if [ "$COMP_CWORD" -eq 2 ]; then
    opts='c cpp'
  else
    opts='--lib --name'
  fi
  COMPREPLY=($(compgen -W "${opts}" -- "${cur}"))
}

_oki_install() {
  case "$prev" in
    --registry)
      return 0
    ;;
  esac
  opts='--registry'
  COMPREPLY=($(compgen -W "${opts}" -- "${cur}"))
}

_oki_package() {
  opts='--list --no-verify'
  COMPREPLY=($(compgen -W "${opts}" -- "${cur}"))
}

_oki_registrable() {
  [ "$prev" = '--registry' ] && return 0
  opts='--registry'
  COMPREPLY=($(compgen -W "${opts}" -- "${cur}"))
}

_oki() {
  local cur cmd opts
  prev="${COMP_WORDS[COMP_CWORD-1]}"
  cur="${COMP_WORDS[COMP_CWORD]}"

  if [ "${COMP_CWORD}" -eq 1 ]; then
    opts="build fetch init install list makefile package publish remove show tree"
    COMPREPLY=($(compgen -W "${opts}" -- "${cur}") )
    return 0
  fi

  cmd="${COMP_WORDS[1]}"
  case "$cmd" in
    init)
      _oki_init
    ;;
    install)
      _oki_install
    ;;
    package)
      _oki_package
    ;;
    list | publish | show)
      _oki_registrable
    ;;
  esac
}

complete -F _oki oki
