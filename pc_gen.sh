prefix=$1
includedir=$2

if [[ $includedir == $prefix* ]]
then
  includedir='${prefix}'${includedir#"$prefix"}
fi

cat << EOF
prefix=$prefix
includedir=$includedir

Name: qoi
Description: The "Quite OK Image Format" for fast, lossless image compression
Version: 0
URL: https://qoiformat.org/
License: MIT
Cflags: -I\${includedir}
EOF
