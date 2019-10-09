#include <vtkm/VectorAnalysis.h>

#include <vtkm/cont/Initialize.h>
#include <vtkm/cont/Invoker.h>

#include <vtkm/io/reader/VTKDataSetReader.h>
#include <vtkm/io/writer/VTKDataSetWriter.h>

#include <vtkm/filter/Gradient.h>

#include <vtkm/worklet/WorkletMapField.h>

class ComputeMagnitude : public vtkm::worklet::WorkletMapField
{
public:
  using ControlSignature = void(FieldIn inputVectors, FieldOut outputMagnitudes);
  using ExecutionSignature = void(_1, _2);

  VTKM_EXEC void operator()(const vtkm::Vec3f& inVector,
                            vtkm::FloatDefault& outMagnitude) const
  {
    outMagnitude = vtkm::Magnitude(inVector);
  }
};

#include <vtkm/filter/FilterField.h>

class FieldMagnitude : public vtkm::filter::FilterField<FieldMagnitude>
{
public:

  template<typename Policy>
  VTKM_CONT vtkm::cont::DataSet DoExecute(
    const vtkm::cont::DataSet& inDataSet,
    const vtkm::cont::ArrayHandle<vtkm::Vec3f>& inField,
    const vtkm::filter::FieldMetadata& fieldMetadata,
    vtkm::filter::PolicyBase<Policy>)
  {
    vtkm::cont::ArrayHandle<vtkm::Vec3f> outField;
    this->Invoke(ComputeMagnitude{}, inField, outField);

    std::string outFieldName = this->GetOutputFieldName();
    if (outFieldName == "")
    {
      outFieldName = fieldMetadata.GetName() + "_magnitude";
    }

    return vtkm::filter::CreateResult(inDataSet,
                                                outField,
                                                outFieldName,
                                                fieldMetadata);
  }
};

int main(int argc, char** argv)
{
  auto opts = vtkm::cont::InitializeOptions::DefaultAnyDevice;
  vtkm::cont::InitializeResult config = vtkm::cont::Initialize(argc, argv, opts);

  const char *input = "data/kitchen.vtk";
  vtkm::io::reader::VTKDataSetReader reader(input);
  vtkm::cont::DataSet ds_from_file = reader.ReadDataSet();

  vtkm::filter::Gradient grad;
  grad.SetActiveField("c1");
  vtkm::cont::DataSet ds_from_grad = grad.Execute(ds_from_file);

  FieldMagnitude mag;
  mag.SetActiveField("Gradients");
  vtkm::cont::DataSet mag_grad = grad.Execute(ds_from_grad);

  vtkm::io::writer::VTKDataSetWriter writer("out_mag_grad.vtk");
  writer.WriteDataSet(mag_grad);

  return 0;
}
