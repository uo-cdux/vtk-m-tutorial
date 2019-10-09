cmake_minimum_required(VERSION 3.10)
project(VTKm_tut)

find_package(VTKm REQUIRED)

add_executable(tut_io tut_io.cxx)
target_link_libraries(tut_io vtkm_cont vtkm_filter)

add_executable(tut_contour tut_contour.cxx)
target_link_libraries(tut_contour vtkm_cont vtkm_filter)

add_executable(tut_contour_2fields tut_contour_2fields.cxx)
target_link_libraries(tut_contour_2fields vtkm_cont vtkm_filter)

add_executable(tut_2filters tut_2filters.cxx)
target_link_libraries(tut_2filters vtkm_cont vtkm_filter)

add_executable(tut_mag_grad tut_mag_grad.cxx)
target_link_libraries(tut_mag_grad vtkm_cont vtkm_filter)

add_executable(tut_rendering tut_rendering.cxx)
target_link_libraries(tut_rendering vtkm_cont vtkm_filter vtkm_rendering)

add_executable(tut_error_handling tut_error_handling.cxx)
target_link_libraries(tut_error_handling vtkm_cont vtkm_filter)

add_executable(tut_logging tut_logging.cxx)
target_link_libraries(tut_logging vtkm_cont vtkm_filter)
