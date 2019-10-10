cmake_minimum_required(VERSION 3.10)
project(VTKm_tut)

find_package(VTKm REQUIRED)

add_executable(tut_io tut_io.cxx)
target_link_libraries(tut_io PUBLIC vtkm_filter)

add_executable(tut_contour tut_contour.cxx)
target_link_libraries(tut_contour PUBLIC vtkm_filter)

add_executable(tut_contour_2fields tut_contour_2fields.cxx)
target_link_libraries(tut_contour_2fields PUBLIC vtkm_filter)

add_executable(tut_2filters tut_2filters.cxx)
target_link_libraries(tut_2filters PUBLIC vtkm_filter)

add_executable(tut_mag_grad tut_mag_grad.cxx)
target_link_libraries(tut_mag_grad PUBLIC vtkm_filter)

add_executable(tut_rendering tut_rendering.cxx)
target_link_libraries(tut_rendering PUBLIC vtkm_filter vtkm_rendering)

add_executable(tut_error_handling tut_error_handling.cxx)
target_link_libraries(tut_error_handling PUBLIC vtkm_filter)

add_executable(tut_logging tut_logging.cxx)
target_link_libraries(tut_logging PUBLIC vtkm_filter)

add_executable(tut_point_to_cell tut_point_to_cell.cxx)
target_link_libraries(tut_point_to_cell PUBLIC vtkm_cont vtkm_filter)

add_executable(tut_extract_edges tut_extract_edges.cxx)
target_link_libraries(tut_extract_edges PUBLIC vtkm_cont vtkm_filter)
