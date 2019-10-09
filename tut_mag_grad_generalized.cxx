#include <vtkm/VecTraits.h>
#include <vtkm/VectorAnalysis.h>

#include <vtkm/cont/ArrayHandle.h>
#include <vtkm/cont/Initialize.h>
#include <vtkm/cont/Invoker.h>
#include <vtkm/cont/VariantArrayHandle.h>

#include <vtkm/io/reader/VTKDataSetReader.h>
#include <vtkm/io/writer/VTKDataSetWriter.h>

#include <vtkm/filter/Gradient.h>

#include <vtkm/worklet/WorkletMapField.h>

namespace vtkm
{
namespace worklet
{

struct Magnitude
{
  class ComputeMagnitude : public vtkm::worklet::WorkletMapField
  {
  public:
    using ControlSignature = void(FieldIn inputVectors, FieldOut outputMagnitudes);
    using ExecutionSignature = void(_1, _2);

    template<typename T, vtkm::IdComponent Size>
    VTKM_EXEC void operator()(const vtkm::Vec<T, Size>& inVector, T& outMagnitude) const
    {
      outMagnitude = vtkm::Magnitude(inVector);
    }
  };
};

} // namespace worklet
} // namespace vtkm

#include <vtkm/filter/FilterField.h>

namespace vtkm
{
namespace filter
{

class FieldMagnitude : public vtkm::filter::FilterField<FieldMagnitude>
{
public:
  using SupportedTypes = vtkm::ListTagBase<vtkm::Vec2f_32,
                                           vtkm::Vec2f_64,
                                           vtkm::Vec3f_32,
                                           vtkm::Vec3f_64,
                                           vtkm::Vec4f_32,
                                           vtkm::Vec4f_64>;

  template<typename ArrayHandleType, typename Policy>
  VTKM_CONT vtkm::cont::DataSet DoExecute(const vtkm::cont::DataSet& inDataSet,
                                          const ArrayHandleType& inField,
                                          const vtkm::filter::FieldMetadata& fieldMetadata,
                                          vtkm::filter::PolicyBase<Policy>);
};

template<typename ArrayHandleType, typename Policy>
VTKM_CONT cont::DataSet FieldMagnitude::DoExecute(const vtkm::cont::DataSet& inDataSet,
                                                  const ArrayHandleType& inField,
                                                  const vtkm::filter::FieldMetadata& fieldMetadata,
                                                  vtkm::filter::PolicyBase<Policy>)
{
  VTKM_IS_ARRAY_HANDLE(ArrayHandleType);

  using ComponentType =
    typename vtkm::VecTraits<typename ArrayHandleType::ValueType>::ComponentType;

  vtkm::cont::ArrayHandle<ComponentType> outField;
  this->Invoke(vtkm::worklet::Magnitude{}, inField, outField);

  std::string outFieldName = this->GetOutputFieldName();
  if (outFieldName == "")
  {
    outFieldName = fieldMetadata.GetName() + "_magnitude";
  }

  return vtkm::filter::CreateResult(inDataSet, outField, outFieldName, fieldMetadata);
}

} // namespace filter
} // namespace vtkm

int main(int argc, char** argv)
{
  auto opts = vtkm::cont::InitializeOptions::DefaultAnyDevice;
  vtkm::cont::InitializeResult config = vtkm::cont::Initialize(argc, argv, opts);

  const char* input = "data/kitchen.vtk";
  vtkm::io::reader::VTKDataSetReader reader(input);
  vtkm::cont::DataSet ds_from_file = reader.ReadDataSet();

  vtkm::filter::Gradient grad;
  grad.SetActiveField("c1");
  vtkm::cont::DataSet ds_from_grad = grad.Execute(ds_from_file);

  vtkm::filter::FieldMagnitude mag;
  mag.SetActiveField("Gradients");
  vtkm::cont::DataSet mag_grad = grad.Execute(ds_from_grad);

  vtkm::io::writer::VTKDataSetWriter writer("out_mag_grad.vtk");
  writer.WriteDataSet(mag_grad);

  return 0;
}
