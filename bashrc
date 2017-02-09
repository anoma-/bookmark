function bk()
{
  path=$(book $1)
  cd $path
}

_bk_options=$(cat ~/.bookmarks_auto)
complete -W "${_bk_options}" 'bk'

