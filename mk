#! /bin/bash


function mark()
{
  egrep -w "^$1:*" "$HOME/.bookmarks"

  if [[ $? -eq 0 ]]; then
    echo "Already have $1 marked"
    return 
  fi
  
  echo "$1:$PWD" >> "$HOME/.bookmarks"
  echo "$1" >> $HOME/.bookmarks_auto

}

mark $1
