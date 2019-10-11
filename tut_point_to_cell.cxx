
#include <vtkm/worklet/WorkletMapTopology.h>

#include <vtkm/io/reader/VTKDataSetReader.h>
#include <vtkm/io/writer/VTKDataSetWriter.h>

namespace vtkm
{
namespace worklet
{

struct ConvertPointFieldToCells : vtkm::worklet::WorkletVisitCellsWithPoints
{
  using ControlSignature = void(CellSetIn topology,
                                FieldInPoint inPointField,
                                FieldOutCell outCellField);
  using ExecutionSignature = void(_2 inPointField, _3 outCellField);
  using InputDomain = _1;

  template <typename InPointFieldVecType, typename OutCellFieldType>
  void operator()(const InPointFieldVecType& inPointFieldVec,
                  OutCellFieldType& outCellField) const
  {
    vtkm::IdComponent numPoints = inPointFieldVec.GetNumberOfComponents();

    outCellField = OutCellFieldType(0);
    for (vtkm::IdComponent pointIndex = 0; pointIndex < numPoints; ++pointIndex)
    {
      outCellField = outCellField + inPointFieldVec[pointIndex];
    }
    outCellField = outCellField / OutCellFieldType(numPoints);
  }
};

} // namespace worklet
} // namespace vtkm

#include <vtkm/filter/FilterField.h>

namespace vtkm
{
namespace filter
{

struct ConvertPointFieldToCells : vtkm::filter::FilterField<ConvertPointFieldToCells>
{
  template<typename ArrayHandleType, typename Policy>
  VTKM_CONT vtkm::cont::DataSet DoExecute(
    const vtkm::cont::DataSet& inDataSet,
    const ArrayHandleType& inField,
    const vtkm::filter::FieldMetadata& fieldMetadata,
    vtkm::filter::PolicyBase<Policy>);
};

template<typename ArrayHandleType, typename Policy>
VTKM_CONT cont::DataSet ConvertPointFieldToCells::DoExecute(
  const vtkm::cont::DataSet& inDataSet,
  const ArrayHandleType& inField,
  const vtkm::filter::FieldMetadata& fieldMetadata,
  vtkm::filter::PolicyBase<Policy> policy)
{
  VTKM_IS_ARRAY_HANDLE(ArrayHandleType);

  using ValueType = typename ArrayHandleType::ValueType;

  vtkm::cont::ArrayHandle<ValueType> outField;
  this->Invoke(vtkm::worklet::ConvertPointFieldToCells{},
               vtkm::filter::ApplyPolicyCellSet(inDataSet.GetCellSet(), policy),
               inField,
               outField);

  std::string outFieldName = this->GetOutputFieldName();
  if (outFieldName == "")
  {
    outFieldName = fieldMetadata.GetName();
  }

  return vtkm::filter::CreateResultFieldCell(inDataSet, outField, outFieldName);
}

} // namespace filter
} // namespace vtkm


int main()
{
  const char *input = "kitchen.vtk";
  vtkm::io::reader::VTKDataSetReader reader(input);
  vtkm::cont::DataSet ds_from_file = reader.ReadDataSet();

  vtkm::filter::ConvertPointFieldToCells pointToCell;
  pointToCell.SetActiveField("c1");
  vtkm::cont::DataSet ds_from_convert = pointToCell.Execute(ds_from_file);

  vtkm::io::writer::VTKDataSetWriter writer("out_point_to_cell.vtk");
  writer.WriteDataSet(ds_from_convert);

  return 0;
}
