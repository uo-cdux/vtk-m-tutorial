#!/bin/bash
path=$(pwd)

cmake_cmd=cmake
if [ "$1" != "" ]; then
  echo "Using the cmake provided at $1"
  cmake_cmd="$1"
else
  echo "Defaulting to cmake found on path"
fi

source_dir="${path}/sources"
build_dir="${path}/builds"
install_dir="${path}/install"

#Note this requires CMake 3.13+ as we are using `-S` and `-B`

#download the required repos
if [ ! -f "${source_dir}/vtkm/CMakeLists.txt" ]; then
  echo "cloning vtkm to ${source_dir}/vtkm"
  git clone --depth=1 https://gitlab.kitware.com/vtk/vtk-m.git "${source_dir}/vtkm"
fi
if [ ! -f "${source_dir}/tutorial/CMakeLists.txt" ]; then
  echo "cloning vtk-m tutorials to ${source_dir}/tutorial"
  git clone --depth=1 https://github.com/uo-cdux/vtk-m-tutorial.git "${source_dir}/tutorial"
fi

#
# Configure && Build VTK-m
# We cache the build directory so we run this each time

echo "Configuring vtk-m with"
echo "${cmake_cmd} -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=MinSizeRel VTKm_ENABLE_RENDERING=ON -DVTKm_ENABLE_TESTING=OFF -DVTKm_ENABLE_LOGGING=ON -DVTKm_USE_64BIT_IDS=OFF -DVTKm_USE_DOUBLE_PRECISION=ON"
mkdir -p "${build_dir}/vtkm"
cd "${build_dir}/vtkm"
${cmake_cmd} -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=MinSizeRel -DVTKm_ENABLE_RENDERING=ON -DVTKm_ENABLE_TESTING=OFF -DVTKm_ENABLE_LOGGING=ON -DVTKm_USE_64BIT_IDS=OFF -DVTKm_USE_DOUBLE_PRECISION=ON  "${source_dir}/vtkm"
echo "Building vtk-m"
${cmake_cmd} --build "${build_dir}/vtkm" -j 3
${cmake_cmd} --install "${build_dir}/vtkm" --prefix "${install_dir}"


cd ${path}
#
# Configure && Build tutorial
echo "Configuring tutorial"
mkdir -p "${build_dir}/tutorial"
cd "${build_dir}/tutorial"
${cmake_cmd} -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="${install_dir}" "${source_dir}/tutorial"
cd ${path}
echo "Building tutorial"
${cmake_cmd} --build "${build_dir}/tutorial" -j 3
