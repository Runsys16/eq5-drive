#!/bin/bash
#-----------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------
SIMU="YES"
NSRC="-1"
NDST="-1"
CMD="cp"
#-----------------------------------------------------------------------------------------
# fonction help()
function help()
{
	cat <<EOF
change le numero de version
  ./version.sh SRC DST [option] 
  
  -f|--force           : pas de simulation
  -h|--help            : affiche cette aide
  -c|--copy            : fait un copy
  -m|--move            : renomme
  
  ex:
    ./version.sh 1.0 2.0 -m  -f    : renomme tous les fichiers contenant "1.0" en changeant en "2.0"
    
    ./version.sh 1.0 2.0 -c        : copy tous les fichiers contenant "1.25" en changeant en "2.12"
                                     mais c'est une simu

EOF
}
#-----------------------------------------------------------------------------------------
#
function lit_num()
{
    if [ "$NSRC" == "-1" ] 
    then
        NSRC=$1
    else
        if [ "$NDST" == "-1" ] 
        then
            NDST=$1
        fi
    fi
}
#-----------------------------------------------------------------------------------------
#
function erreur()
{
    echo Erreur : $1
    help
    exit
}
#-----------------------------------------------------------------------------------------
#
numargs=$#

#if [ $numargs == 0 ]
#then
#    help
#    exit
#fi

for ((i=1 ; i <= numargs ; i++))
do
	arg=$1
	case $arg in
		-f|--force)
			SIMU="-n"
			;;
		-c|--copy)
			CMD="cp"
			;;
		-m|--move)
			CMD="mv"
			;;
		-h|--help)
			help
            exit
            ;;
		*)
		    lit_num $arg
			;;
	esac
	shift
done
#-----------------------------------------------------------------------------------------
#

if [ "$NSRC" == "-1" ]
then
    erreur "Pas de numero source"
fi

if [ "$NDST" == "-1" ]
then
    erreur "Pas de numero destination"
fi


if [ "$SIMU" == "YES" ]
then
    echo "***SIMU ***"
fi


for f in $(ls *"$NSRC"*)
do
    new=$(echo $f | sed -e "s/$NSRC/$NDST/g")

    echo $CMD $f $new

    if [ "$SIMU" == "-n" ]
    then
        $($CMD $f $new)
    fi
done



