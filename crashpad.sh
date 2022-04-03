set -euxo pipefail

msg="$(gdate --iso-8601=seconds)"

cmake \
  -DSENTRY_BACKEND=crashpad \
  -DBUILD_SHARED_LIBS=OFF \
  -B out/basic
cmake \
  --build out/basic --parallel

cmake \
  -DSENTRY_BACKEND=crashpad \
  -DCMAKE_OSX_ARCHITECTURES=x86_64 \
  -DBUILD_SHARED_LIBS=OFF \
  -B out/x86-64
cmake --build out/x86-64 --parallel
 
# cmake \
#   -DCMAKE_VERBOSE_MAKEFILE=ON \
#   -DSENTRY_BACKEND=crashpad \
#   -DCMAKE_OSX_ARCHITECTURES=x86_64 \
#   -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
#   -DBUILD_SHARED_LIBS=OFF \
#   -B out/10-15-x86_64
# cmake --build out/10-15-x86_64 --parallel
# 
# cmake \
#   -DCMAKE_VERBOSE_MAKEFILE=ON \
#   -DSENTRY_BACKEND=crashpad \
#   -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
#   -DBUILD_SHARED_LIBS=OFF \
#   -B out/10-15
# cmake --build out/10-15 --parallel
#
#cmake \
#  -DSENTRY_BACKEND=crashpad \
#  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 \
#  -DBUILD_SHARED_LIBS=OFF \
#  -B out/11-0
#cmake --build out/11-0 --parallel

set +e
./out/basic/sentry_example2 "${msg}"
./out/x86-64/sentry_example2 "${msg}"
#./out/10-15-x86_64/sentry_example2 "${msg}"
#./out/10-15/sentry_example2 "${msg}"
#./out/11-0/sentry_example2 "${msg}"
