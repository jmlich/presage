#!/bin/bash

set -e

PROGPATH=$(dirname "$0")

if [ "$#" -ne 5 ]; then
    echo "Usage: $0 Language langcode version affix dictionary"
    echo
    echo "Language: Specify language in English starting with the capital letter, ex 'Estonian'"
    echo "langcode: Specify language code, ex 'en_US'. Use the same notation as Hunspell dictionaries."
    echo "version: Version of the language package, ex '1.0.0'"
    echo "affix: Affix file"
    echo "dictionary: Dictionary file"
    echo
    echo "When finished, language dictionary will be packaged into RPM in the current directory"
    echo
    echo "The script requires rpmbuild to be installed. Note that rpmbuild can be installed on distributions that don't use RPM for packaging"
    echo
    exit 0
fi

L=$1
CODE=$2
VERSION=$3
AFF=$4
DICT=$5

NAME=hunspell-lang-$CODE

TMPDIR=`mktemp -d`

mkdir -p $TMPDIR/$NAME-$VERSION/hunspell
mkdir -p $TMPDIR/$NAME-$VERSION/rpm
cp "$AFF" $TMPDIR/$NAME-$VERSION/hunspell/${CODE}.aff
cp "$DICT" $TMPDIR/$NAME-$VERSION/hunspell/${CODE}.dic
cp "$PROGPATH"/hunspell-lang-XX.spec $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec
sed -i "s/__langcode__/$CODE/"  $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec
sed -i "s/__Language__/$L/"  $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec
sed -i "s/__version__/$VERSION/"  $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec

tar -C $TMPDIR -cJf $TMPDIR/$NAME-$VERSION.tar.xz $NAME-$VERSION

mkdir -p $HOME/rpmbuild/SOURCES
mkdir -p $HOME/rpmbuild/SPECS

cp $TMPDIR/$NAME-$VERSION.tar.xz $HOME/rpmbuild/SOURCES
cp $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec $HOME/rpmbuild/SPECS

rm -rf $TMPDIR

rm -rf $HOME/rpmbuild/BUILD/$NAME-$VERSION
rpmbuild -ba --nodeps $HOME/rpmbuild/SPECS/$NAME.spec

mkdir -p RPMS
cp $HOME/rpmbuild/RPMS/noarch/$NAME-$VERSION-*.rpm .
