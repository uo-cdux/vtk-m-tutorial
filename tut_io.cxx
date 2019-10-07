// Example 1: very simple VTK-m program.
// Read data set, write it out.
//
#include <vtkm/io/reader/VTKDataSetReader.h>
#include <vtkm/io/writer/VTKDataSetWriter.h>

int main()
{
  const char *input = "kitchen.vtk";
  vtkm::io::reader::VTKDataSetReader reader(input);
  vtkm::cont::DataSet ds = reader.ReadDataSet();
  vtkm::io::writer::VTKDataSetWriter writer("out_io.vtk");
  writer.WriteDataSet(ds);

  return 0;
}
