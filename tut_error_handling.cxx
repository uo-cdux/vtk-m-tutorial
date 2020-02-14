#include <vtkm/cont/Initialize.h>
#include <vtkm/filter/Contour.h>
#include <vtkm/io/reader/VTKDataSetReader.h>
#include <vtkm/io/writer/VTKDataSetWriter.h>

int main(int argc, char** argv)
{
  auto opts = vtkm::cont::InitializeOptions::DefaultAnyDevice;
  vtkm::cont::InitializeResult config = vtkm::cont::Initialize(argc, argv, opts);

  try
  {
    vtkm::io::reader::VTKDataSetReader reader("data/kitchen.vtk");

    // PROBLEM! ... we aren't reading from a file, so we have an empty vtkm::cont::DataSet.
    //vtkm::cont::DataSet ds_from_file = reader.ReadDataSet();
    vtkm::cont::DataSet ds_from_file;

    vtkm::filter::Contour contour;
    contour.SetActiveField("c1");
    contour.SetNumberOfIsoValues(3);
    contour.SetIsoValue(0, 0.05);
    contour.SetIsoValue(1, 0.10);
    contour.SetIsoValue(2, 0.15);

    vtkm::cont::DataSet ds_from_mc = contour.Execute(ds_from_file);
    vtkm::io::writer::VTKDataSetWriter writer("out_mc.vtk");
    writer.WriteDataSet(ds_from_mc); //! Exception here

    std::cerr << "FAILED TEST, Exception not raised" << std::endl;
  }
  catch (const vtkm::cont::Error& error)
  {
    return 0;
  }

  return 1;
}
