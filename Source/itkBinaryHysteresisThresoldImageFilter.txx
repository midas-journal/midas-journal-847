#include "itkBinaryHysteresisThresoldImageFilter.h"
#include <itkConstantBoundaryCondition.h>


//--------------------------------------------------
template<class TInputImage, class TOutputPixelType>
BinaryHysteresisThresoldImageFilter<TInputImage, TOutputPixelType>
::BinaryHysteresisThresoldImageFilter()
//--------------------------------------------------
{
  this->SetInsideValue(itk::NumericTraits<TOutputPixelType>::max());
  this->SetOutsideValue(itk::NumericTraits<TOutputPixelType>::Zero);
}

//--------------------------------------------------
template<class TInputImage, class TOutputPixelType>
BinaryHysteresisThresoldImageFilter<TInputImage, TOutputPixelType>
::~BinaryHysteresisThresoldImageFilter()
//--------------------------------------------------
{
}

//--------------------------------------------------
template<class TInputImage, class TOutputPixelType>
void
BinaryHysteresisThresoldImageFilter<TInputImage, TOutputPixelType>
::GenerateData()
//--------------------------------------------------
{
  try
  {
    this->m_inputImg =
      dynamic_cast<const TInputImage*>(ProcessObject::GetInput(0));
    this->m_outputImg =
      dynamic_cast<TOutputImage*>(this->ProcessObject::GetOutput(0));

    this->m_outputImg->SetSpacing(this->m_inputImg->GetSpacing());
    this->m_outputImg->SetOrigin(this->m_inputImg->GetOrigin());
    this->m_outputImg->SetRegions(this->m_inputImg->GetRequestedRegion());
    this->m_outputImg->Allocate();
    this->m_queueImg = TOutputImage::New();
    this->m_queueImg->SetSpacing(this->m_inputImg->GetSpacing());
    this->m_queueImg->SetOrigin(this->m_inputImg->GetOrigin());
    this->m_queueImg->SetRegions(this->m_inputImg->GetRequestedRegion());
    this->m_queueImg->Allocate();
    this->m_queueImg->FillBuffer(itk::NumericTraits<TOutputPixelType>::Zero);

    // 1st pass raw threshold

    //Iterators
    InputConstIteratorType
      icit(this->m_inputImg, this->m_inputImg->GetRequestedRegion());
    OutputIteratorType
      oit(this->m_outputImg, this->m_outputImg->GetRequestedRegion());
    OutputIteratorType
      qoit(this->m_queueImg, this->m_queueImg->GetRequestedRegion());

    for (icit.GoToBegin(), oit.GoToBegin(), qoit.GoToBegin();
         !icit.IsAtEnd() && !oit.IsAtEnd(); ++icit, ++oit, ++qoit) 
      {
      if (icit.Get() >= m_UpperThreshold) 
        {
        oit.Set(m_InsideValue);
        m_nodeList.push_back(oit.GetIndex());
        qoit.Set(itk::NumericTraits<TOutputPixelType>::One);
        } 
      else
        oit.Set(m_OutsideValue);
      }

    // 2nd pass hysteresis
    typename OutputNeighborhoodIteratorType::RadiusType radius;
    radius.Fill(1);
    InputConstNeighborhoodIteratorType
      icnit(radius, this->m_inputImg, this->m_inputImg->GetRequestedRegion());

    itk::ConstantBoundaryCondition<TInputImage> cbc;
    icnit.OverrideBoundaryCondition(&cbc);
    while (!m_nodeList.empty()) 
      {
      OutputIndexType pixelIndex = m_nodeList.front();
      m_nodeList.pop_front();
      icnit.SetLocation(pixelIndex);
      for (size_t i = 0; i < icnit.Size(); i++) 
        {
        InputPixelType pxl = icnit.GetPixel(i);
        if (pxl < m_UpperThreshold  && pxl > m_LowerThreshold) 
          {
          qoit.SetIndex(icnit.GetIndex(i));
          if (qoit.Get() == itk::NumericTraits<TOutputPixelType>::Zero) 
            {
            oit.SetIndex(icnit.GetIndex(i));
            oit.Set(m_InsideValue);
            m_nodeList.push_back(icnit.GetIndex(i));
            qoit.Set(itk::NumericTraits<TOutputPixelType>::One);
            }
          }
        }
      }
    }
  catch (const itk::ExceptionObject& err)
    {
    throw err;
    }
  catch (const std::exception& e)
    {
    itkExceptionMacro(
      << "itkBinaryHysteresisThresoldImageFilter::GenerateData() - Unexpected"
          " std::exception!"
      << e.what()
      << std::endl );
    }
  catch (...)
    {
    itkExceptionMacro(
      << "itkBinaryHysteresisThresoldImageFilter::GenerateData() - Unexpected"
          " error!"
      << std::endl );
    }
}

/**
 *  Print Self
 */
template<class TInputImage, class TOutputPixelType>
void BinaryHysteresisThresoldImageFilter<TInputImage, TOutputPixelType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "BinaryHysteresisThresoldImageFilter:" << std::endl;
  os << indent << indent << "lowerThreshold      : " << m_LowerThreshold
    << std::endl;
  os << indent << indent << "upperThreshold      : " << m_UpperThreshold
    << std::endl;
  os << indent << indent << "insideValue         : " << m_InsideValue
    << std::endl;
  os << indent << indent << "outsideValue        : " << m_OutsideValue
    << std::endl;
}
