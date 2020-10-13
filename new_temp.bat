@echo off
chcp 65001

if "%1" == "" (
    echo "Input parameter is null."
) else (
    hexo new draft %1
    move source\_drafts\%1.md source\_temps\

    if not "%2" == "" (
        hexo new draft %2
        move source\_drafts\%2.md source\_temps\
        if not "%3" == "" (
            hexo new draft %3
            move source\_drafts\%3.md source\_temps\
        )
    )
)