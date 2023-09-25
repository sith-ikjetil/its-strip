function InstallUbuntu()
{
    if [ ! -d "/usr/local/man/man1" ]
    then
        sudo mkdir /usr/local/man/man1
    fi

    sudo cp its-strip.1 /usr/local/man/man1
    sudo chmod go+r /usr/local/man/man1/its-strip.1
    printf "Install Ubuntu Complete\n"
}

function InstallFedora()
{
    # fedora
    if [ ! -d "/usr/local/share/man/man1" ]
    then
        sudo mkdir "/usr/local/share/man/man1"
    fi        
    sudo cp its-strip.1 /usr/local/share/man/man1
    sudo chmod go+r /usr/local/share/man/man1/its-strip.1
    printf "Install Fedora Complete\n"
}


printf "Installing man page\n"

./render.sh

if [ -d "/usr/local/man" ]
then
    InstallUbuntu
fi

if [ -d "/usr/local/share/man" ]
then
    InstallFedora
fi
